%{
#include <iostream>
#include <sstream>
#include "node.h"
#include "model.h"
#include "irep.h"
#include "new_to_old.h"

extern FILE *yyin;

%}
%locations

%code provides {
int yylex(
  yy::parser::semantic_type *yylval,
  yy::parser::location_type *yylloc);
}

// to make sure that all nameespaces are resolved correctly
%code requires {
  #include "model.h"
  #include "irep.h"
}

%skeleton "lalr1.cc"
%define api.value.type variant

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

%token <std::string> identifier
%token <std::string> number

%left EQ LT GT LE GE NE
%left PLUS MINUS
%left TIMES DIVIDE
%precedence UMINUS UPLUS
%right POWER

%type<std::unique_ptr<real_exprt>> expr

%type<std::string> mode_declaration
%type<std::pair<node*, node*>*> time_section
%type<std::vector<node*>*> props
%type<node*> prop
%type<std::pair<node*, node*>*> interval
%type<std::pair<node*, node*>*> dd_pair
%type<std::map<node*, node*>*> dd_pairs
%type<old::statet*> cond_state
%type<std::vector<old::statet>*> cond_states init goal
%type<std::pair<std::string, node*>*> ode
%type<std::map<std::string, node*>*> odes flow_section
%type<std::string> reset_var
%type<std::pair<std::string, node*>*> assignment
%type<std::map<std::string, node*>*> assignments
%type<std::pair<std::string, std::map<std::string, node*>>*> reset_state
%type<old::jumpt*> jump
%type<std::vector<old::jumpt>*> jumps jump_section
%type<std::vector<node*>*> invt_list invt_section
%type<old::modet*> mode
%type<std::vector<old::modet>*> modes
%type<old::declarationt*> declaration const_declaration var_declaration dist_declaration
%type<std::map<std::string, old::declarationt>*> declarations
%type<node*> dist

// setting global extern variable here
%{
old::modelt old::global_model;
%}

%%
model:
	declarations modes init goal 
{
  old::global_model.declarations.decls = *$1;
  old::global_model.modes = *$2;
  old::global_model.init = *$3;
  old::global_model.goal = *$4;
  old::global_model.finalise();
}

declarations:
	declarations declaration 
{
  $1->insert(make_pair($2->sym, *$2));
  $$ = $1;
}
	| declaration 
{
  $$ = new std::map<std::string, old::declarationt>();
  $$->insert(make_pair($1->sym, *$1));
}

declaration:
	var_declaration 
{ 
  $$ = $1;
}
	| dist_declaration 
{
  $$ = $1;
}
	| const_declaration 
{ 
  std::cout << "Const declaration 2\n";
  $$ = $1;
}

const_declaration:
  '[' number ']' identifier ';' 
{
  node* decl = new node("const_decl", { new node($2) });
  $$ = new old::declarationt($4, decl);
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
  node* decl = new node("var_decl", {$1->first, $1->second});
  $$ = new old::declarationt($2, decl);
}

dist_declaration:
  dist identifier ';'
{
  $$ = new old::declarationt($2, new node("dist_decl", {$1}));
}

dist:
  N_DIST '(' number ',' number ')'
{
  $$ = new node("dist_normal", {new node($3), new node($5)});
}
  | U_DIST '(' number ',' number ')'
{
  $$ = new node("dist_uniform", {new node($3), new node($5)});
}
  | E_DIST '(' number ')'
{
  $$ = new node("dist_exp", {new node($3)});
}
  | DD_DIST '(' dd_pairs ')'
{
  node* params = new node();
  for (auto it = $3->cbegin(); it != $3->cend(); ++it)
  {
    node* dd_pair = new node(":", {it->first, it->second});
    params->operands.push_back(dd_pair);
  }
  $$ = new node("dist_discrete", {params});
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
  $$ = new std::vector<old::modet>();
  $$->push_back(*$1); 
}

mode:
  '{' mode_declaration time_section invt_section flow_section jump_section '}'
{
  $$ = new old::modet();
  $$->id = $2;
  $$->time = std::make_pair($3->first, $3->second);
  $$->invts = *$4;
  $$->odes = *$5;
  $$->jumps = *$6;
}

mode_declaration:
  MODE number ';'
{
  $$ = $2;
}
  | MODE identifier ';'
{
  $$ = $2;
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
	props prop 
{
  $1->push_back($2);
  $$ = $1;
}
	| prop                  
{
  $$ = new std::vector<node*>();
	$$->push_back($1);
}

prop:
    expr EQ expr                
{ 
  $$ = new node("=", {new_to_old(*$1), new_to_old(*$3)});
}
    | expr GT expr              
{ 
  $$ = new node(">", {new_to_old(*$1), new_to_old(*$3)}); 
}
    | expr LT expr              
{ 
  $$ = new node("<", {new_to_old(*$1), new_to_old(*$3)}); 
}
    | expr GE expr              
{ 
  $$ = new node(">=", {new_to_old(*$1), new_to_old(*$3)}); 
}
    | expr LE expr              
{ 
  $$ = new node("<=", {new_to_old(*$1), new_to_old(*$3)}); 
}
    | expr NE expr              
{ 
  $$ = new node("!=", {new_to_old(*$1), new_to_old(*$3)}); 
}
    | TRUE                      { $$ = new node("(true)"); }
    | FALSE                     { $$ = new node("(false)"); }
    | '(' prop ')'              { $$ = $2; }
    | NOT prop                  { $$ = new node("not", {$2}); }
    | '(' AND props ')'         
{
  $$ = new node("and", *($3)); 
}
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
  $$->second = new_to_old(*$6);
}

expr:
  identifier                  
{
  $$ = std::make_unique<symbolt>($1); 
}
  | number                    
{
  $$ = std::make_unique<numbert>($1); 
}
  | MINUS expr %prec UMINUS   
{ 
  $$ = std::make_unique<minust>(std::move($2));
}
  | PLUS expr %prec UPLUS     
{ 
  $$ = std::make_unique<plust>(std::move($2));
}
  | expr MINUS expr           
{ 
  $$ = std::make_unique<subt>(std::move($1), std::move($3));
}
  | expr PLUS expr            
{ 
  $$ = std::make_unique<addt>(std::move($1), std::move($3));
}
  | expr TIMES expr           
{ 
  $$ = std::make_unique<mult>(std::move($1), std::move($3));
}
  | expr DIVIDE expr          
{ 
  $$ = std::make_unique<divt>(std::move($1), std::move($3));
}
  | expr POWER expr           
{ 
  $$ = std::make_unique<powt>(std::move($1), std::move($3));
}
  | ABS '(' expr ')'          
{ 
  $$ = std::make_unique<abst>(std::move($3));
}
  | SQRT '(' expr ')'         
{ 
  $$ = std::make_unique<sqrtt>(std::move($3));
}
  | EXP '(' expr ')'          
{ 
  $$ = std::make_unique<expt>(std::move($3));
}
  | LOGN '(' expr ')'         
{ 
  $$ = std::make_unique<logt>(std::move($3));
}
  | SIN '(' expr ')'          
{ 
  $$ = std::make_unique<sint>(std::move($3));
}
  | COS '(' expr ')'          
{ 
  $$ = std::make_unique<cost>(std::move($3));
}
  | TAN '(' expr ')'          
{ 
  $$ = std::make_unique<tant>(std::move($3));
}
  | ASIN '(' expr ')'         
{ 
  $$ = std::make_unique<asint>(std::move($3));
}
  | ACOS '(' expr ')'         
{ 
  $$ = std::make_unique<acost>(std::move($3));
}
  | ATAN '(' expr ')'         
{ 
  $$ = std::make_unique<atant>(std::move($3));
}
  | '(' expr ')'              
{ 
  $$ = std::move($2); 
}

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
  $$ = new std::pair<std::string, node*>($1, new_to_old(*$3));
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
	| JUMP ':' { $$ = new std::vector<old::jumpt>(); }
  | { $$ = new std::vector<old::jumpt>(); }

jumps:
	jumps jump 
{ 
  $1->push_back(*$2);
  $$ = $1; 
}
	| jump 
{ 
  $$ = new std::vector<old::jumpt>();
  $$->push_back(*$1);
}

jump:
	prop TRANS reset_state
{
  $$ = new old::jumpt();
  $$->guard = $1;
  $$->next_id = $3->first;
  $$->reset = $3->second;
}

cond_state:
	'@' number prop ';' 
{
  $$ = new old::statet();
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
  $$ = new std::vector<old::statet>();
  $$->push_back(*$1); 
}

init:
	INIT ':' cond_states
{
  $$ = $3;
}

goal:
	GOAL ':' cond_states
{
  $$ = $3;
}

%%

void yy::parser::error(
    const location_type& loc,
    const std::string& msg)
{
    std::cerr
        << "error at "
        << loc.begin.line
        << ":"
        << loc.begin.column
        << ": "
        << msg
        << '\n';
}
