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
%type<std::unique_ptr<bool_exprt>> prop
%type<std::vector<std::unique_ptr<bool_exprt>>*> props
%type<std::unique_ptr<symbolt>> mode_id
%type<std::unique_ptr<symbolt>> mode_declaration
%type<std::unique_ptr<intervalt>> interval
%type<std::unique_ptr<intervalt>> time_section
%type<std::vector<std::unique_ptr<invtt>>*> invt_section
%type<std::vector<std::unique_ptr<invtt>>*> invt_list 

%type<std::unique_ptr<symbolt>> reset_var
%type<std::unique_ptr<assignt>> assignment
%type<std::vector<std::unique_ptr<assignt>>*> assignments
%type<std::unique_ptr<reset_statet>> reset_state

%type<std::unique_ptr<cond_statet>> cond_state
%type<std::map<std::unique_ptr<symbolt>, std::unique_ptr<cond_statet>>*> cond_states
%type<std::map<std::unique_ptr<symbolt>, std::unique_ptr<cond_statet>>*> init
%type<std::map<std::unique_ptr<symbolt>, std::unique_ptr<cond_statet>>*> goal

%type<std::unique_ptr<odet>> ode
%type<std::vector<std::unique_ptr<odet>>*> odes 
%type<std::unique_ptr<flowt>> flow_section

%type<std::unique_ptr<jumpt>> jump
%type<std::map<std::unique_ptr<symbolt>, std::unique_ptr<jumpt>>*> jumps 
%type<std::map<std::unique_ptr<symbolt>, std::unique_ptr<jumpt>>*> jump_section 

%type<std::unique_ptr<modet>> mode
%type<std::map<std::unique_ptr<symbolt>, std::unique_ptr<modet>>*> modes

%type<std::pair<std::unique_ptr<numbert>, std::unique_ptr<numbert>>*> dd_pair
%type<std::map<std::unique_ptr<numbert>, std::unique_ptr<numbert>>*> dd_pairs

%type<std::unique_ptr<distt>> dist

//%type<old::declarationt*> declaration 
//%type<old::declarationt*> const_declaration 
//%type<old::declarationt*> var_declaration 
//%type<old::declarationt*> dist_declaration
//%type<std::map<std::string, old::declarationt>*> declarations

%type<std::unique_ptr<declt>> declaration 
%type<std::unique_ptr<declt>> const_declaration 
%type<std::unique_ptr<declt>> var_declaration 
%type<std::unique_ptr<declt>> dist_declaration
%type<std::map<std::unique_ptr<symbolt>, std::unique_ptr<declt>>*> declarations

// setting global extern variable here
%{
old::modelt old::global_model;
%}

%%
model:
	declarations modes init goal 
{
  old::global_model.declarations.decls = *$1;
  old::global_model.modes = new_to_old(*$2);
  old::global_model.init = new_to_old(*$3);
  old::global_model.goal = new_to_old(*$4);
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
  $$ = $1;
}

const_declaration:
  '[' number ']' identifier ';' 
{
/*
  node* decl = new node("const_decl", { new node($2) });
  $$ = new old::declarationt($4, decl);
*/
  $$ = std::make_unique<const_declt>(
    std::make_unique<symbolt>($4), std::make_unique<numbert>($2));
}

interval:
  '[' number ',' number ']'
{
  $$ = std::make_unique<intervalt>(
    std::make_unique<numbert>($2),
    std::make_unique<numbert>($4));
}

var_declaration:
	interval identifier ';'
{
/*
  auto old_interval = new_to_old(*$1);
  node* decl = new node("var_decl", {old_interval.first, old_interval.second});
  $$ = new old::declarationt($2, decl);
*/
  $$ = std::make_unique<var_declt>(
    std::make_unique<symbolt>($2), std::move($1));
}

dist_declaration:
  dist identifier ';'
{
/*
  $$ = new old::declarationt($2, new node("dist_decl", {new_to_old(*$1)}));
*/
  $$ = std::make_unique<dist_declt>(std::make_unique<symbolt>($2), std::move($1));
}

dist:
  N_DIST '(' number ',' number ')'
{
  $$ = std::make_unique<normal_distt>(
    std::make_unique<numbert>($3), std::make_unique<numbert>($5));
}
  | U_DIST '(' number ',' number ')'
{
  $$ = std::make_unique<uniform_distt>(
    std::make_unique<numbert>($3), std::make_unique<numbert>($5));
}
  | E_DIST '(' number ')'
{
  $$ = std::make_unique<exp_distt>(std::make_unique<numbert>($3));
}
  | DD_DIST '(' dd_pairs ')'
{
  $$ = std::make_unique<discrete_distt>(std::move(*$3));
}

dd_pairs:
  dd_pairs ',' dd_pair        
{
  $1->insert(std::move(*$3));
  $$ = std::move($1);
}
  | dd_pair
{
  $$ = new std::map<std::unique_ptr<numbert>, std::unique_ptr<numbert>>();
  $$->insert(std::move(*$1));
}

dd_pair:
    number ':' number
{
  $$ = new std::pair<std::unique_ptr<numbert>, std::unique_ptr<numbert>>(
    std::make_unique<numbert>($1), std::make_unique<numbert>($3));
}

modes:
	modes mode  
{
  auto mode_id = std::make_unique<symbolt>($2->get_mode_id().get_value());
  $1->emplace(std::move(mode_id), std::move($2));
  $$ = std::move($1);
}
	| mode      
{
  $$ = new std::map<std::unique_ptr<symbolt>, std::unique_ptr<modet>>();
  auto mode_id = std::make_unique<symbolt>($1->get_mode_id().get_value());
  $$->emplace(std::move(mode_id), std::move($1));
}

mode:
  '{' mode_declaration time_section invt_section flow_section jump_section '}'
{
  $$ = std::make_unique<modet>(
    std::move($2), std::move($3), std::move(*$4), std::move($5), std::move(*$6));
}

mode_id:
 number
{
  $$ = std::make_unique<symbolt>($1);
}
  | identifier
{
  $$ = std::make_unique<symbolt>($1);
}

mode_declaration:
  MODE mode_id ';'
{
  $$ = std::move($2);
}

time_section:
  TIME ':' interval ';' 
{ 
  $$ = std::move($3);
}

invt_section:
	INVT ':' invt_list 
{ 
  $$ = std::move($3);
}
	| INVT ':' 
{ 
  $$ = new std::vector<std::unique_ptr<invtt>>(); 
}
  | 
{ 
  $$ = new std::vector<std::unique_ptr<invtt>>(); 
}

invt_list:
	invt_list prop ';'  
{ 
  $1->push_back(std::make_unique<invtt>(std::move($2)));
  $$ = std::move($1);
}
	| prop ';'          
{ 
  $$ = new std::vector<std::unique_ptr<invtt>>();
  $$->push_back(std::make_unique<invtt>(std::move($1)));  
}

props:
	props prop 
{
  $1->push_back(std::move($2));
  $$ = $1;
}
	| prop                  
{
  $$ = new std::vector<std::unique_ptr<bool_exprt>>();
	$$->push_back(std::move($1));
}

prop:
    expr EQ expr                
{
  $$ = std::make_unique<equalt>(std::move($1), std::move($3)); 
}
    | expr GT expr              
{ 
  $$ = std::make_unique<greater_thant>(std::move($1), std::move($3)); 
}
    | expr LT expr              
{ 
  $$ = std::make_unique<less_thant>(std::move($1), std::move($3)); 
}
    | expr GE expr              
{ 
  $$ = std::make_unique<greater_equalt>(std::move($1), std::move($3)); 
}
    | expr LE expr              
{ 
  $$ = std::make_unique<less_equalt>(std::move($1), std::move($3)); 
}
    | expr NE expr              
{ 
  $$ = std::make_unique<not_equalt>(std::move($1), std::move($3)); 
}
    | TRUE                      
{ 
  $$ = std::make_unique<truet>(); 
}
    | FALSE                     
{ 
  $$ = std::make_unique<falset>(); 
}
    | '(' prop ')'              
{ 
  $$ = std::move($2);
}
    | NOT prop                  
{ 
  $$ = std::make_unique<nott>(std::move($2)); 
}
    | '(' IMPLY prop prop ')'   
{
  $$ = std::make_unique<implyt>(std::move($3), std::move($4));
}
    | '(' AND props ')'         
{
  $$ = std::make_unique<andt>(std::move(*$3));
  delete $3;
}
    | '(' OR props ')'          
{ 
  $$ = std::make_unique<ort>(std::move(*$3));
  delete $3;
}
    | '(' XOR props ')'         
{ 
  $$ = std::make_unique<xort>(std::move(*$3));
  delete $3;
}

flow_section:
	FLOW ':' odes 
{
  $$ = std::make_unique<flowt>(std::move(*$3)); 
}

odes:
	odes ode 
{
  $1->push_back(std::move($2));
  $$ = std::move($1);
}
	| ode 
{
  $$ = new std::vector<std::unique_ptr<odet>>();
  $$->push_back(std::move($1));
}

ode:
	D_DT '[' identifier ']' EQ expr ';'
{
  $$ = std::make_unique<odet>(std::make_unique<symbolt>($3), std::move($6));
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
  $1->push_back(std::move($2));
  $$ = std::move($1);
}
  | '(' AND assignments ')' 
{ 
  $$ = std::move($3);
}
	| assignment 
{ 
  $$ = new std::vector<std::unique_ptr<assignt>>();
  $$->push_back(std::move($1)); 
}

assignment:
  reset_var EQ expr 
{
  $$ = std::make_unique<assignt>(std::move($1), std::move($3));
}
  | '(' assignment ')'                    
{ 
  $$ = std::move($2);
}

reset_var:
  identifier PRIME 	
{
  $$ = std::make_unique<symbolt>($1);
}

reset_state:
	'@' mode_id assignments ';'
{
  $$ = std::make_unique<reset_statet>(std::move($2), std::move(*$3));
}

jump_section:
	JUMP ':' jumps 
{ 
  $$ = std::move($3); 
}
	| JUMP ':' 
{ 
  $$ = new std::map<std::unique_ptr<symbolt>, std::unique_ptr<jumpt>>();
}
  | 
{ 
  $$ = new std::map<std::unique_ptr<symbolt>, std::unique_ptr<jumpt>>();
}

jumps:
	jumps jump 
{
  auto mode_id = 
    std::make_unique<symbolt>($2->get_reset().get_mode_id().get_value());
  $1->emplace(std::move(mode_id), std::move($2));
  $$ = std::move($1);
}
	| jump 
{
  $$ = new std::map<std::unique_ptr<symbolt>, std::unique_ptr<jumpt>>();
  auto mode_id = 
    std::make_unique<symbolt>($1->get_reset().get_mode_id().get_value());
  $$->emplace(std::move(mode_id), std::move($1));
}

jump:
	prop TRANS reset_state
{
  $$ = std::make_unique<jumpt>(std::move($1), std::move($3));
}

cond_state:
	'@' mode_id prop ';' 
{
  $$ = std::make_unique<cond_statet>(std::move($2), std::move($3));
}

cond_states:
  cond_states cond_state 
{ 
  $1->emplace(std::make_unique<symbolt>($2->get_mode_id().get_value()), std::move($2));
  $$ = $1;
 }
  | cond_state 
{ 
  $$ = new std::map<std::unique_ptr<symbolt>, std::unique_ptr<cond_statet>>();
  $$->emplace(std::make_unique<symbolt>($1->get_mode_id().get_value()), std::move($1));
}

init:
	INIT ':' cond_states
{
  $$ = std::move($3);
}

goal:
	GOAL ':' cond_states
{
  $$ = std::move($3);
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
