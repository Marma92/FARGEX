%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "minipseudtree.h"
#include "minipseudeval.h"

extern int  yyparse();
extern FILE *yyin;


Node root;
 

%}

%union {
	struct Node *node;
}


%token   <node> NUM VAR STR VARSTR AFF DISP
%token   <node> CLAVIER CAT PLUS MIN MULT DIV POW EQ NEQ GT LT GET LET AND OR  IF WHILE ELSE FOR
%token   OP_PAR CL_PAR COLON OP_CRO CL_CRO SHARP
%token   EOL
%nonassoc FI


%type   <node> Instlist
%type   <node> Inst
%type   <node> Expr
%type   <node> ExprSTR


%left OR
%left AND
%left EQ NEQ
%left GT LT GET LET
%left PLUS  MIN CAT
%left MULT  DIV
%left NEG NOT
%right  POW

%start Input
%%

Input:
      {/* Nothing ... */ }
  | Line Input { /* Nothing ... */ }


Line:
    EOL {  }
  | Instlist EOL { exec($1);    }
  ; 

Instlist:
    Inst { $$ = nodeChildren(createNode(NTINSTLIST), $1, createNode(NTEMPTY)); } 
  | Instlist Inst { $$ = nodeChildren(createNode(NTINSTLIST), $1, $2); }
  ;

Inst:
    Expr COLON { $$ = $1; } 
	|VAR AFF Expr COLON {$$ = nodeChildren($2, $1, $3);}
	|VARSTR AFF ExprSTR COLON {$$ = nodeChildren($2, $1, $3);}
  | IF OP_PAR Expr CL_PAR OP_CRO Instlist CL_CRO  {$$ = nodeChildren($1, $3, $6);} /* Le S/R conflit se resoud automatiquement */
  | IF OP_PAR Expr CL_PAR OP_CRO Instlist CL_CRO ELSE OP_CRO Instlist CL_CRO {$$ = nodeChildrenIF($8, $3, $6,$10 );}
  | WHILE OP_PAR Expr CL_PAR OP_CRO Instlist CL_CRO {printf("while trouve\n"); $$ = nodeChildren($1, $3, $6);}
  | FOR OP_PAR Instlist SHARP Expr SHARP Instlist CL_PAR OP_CRO Instlist CL_CRO {$$ = nodeChildrenFOR($1, $3, $5,$7,$10 );}
  | DISP OP_PAR Expr CL_PAR COLON { $$ = nodeChildren($1,$3,createNode(NTEMPTY)); }
  | DISP OP_PAR ExprSTR CL_PAR COLON { $$ = nodeChildren($1,$3,createNode(NTEMPTY)); }
  ;




Expr:
    NUM			{ $$ = $1; }
  | VAR
  | CLAVIER {$$ = nodeChildren($1, createNode(NTEMPTY), createNode(NTEMPTY));}
  | Expr PLUS Expr     { $$ = nodeChildren($2, $1, $3); }
  | Expr MIN Expr      { $$ = nodeChildren($2, $1, $3); }
  | Expr MULT Expr     { $$ = nodeChildren($2, $1, $3); }
  | Expr DIV Expr      { $$ = nodeChildren($2, $1, $3); }
  | MIN Expr %prec NEG { $$ = nodeChildren($1, createNode(NTEMPTY), $2); }
  | Expr POW Expr      { $$ = nodeChildren($2, $1, $3); }
  | OP_PAR Expr CL_PAR { $$ = $2; }
  | Expr GT Expr       { $$ = nodeChildren($2, $1, $3); }
  | Expr LT Expr       { $$ = nodeChildren($2, $1, $3); }
  | Expr GET Expr      { $$ = nodeChildren($2, $1, $3); }
  | Expr LET Expr      { $$ = nodeChildren($2, $1, $3); }
  | Expr EQ Expr       { $$ = nodeChildren($2, $1, $3); }
  | Expr NEQ Expr      { $$ = nodeChildren($2, $1, $3); }
  | Expr AND Expr      { $$ = nodeChildren($2, $1, $3); }
  | Expr OR Expr      { $$ = nodeChildren($2, $1, $3); }
  ;
  
ExprSTR:
    STR			{ $$ = $1; }
  | VARSTR
  | CLAVIER {$$ = nodeChildren($1, createNode(NTEMPTY), createNode(NTEMPTY));}
  | ExprSTR CAT ExprSTR     { $$ = nodeChildren($2, $1, $3); }
  ;


%%

 
 

int exec(Node *node) {
  //commenter pour arrêter l'impression de l'arbre
  //printGraph(node);
  eval(node);
}

 

int yyerror(char *s) {
  printf("%s\n", s);
}

 

int main(int arc, char **argv) {
   if ((arc == 3) && (strcmp(argv[1], "-f") == 0)) {
    
    FILE *fp=fopen(argv[2],"r");
    if(!fp) {
      printf("Impossible d'ouvrir le fichier à executer.\n");
      exit(0);
    }      
    yyin=fp;
    yyparse();
		  
    fclose(fp);
  }  
  exit(0);
}
