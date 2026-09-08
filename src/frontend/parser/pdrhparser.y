%{
#include <iostream>
#include <sstream>
#include "node.h"
#include "model.h"

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

void yyerror(const char *s);

%}
%locations

%union
{
	char*                     sval;
  node*                     node_val;
  std::vector<node*>*       node_val_list;
  std::pair<node*, node*>*  node_val_pair;
}

// terminals
%token TIME
%token N_DIST U_DIST E_DIST DD_DIST
%token INFTY

%token MODE INVT FLOW JUMP INIT GOAL 
%token D_DT TRANS PRIME

%token SQRT EXP LOGN SIN COS TAN ASIN ACOS ATAN ABS
%token NOT AND OR XOR IMPLY
%token PLUS MINUS TIMES DIVIDE POWER
%token EQ GT LT GE LE NE
%token TRUE FALSE

%token <sval> identifier
%token <sval> number

%left EQ LT GT LE GE NE
%left PLUS MINUS
%left TIMES DIVIDE
%precedence UMINUS UPLUS
%right POWER

%type<sval> reset_var
%type<node_val_list> props dd_pairs
%type<node_val> prop expr dd_pair
%type<node_val_pair> interval

// declaring some variables
%{
model::mode *cur_mode = new model::mode;
model::mode::jump *cur_jump = new model::mode::jump;
std::vector<model::state> cur_states;
std::vector<model::mode*> cur_path;
std::map<node*, node*> cur_dd;
%}

%%
pdrh:
	| declarations modes init goal { ; }

declarations:
	declarations declaration { ; }
	| declaration { ; }

declaration:
	var_declaration { ; }
	| dist_declaration { ; }
	| const_declaration { ; }

const_declaration:
  '[' number ']' identifier ';' 
{
  model::push_var($4, new node($2), new node($2));
}

interval:
  '[' number ',' number ']'
{
  $$ = new std::pair<node*, node*>();
  $$->first = new node($2);
  $$->second = new node($4);
}

var_declaration:
	interval identifier ';'
{
  model::push_var($2, $1->first, $1->second);
}

dist_declaration:
  N_DIST '(' number ',' number ')' identifier ';'
{
  model::distribution::push_normal($7, new node($3), new node($5));
}
  | U_DIST '(' number ',' number ')' identifier ';'
{
  model::distribution::push_uniform($7, new node($3), new node($5));
}
  | E_DIST '(' number ')' identifier ';'
{
  model::distribution::push_exp($5, new node($3));
}
  | DD_DIST '(' dd_pairs ')' identifier ';'
{
  model::push_dd($5, cur_dd);
  cur_dd.clear();
}

dd_pairs:
    dd_pairs ',' dd_pair        
{
  $1->push_back($3);
  $$ = $1;
}
  | dd_pair
{
  $$ = new std::vector<node*>();
  $$->push_back($1);
}

dd_pair:
    number ':' number
{
  $$ = new node(":", {new node($1), new node($3)});
  cur_dd.insert(std::make_pair(new node($1), new node($3)));
}

modes:
	modes mode  { ; }
	| mode      { ; }

mode:
  '{' MODE number ';' TIME ':' interval ';' flow jumps_section '}'
{
  if(model::get_mode(atoi($3)) == NULL)
  {
    cur_dd.clear();
    cur_mode->id = atoi($3);
    cur_mode->time = std::make_pair($7->first, $7->second);
    model::push_mode(*cur_mode);
    delete cur_mode;
    cur_mode = new model::mode;
  }
  else
  {
    std::stringstream s;
    s << "multiple declaration of mode \"" << $3 << "\"";
    yyerror(s.str().c_str());
  }
}
  | '{' MODE number ';' TIME ':' interval ';'  invt flow jumps_section '}'
{
  if(model::get_mode(atoi($3)) == NULL)
  {
    cur_dd.clear();
    cur_mode->id = atoi($3);
    cur_mode->time = std::make_pair($7->first, $7->second);
    model::push_mode(*cur_mode);
    delete cur_mode;
    cur_mode = new model::mode;
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
	prop_list prop ';'  { model::push_invt(*cur_mode, $2); }
	| prop ';'          { model::push_invt(*cur_mode, $1); }

props:
	props prop { $$->push_back($2); }
	| prop                  
{
  $$ = new std::vector<node*>;
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

expr:
  identifier                  { $$ = new node($1); }
  | number                    { $$ = new node($1); }
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

assignments:
	assignments assignment { ; }
	| assignment { ; }

assignment:
  reset_var EQ expr { push_reset(*cur_mode, *cur_jump, $1, $3); }
  | '(' assignment ')'                    { ; }
  | '(' AND assignments ')'               { ; }

reset_var:
  identifier PRIME 	
{
  $$ = $1;
}

reset_state:
	'@' number assignments ';'
{
  cur_jump->next_id = atoi($2);
	// updating resets
  // variables
  for(auto it = model::var_map.begin(); it != model::var_map.end(); it++)
  {
    if(cur_jump->reset.find(it->first) == cur_jump->reset.end())
    {
      cur_jump->reset.insert(make_pair(it->first, new node(it->first)));
    }
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
	model::push_jump(*cur_mode, *cur_jump);
	delete cur_jump;
	cur_jump = new model::mode::jump;
}

cond_state:
	'@' number prop ';' 
{
  if(model::get_mode(atoi($2)) != NULL)
  {
    model::state *s = new model::state;
    s->id = atoi($2);
    s->prop = $3;
    cur_states.push_back(*s);
    delete s;
	}
	else
	{
	  std::stringstream s;
    s << "mode \"" << $2 << "\" does not exist";
    yyerror(s.str().c_str());
	}
}

cond_states:
  cond_states cond_state { ; }
  | cond_state { ; }

init:
	INIT ':' cond_states
{
  delete cur_mode;
  delete cur_jump;
	model::push_init(cur_states);
	cur_states.clear();
}

goal:
	GOAL ':' cond_states
{
  model::push_goal(cur_states);
  cur_states.clear();
}


%%

void yyerror(const char *s)
{
  std::cerr << "error at " << yylloc.first_line << ":" 
    << (yylloc.first_column + 1) << ":" << s << "\n";
  exit(EXIT_FAILURE);
}
