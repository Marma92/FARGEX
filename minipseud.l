%option noyywrap
%{

#include "minipseud.h"
#include <stdlib.h>
#include "minipseudtree.h"
#include <string.h>

%}

empty    [ \t]+

num   [0-9]
int    {num}+
exp  [eE][+-]?{int}
var   [a-z]+
str   \"[a-zA-Z :'éàèç]+\"
varstr   _[a-z]+
real      {int}("."{int})?{exp}?


%%

{empty}  { /* Nothing */ }



"("   return(OP_PAR);
")"   return(CL_PAR);
"{"	  return(OP_CRO);
"}"   return(CL_CRO);
"#"   return(SHARP);

{var}    {
    yylval.node = createNode(NTVAR);
    yylval.node->var = (char*)_strdup(yytext);
    return(VAR);
    }
{str}    {
    yylval.node = createNode(NTSTR);
    yylval.node->var = (char*)_strdup(yytext)+1;
	yylval.node->var[strlen(yylval.node->var)-1]='\0';
    return(STR);
    }
{varstr}    {
    yylval.node = createNode(NTVARSTR);
    yylval.node->var = (char*)_strdup(yytext);
    return(VARSTR);
    }

{real}    {
    yylval.node = createNode(NTNUM);
    yylval.node->val = atof(yytext);
    return(NUM);
    }

">" 	{ yylval.node = createNode(NTGT); return(GT); }
"<" 	{ yylval.node = createNode(NTLT); return(LT); }
">=" 	{ yylval.node = createNode(NTGET); return(GET); }
"<=" 	{ yylval.node = createNode(NTLET); return(LET); }
"==" 	{ yylval.node = createNode(NTEQ); return(EQ); }
"!=" 	{ yylval.node = createNode(NTNEQ); return(NEQ); }
"&&" 	{ yylval.node = createNode(NTAND); return(AND); }
"||" 	{ yylval.node = createNode(NTOR); return(OR); }


"@"  	{ yylval.node = createNode(NTCAT); return(CAT); }
"+"  	{ yylval.node = createNode(NTPLUS); return(PLUS); }
"-"  	{ yylval.node = createNode(NTMIN); return(MIN); }
"*"   	{ yylval.node = createNode(NTMULT); return(MULT); }
"/"   	{ yylval.node = createNode(NTDIV); return(DIV); }
"^"  	{ yylval.node = createNode(NTPOW); return(POW); }
"="  	{ yylval.node = createNode(NTAFF); return(AFF); }
"IF" 	{  yylval.node = createNode(NTIF); return(IF); }
"ELSE" 	{  yylval.node = createNode(NTELSE); return(ELSE); }
"WHILE" {  yylval.node = createNode(NTWHILE); return(WHILE); }
"FOR" {  yylval.node = createNode(NTFOR); return(FOR); }
"AFFICHER" { yylval.node = createNode(NTDISP); return(DISP); }
"CLAVIER"  { yylval.node = createNode(NTCLAVIER);
    yylval.node->val = 13.37;
    return(CLAVIER); }






";"    { return(COLON); }

"\n"   { return(EOL); }
