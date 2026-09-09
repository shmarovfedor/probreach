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

// to make sure that all nameespaces are resolved correctly
%code requires {
  #include "model.h"
}

%union
{
	char*                                                 str_val;
  node*                                                 node_val;
  std::vector<node*>*                                   node_list;
  std::pair<node*, node*>*                              node_node_pair;
  std::map<node*, node*>*                               node_node_map;
  model::state*                                         state_val;
  std::vector<model::state>*                            state_list;
  std::pair<std::string, node*>*                        str_node_pair;
  std::map<std::string, node*>*                         str_node_map;
  std::pair<std::string, std::map<std::string, node*>>* reset_state_val;
  model::mode::jump*                                    jump_val;
  std::vector<model::mode::jump>*                       jump_list;
  model::mode*                                          mode_val;
  std::vector<model::mode>*                             mode_list;
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

%token <str_val> identifier
%token <str_val> number

%left EQ LT GT LE GE NE
%left PLUS MINUS
%left TIMES DIVIDE
%precedence UMINUS UPLUS
%right POWER

%type<str_val> mode_declaration
%type<node_node_pair> time_section
%type<node_val> expr
%type<node_list> props
%type<node_val> prop
%type<node_node_pair> interval
%type<node_node_pair> dd_pair
%type<node_node_map> dd_pairs
%type<state_val> cond_state
%type<state_list> cond_states init goal
%type<str_node_pair> ode
%type<str_node_map> odes flow_section
%type<str_val> reset_var
%type<str_node_pair> assignment
%type<str_node_map> assignments
%type<reset_state_val> reset_state
%type<jump_val> jump
%type<jump_list> jumps jump_section
%type<node_list> invt_list invt_section
%type<mode_val> mode
%type<mode_list> modes

// declaring some variables
%{
%}

%%
pdrh:
	| declarations modes init goal 
{
  model::modes = *$2;
  model::init = *$3;
  model::goal = *$4;
  model::finalise();
}

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
  model::push_dd($5, *$3);
}

dd_pairs:
  dd_pairs ',' dd_pair        
{
  $1->insert(*$3);
  $$ = $1;
}
  | dd_pair
{
  $$ = new std::map<node*, node*>();
  $$->insert(*$1);
}

dd_pair:
    number ':' number
{
  $$ = new std::pair<node*, node*>();
  $$->first = new node($1);
  $$->second = new node($3);
}

modes:
	modes mode  
{ 
  $1->push_back(*$2);
  $$ = $1;
}
	| mode      
{
  $$ = new std::vector<model::mode>();
  $$->push_back(*$1); 
}

mode:
  '{' mode_declaration time_section invt_section flow_section jump_section '}'
{
  $$ = new model::mode();
  $$->id = $2;
  $$->time = std::make_pair($3->first, $3->second);
  $$->invts = *$4;
  $$->odes = *$5;
  $$->jumps = *$6;
}

mode_declaration:
  MODE number ';'
{
  if(model::get_mode($2) == NULL)
  {
    $$ = $2;
  }
  else
  {
    std::stringstream s;
    s << "multiple declaration of mode \"" << $2 << "\"";
    yyerror(s.str().c_str());
  }
}

time_section:
  TIME ':' interval ';' { $$ = $3; }

invt_section:
	INVT ':' invt_list { $$ = $3; }
	| INVT ':' { $$ = new std::vector<node*>(); }
  | { $$ = new std::vector<node*>(); }

invt_list:
	invt_list prop ';'  
{ 
  $1->push_back($2);
  $$ = $1;
}
	| prop ';'          
{ 
  $$ = new std::vector<node*>();
  $$->push_back($1);  
}

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

flow_section:
	FLOW ':' odes 
{
  $$ = $3; 
}

odes:
	odes ode 
{ 
  $1->insert(*$2);
  $$ = $1; 
}
	| ode 
{ 
  $$ = new std::map<std::string, node*>();
  $$->insert(*$1);
}

ode:
	D_DT '[' identifier ']' EQ expr ';'
{
  $$ = new std::pair<std::string, node*>();
  $$->first = $3;
  $$->second = $6;
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
	assignments assignment 
{ 
  $1->insert(*$2);
  $$ = $1; 
}
  | '(' AND assignments ')' 
{ 
  $$ = $3;
}
	| assignment 
{ 
  $$ = new std::map<std::string, node*>();
  $$->insert(*$1); 
}

assignment:
  reset_var EQ expr 
{ 
  $$ = new std::pair<std::string, node*>($1, $3);
}
  | '(' assignment ')'                    
{ 
  $$ = $2;
}

reset_var:
  identifier PRIME 	
{
  $$ = $1;
}

reset_state:
	'@' number assignments ';'
{
  $$ = new std::pair<std::string, std::map<std::string, node*>>($2, *$3);
}

jump_section:
	JUMP ':' jumps { $$ = $3; }
	| JUMP ':' { $$ = new std::vector<model::mode::jump>(); }
  | { $$ = new std::vector<model::mode::jump>(); }

jumps:
	jumps jump 
{ 
  $1->push_back(*$2);
  $$ = $1; 
}
	| jump 
{ 
  $$ = new std::vector<model::mode::jump>();
  $$->push_back(*$1);
}

jump:
	prop TRANS reset_state
{
  $$ = new model::mode::jump();
  $$->guard = $1;
  $$->next_id = $3->first;
  $$->reset = $3->second;
}

cond_state:
	'@' number prop ';' 
{
  $$ = new model::state();
  $$->id = $2;
  $$->prop = $3;
}

cond_states:
  cond_states cond_state 
{ 
  $1->push_back(*$2);
  $$ = $1;
 }
  | cond_state 
{ 
  $$ = new std::vector<model::state>();
  $$->push_back(*$1); 
}

init:
	INIT ':' cond_states
{
  //model::init = *$3;
  $$ = $3;
}

goal:
	GOAL ':' cond_states
{
  //model::goal = *$3;
  $$ = $3;
}


%%

void yyerror(const char *s)
{
  std::cerr << "error at " << yylloc.first_line << ":" 
    << (yylloc.first_column + 1) << ":" << s << "\n";
  exit(EXIT_FAILURE);
}
