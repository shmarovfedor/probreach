%{
#include "pdrhparser.hpp"

#define YY_DECL \
int yylex( \
  yy::parser::semantic_type *yylval, \
  yy::parser::location_type *yylloc)

#define YY_USER_ACTION \
  do { \
    yylloc->step(); \
    yylloc->columns(yyleng); \
  } while(0); 
%}

%option noyywrap

%x COMMENT

%%
"//".*\n	 		  	  { ; }
"/*"            		{ BEGIN(COMMENT); }
<COMMENT>"*/" 			{ BEGIN(INITIAL); }
<COMMENT>\n    			{ ; }
<COMMENT>.    			;

"+"                 { return yy::parser::token::PLUS; }
"-"                 { return yy::parser::token::MINUS; }
"*"                 { return yy::parser::token::TIMES; }
"/"                 { return yy::parser::token::DIVIDE; }
"^"                 { return yy::parser::token::POWER; }

"="                 { return yy::parser::token::EQ; }
">"                 { return yy::parser::token::GT; }
"<"                 { return yy::parser::token::LT; }
">="                { return yy::parser::token::GE; }
"<="                { return yy::parser::token::LE; }
"!="                { return yy::parser::token::NE; }

"'"                 { return yy::parser::token::PRIME; }

true                { return yy::parser::token::TRUE; }
false               { return yy::parser::token::FALSE; }

time                { return yy::parser::token::TIME; }

dist_uniform        { return yy::parser::token::U_DIST; }
dist_normal         { return yy::parser::token::N_DIST; }
dist_exp            { return yy::parser::token::E_DIST; }
dist_discrete       { return yy::parser::token::DD_DIST; }

mode                { return yy::parser::token::MODE; }
invt                { return yy::parser::token::INVT; }
flow                { return yy::parser::token::FLOW; }
"d/dt"              { return yy::parser::token::D_DT; }
jump                { return yy::parser::token::JUMP; }
init                { return yy::parser::token::INIT; }
goal                { return yy::parser::token::GOAL; }

not	            		{ return yy::parser::token::NOT; }
and                 { return yy::parser::token::AND; }
or                  { return yy::parser::token::OR; }
xor                 { return yy::parser::token::XOR; }
"=>"                { return yy::parser::token::IMPLY; }

abs                 { return yy::parser::token::ABS; }
sqrt              	{ return yy::parser::token::SQRT; }
exp              	  { return yy::parser::token::EXP; }
log              	  { return yy::parser::token::LOGN; }
sin              	  { return yy::parser::token::SIN; }
cos              	  { return yy::parser::token::COS; }
tan              	  { return yy::parser::token::TAN; }
asin              	{ return yy::parser::token::ASIN; }
acos              	{ return yy::parser::token::ACOS; }
atan              	{ return yy::parser::token::ATAN; }

[+-]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?	{ 
  yylval->emplace<std::string>(yytext);
  return yy::parser::token::number;
}

[a-zA-Z_][a-zA-Z0-9_]*            { 
  yylval->emplace<std::string>(yytext);
  return yy::parser::token::identifier;
}

[\[\]\(\):;,\{\}@]	{ return yytext[0]; }
"==>"	              { return yy::parser::token::TRANS; }

\n {
    yylloc->lines(1);
    yylloc->step();
}

[ \t]+				      ;

%%
