%{
#include <iostream>
#include <string>
#include <sstream>
#include "node.h"
#include "model.h"

#include "symbolt.h"

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

void yyerror(const char *s);

%}
%locations

%union
{
	int                   ival;
	float                 fval;
	char*                 sval;
  node*                 nval;
  std::vector<node*>*   nval_list;
}

// terminals
%token MODEL TIME
%token N_DIST U_DIST E_DIST G_DIST DD_DIST
%token INFTY

%token MODE INVT FLOW JUMP INIT GOAL 
%token D_DT TRANS PRIME

%token SQRT EXP LOGN SIN COS TAN ASIN ACOS ATAN ABS
%token NOT AND OR XOR IMPLY
%token PLUS MINUS TIMES DIVIDE POWER
%token EQ GT LT GE LE NE
%token TRUE FALSE
%token DEFINE

%token <sval> m_type
%token <sval> identifier
%token <sval> number
%token <fval> n_float
%token <ival> n_int

%left EQ LT GT LE GE NE
%left PLUS MINUS
%left TIMES DIVIDE
%precedence UMINUS UPLUS
%right POWER

%type<sval> reset_var
%type<nval_list> props dd_pairs
%type<nval> prop expr dist dd_pair

// to compile
//bison -d -o pdrhparser.c pdrhparser.y && flex -o pdrhlexer.c pdrhlexer.l && g++ -O2 -std=c++11 `/home/fedor/dreal3/build/release/bin/capd-config --cflags` pdrhparser.h pdrhparser.c pdrhlexer.c ../../model.cpp -lfl `/home/fedor/dreal3/build/release/bin/capd-config --libs` -o pdrh && ./pdrh ../../test/parser/test1.pdrh

// declaring some variables
%{
pdrh::mode *cur_mode = new pdrh::mode;
pdrh::mode::jump *cur_jump = new pdrh::mode::jump;
std::vector<pdrh::state> cur_states;
std::vector<pdrh::mode*> cur_path;
std::map<node*, node*> cur_dd;
std::map<std::string, node*> define_map;

using namespace std;
using namespace pdrh;
%}

%%
pdrh:
	| declarations modes init goal      { ; }
	| model declarations modes init goal { ; }

model:
    MODEL ':' m_type ';' 
{
  std::string type_str($3);
  if(type_str == "ha")           pdrh::model_type = pdrh::HA;
  else if(type_str == "pha")     pdrh::model_type = pdrh::PHA;
  else if(type_str == "nha")     pdrh::model_type = pdrh::NHA;
  else if(type_str == "npha")    pdrh::model_type = pdrh::NPHA;
}

declarations:
	declarations declaration { ; }
	| declaration { ; }

declaration:
	var_declaration { ; }
	| dist_declaration { ; }
	| const_declaration { ; }

const_declaration:
    DEFINE identifier expr      
{
  // adding the value into the map
  define_map[$2] = $3;
  // scanning the define map for the constants defined before
  for(auto it = define_map.begin(); it != define_map.end(); it++)
  {
    // if the value is a terminal node
    if(it->second->operands.size() == 0)
    {
      // if the value is an identifier
      if(it->second->value == $2)
      {
        define_map[it->first] = $3;
        define_map.erase($2);
      }
    }
  }
}

var_declaration:
	'[' expr ',' expr ']' identifier ';'
{
  if(!pdrh::var_exists($6))
  {
    pdrh::push_var($6, $2, $4);
  }
  else
  {
    std::stringstream s;
    s << "multiple declaration of variable \"" << $6 << "\"";
    yyerror(s.str().c_str());
  }
}
	| '[' expr ']' identifier ';'
{
  if(!pdrh::var_exists($4))
  {
    pdrh::push_var($4, $2, $2);
  }
  else
  {
    std::stringstream s;
    s << "multiple declaration of variable \"" << $4 << "\"";
    yyerror(s.str().c_str());
  }
}
	| '[' expr ',' expr ']' TIME ';'
{ 
  pdrh::push_time_bounds($2, $4); 
}

dist_declaration:
  G_DIST '(' expr ',' expr ')' identifier ';'
{
  if(!pdrh::var_exists($7))
  {
    pdrh::push_var($7, new node("-infty"), new node("infty"));
    pdrh::distribution::push_gamma($7, $3, $5);
  }
  else
  {
    std::stringstream s;
    s << "multiple declaration of variable \"" << $7 << "\"";
    yyerror(s.str().c_str());
  }
}
  | N_DIST '(' expr ',' expr ')' identifier ';'
{
  if(!pdrh::var_exists($7))
  {
    pdrh::push_var($7, new node("-infty"), new node("infty"));
    pdrh::push_rv($7, pdrh::distribution::normal_to_node($7, $3, $5),
      new node("-infty"), new node("infty"), $3);
    pdrh::distribution::push_normal($7, $3, $5);
  }
  else
  {
    std::stringstream s;
    s << "multiple declaration of variable \"" << $7 << "\"";
    yyerror(s.str().c_str());
  }
}
  | U_DIST '(' expr ',' expr ')' identifier ';'
{
  if(!pdrh::var_exists($7))
  {
    pdrh::push_var($7, $3, $5);
    pdrh::push_rv($7, pdrh::distribution::uniform_to_node($3, $5), $3, $5, $3);
    pdrh::distribution::push_uniform($7, $3, $5);
  }
  else
  {
    std::stringstream s;
    s << "multiple declaration of variable \"" << $7 << "\"";
    yyerror(s.str().c_str());
  }
}
  | E_DIST '(' expr ')' identifier ';'
{
  if(!pdrh::var_exists($5))
  {
    pdrh::push_var($5, new node("0"), new node("infty"));
    pdrh::push_rv($5, pdrh::distribution::exp_to_node($5, $3),
      new node("0"), new node("infty"), new node("0"));
    pdrh::distribution::push_exp($5, $3);
  }
  else
  {
    std::stringstream s;
    s << "multiple declaration of variable \"" << $5 << "\"";
    yyerror(s.str().c_str());
  }
}
  | DD_DIST '(' dd_pairs ')' identifier ';'
{
  if(!pdrh::var_exists($5))
  {
    push_var($5, new node("-infty"), new node("infty"));
    push_dd($5, cur_dd);
    cur_dd.clear();
  }
  else
  {
    stringstream s;
    s << "multiple declaration of variable \"" << $5 << "\"";
    yyerror(s.str().c_str());
  }
}

dist:
  G_DIST '(' expr ',' expr ')'
{ 
  $$ = new node("dist_gamma", {$3, $5}); 
}
  | N_DIST '(' expr ',' expr ')'
{
  $$ = new node("dist_normal", {$3, $5});
}
  | U_DIST '(' expr ',' expr ')'
{
  $$ = new node("dist_uniform", {$3, $5});
}
  | E_DIST '(' expr ')'
{
  $$ = new node("dist_exp", {$3});
}
  | DD_DIST '(' dd_pairs ')'
{
  vector<node*> tmp;
  for(size_t i = 0; i < $3->size(); i++)
  {
    tmp.push_back($3->at(i));
  }
  delete $3;
  $$ = new node("dist_discrete", tmp);
}

dd_pairs:
    dd_pairs ',' dd_pair        
{
  $1->push_back($3);
  $$ = $1;
}
  | dd_pair
{
  std::vector<node*>* tmp = new std::vector<node*>();
  tmp->push_back($1);
  $$ = tmp;
}

dd_pair:
    expr ':' expr
{
  $$ = new node(":", {$1, $3});
  cur_dd.insert(std::make_pair($1, $3));
}

modes:
	modes mode  { ; }
	| mode      { ; }

mode:
	'{' MODE number ';' invt flow jumps_section '}'
{
  if(pdrh::get_mode(atoi($3)) == NULL)
	{
    cur_dd.clear();
    cur_mode->id = atoi($3);
    cur_mode->time = pdrh::time;
    pdrh::push_mode(*cur_mode);
    delete cur_mode;
    cur_mode = new pdrh::mode;
  }
  else
  {
    std::stringstream s;
    s << "multiple declaration of mode \"" << $3 << "\"";
    yyerror(s.str().c_str());
  }
}
  | '{' MODE number ';' flow jumps_section '}'
{
  if(pdrh::get_mode(atoi($3)) == NULL)
  {
    cur_dd.clear();
    cur_mode->id = atoi($3);
    cur_mode->time = pdrh::time;
    pdrh::push_mode(*cur_mode);
    delete cur_mode;
    cur_mode = new pdrh::mode;
  }
  else
  {
    std::stringstream s;
    s << "multiple declaration of mode \"" << $3 << "\"";
    yyerror(s.str().c_str());
  }
}
  | '{' MODE number ';' TIME ':' '[' expr ',' expr ']' ';' flow jumps_section '}'
{
  if(pdrh::get_mode(atoi($3)) == NULL)
  {
    cur_dd.clear();
    cur_mode->id = atoi($3);
    cur_mode->time = make_pair($8, $10);
    pdrh::push_mode(*cur_mode);
    delete cur_mode;
    cur_mode = new pdrh::mode;
  }
  else
  {
    std::stringstream s;
    s << "multiple declaration of mode \"" << $3 << "\"";
    yyerror(s.str().c_str());
  }
}
  | '{' MODE number ';' TIME ':' '[' expr ',' expr ']' ';'  invt flow jumps_section '}'
{
  if(pdrh::get_mode(atoi($3)) == NULL)
  {
    cur_dd.clear();
    cur_mode->id = atoi($3);
    cur_mode->time = make_pair($8, $10);
    pdrh::push_mode(*cur_mode);
    delete cur_mode;
    cur_mode = new pdrh::mode;
  }
  else
  {
    std::stringstream s;
    s << "multiple declaration of mode \"" << $3 << "\"";
    yyerror(s.str().c_str());
  }
}

invt:
	INVT ':' prop_list { ; }
	| INVT ':'

prop_list:
	prop_list prop ';'  { pdrh::push_invt(*cur_mode, $2); }
	| prop ';'          { pdrh::push_invt(*cur_mode, $1); }

props:
	props prop { $$->push_back($2); }
	| prop                  
{
  $$ = new vector<node*>;
	$$->push_back($1);
}

prop:
    expr EQ expr                { $$ = new node("=", {$1, $3}); }
    | expr GT expr              { $$ = new node(">", {$1, $3}); }
    | expr LT expr              { $$ = new node("<", {$1, $3}); }
    | expr GE expr              { $$ = new node(">=", {$1, $3}); }
    | expr LE expr              { $$ = new node("<=", {$1, $3}); }
    | expr NE expr              { $$ = new node("!=", {$1, $3}); }
    | TRUE                      { $$ = new node("(true)"); }
    | FALSE                     { $$ = new node("(false)"); }
    | '(' prop ')'              { $$ = $2; }
    | NOT prop                  { $$ = new node("not", {$2}); }
    | '(' AND props ')'         { $$ = new node("and", *($3)); }
    | '(' OR props ')'          { $$ = new node("or", *($3)); }
    | '(' XOR props ')'         { $$ = new node("xor", *($3)); }
    | '(' IMPLY prop prop ')'   { $$ = new node("=>", {$3, $4}); }

flow:
	FLOW ':' odes { ; }

odes:
	odes ode { ; }
	| ode { ; }

ode:
	D_DT '[' identifier ']' EQ expr ';'
{
  push_ode(*cur_mode, std::string($3), $6);
	free($3);
}

// ADD INTERVAL INTO THE EXPRESSION
expr:
    identifier
{
  if(define_map.find($1) != define_map.end()) $$ = define_map[$1];
  else $$ = new node($1);
}
  | number                    { $$ = new node($1); }
  | dist                      { $$ = $1; }
  | MINUS expr %prec UMINUS   { $$ = new node("-", {$2}); }
  | PLUS expr %prec UPLUS     { $$ = $2; }
  | expr MINUS expr           { $$ = new node("-", {$1, $3}); }
  | expr PLUS expr            { $$ = new node("+", {$1, $3}); }
  | expr TIMES expr           { $$ = new node("*", {$1, $3}); }
  | expr DIVIDE expr          { $$ = new node("/", {$1, $3}); }
  | expr POWER expr           { $$ = new node("^", {$1, $3}); }
  | ABS '(' expr ')'          { $$ = new node("abs", {$3}); }
  | SQRT '(' expr ')'         { $$ = new node("^", {$3, new node("0.5")}); }
  | EXP '(' expr ')'          { $$ = new node("exp", {$3}); }
  | LOGN '(' expr ')'         { $$ = new node("log", {$3}); }
  | SIN '(' expr ')'          { $$ = new node("sin", {$3}); }
  | COS '(' expr ')'          { $$ = new node("cos", {$3}); }
  | TAN '(' expr ')'          { $$ = new node("tan", {$3}); }
  | ASIN '(' expr ')'         { $$ = new node("asin", {$3}); }
  | ACOS '(' expr ')'         { $$ = new node("acos", {$3}); }
  | ATAN '(' expr ')'         { $$ = new node("atan", {$3}); }
  | '(' expr ')'              { $$ = $2; }

reset_props:
	reset_props reset_prop { ; }
	| reset_prop { ; }

reset_prop:
  reset_var EQ expr { push_reset(*cur_mode, *cur_jump, $1, $3); }
  | reset_var EQ '[' expr ',' expr ']'
{
  cur_jump->reset_nondet.insert(make_pair($1, make_pair($4, $6)));
}
  | TRUE                                  { ; }
  | FALSE                                 { ; }
  | '(' reset_prop ')'                    { ; }
  | '(' AND reset_props ')'               { ; }

reset_var:
  identifier PRIME 	
{
  if(pdrh::var_exists($1))
  {
    $$ = $1;
  }
  else
  {
    std::stringstream s;
    s << "undefined variable \"" << $1 << "\"";
    yyerror(s.str().c_str());
  }
}

reset_state:
	'@' number reset_prop ';'
{
  cur_jump->next_id = atoi($2);
	// updating resets
  // variables
  for(auto it = pdrh::var_map.begin(); it != pdrh::var_map.end(); it++)
  {
    if(cur_jump->reset.find(it->first) == cur_jump->reset.end())
    {
      cur_jump->reset.insert(make_pair(it->first, new node(it->first)));
    }
  }
  // nondeterministic parameters
  for(auto it = pdrh::par_map.begin(); it != pdrh::par_map.end(); it++)
  {
    cur_jump->reset.insert(make_pair(it->first, new node(it->first)));
  }
  // discrete random parameters
  for(auto it = pdrh::dd_map.begin(); it != pdrh::dd_map.end(); it++)
  {
    cur_jump->reset.insert(make_pair(it->first, new node(it->first)));
  }
  // continuous random parameters
  for(auto it = pdrh::rv_map.begin(); it != pdrh::rv_map.end(); it++)
  {
    cur_jump->reset.insert(make_pair(it->first, new node(it->first)));
  }
}

jumps_section:
	JUMP ':' jumps { ; }
	| JUMP ':' { ; }

jumps:
	jumps jump { ; }
	| jump { ; }

jump:
	prop TRANS reset_state
{
  cur_jump->guard = $1;
	pdrh::push_jump(*cur_mode, *cur_jump);
	delete cur_jump;
	cur_jump = new pdrh::mode::jump;
}

init:
	INIT ':' states
{
  delete cur_mode;
  delete cur_jump;
	pdrh::push_init(cur_states);
	cur_states.clear();
}

goal:
	GOAL ':' states
{
  pdrh::push_goal(cur_states);
  cur_states.clear();
}

state:
	'@' number prop ';' 
{
  if(get_mode(atoi($2)) != NULL)
  {
    state *s = new state;
    s->id = atoi($2);
    s->prop = $3;
    cur_states.push_back(*s);
    delete s;
	}
	else
	{
	  stringstream s;
    s << "mode \"" << $2 << "\" does not exist";
    yyerror(s.str().c_str());
	}
}

states:
  states state { ; }
  | state { ; }

%%

void yyerror(const char *s)
{
  cerr << "error at " << yylloc.first_line << ":" 
    << yylloc.first_column << ":" << s << "\n";
  exit(EXIT_FAILURE);
}
