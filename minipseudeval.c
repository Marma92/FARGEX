#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <setjmp.h>
#include "minipseudtree.h"




 Variable * tabVar=NULL;
 VariableSTR * tabVarStr=NULL;



int printDepth = 0;
int funcDepth = 0;
char * rang=NULL;
double valeur=0.0;
char * chaine=NULL;

double evalExpr(Node *node) {
	double input;
	switch ( node->type ) {
	case NTEMPTY:  return 0.0;
	case NTNUM: return node->val;
	case NTVAR : return varsearch(&tabVar,node->var)->val;
	case NTCLAVIER: 
	scanf("%lf",&(node->val)); return node->val;
	 
	case NTPLUS: return evalExpr(node->children[0])
			+ evalExpr(node->children[1]);
	case NTMIN: return evalExpr(node->children[0])
			- evalExpr(node->children[1]);
	case NTMULT: return evalExpr(node->children[0])
			* evalExpr(node->children[1]);
	case NTDIV: return evalExpr(node->children[0])
			/ evalExpr(node->children[1]);
	case NTPOW: return pow(evalExpr(node->children[0]),
			evalExpr(node->children[1]));

	case NTGT: return evalExpr(node->children[0])
			> evalExpr(node->children[1]);

	case NTLT: return evalExpr(node->children[0])
			< evalExpr(node->children[1]);

	case NTGET: return evalExpr(node->children[0])
			>= evalExpr(node->children[1]);

	case NTLET: return evalExpr(node->children[0])
			<= evalExpr(node->children[1]);

	case NTEQ: return evalExpr(node->children[0])
			== evalExpr(node->children[1]);

	case NTNEQ: return evalExpr(node->children[0])
			!= evalExpr(node->children[1]);
	case NTAND: return evalExpr(node->children[0])
			&& evalExpr(node->children[1]);
	case NTOR: return evalExpr(node->children[0])
			|| evalExpr(node->children[1]);

	default: 
		//printf("Error in evalExpr ... Wrong node type: %s\n", node2String(node));
		exit(1);
	};
}

char * evalExprStr(Node *node) {
	double input;
	char * chaine;
	switch ( node->type ) {
	case NTEMPTY:  return "";
	case NTSTR: return node->var;
	case NTVARSTR : return varstrsearch(&tabVarStr,node->var)->val;
	
	case NTCAT: 
	chaine=malloc(sizeof(char)*150);
	strcpy(chaine,"");
	strcat(chaine,evalExprStr(node->children[0]));
	strcat(chaine,evalExprStr(node->children[1]));
	return chaine;
	case NTCLAVIER: 
	//printf("avant scanf %f\n",node->val);scanf("%lf",&(node->val));printf("scanf passe %f\n",node->val); return node->val;
	chaine=malloc(sizeof(char)*32);
    fgets(chaine, sizeof(char)*32, stdin);
    char *p = strchr(chaine, '\n');

    if (p)
    {
        *p = 0;
    }

	return chaine;

	default: 
		//printf("Error in evalExpr ... Wrong node type: %s\n", node2String(node));
		exit(1);
	};
}


void evalInst(Node* node) {
	double val;
	switch ( node->type ) {
	case NTEMPTY: return;
	case NTINSTLIST:	
		evalInst(node->children[0]);
		evalInst(node->children[1]);
		return;
	
	case NTDISP :
	if(node->children[0]->type==NTSTR||node->children[0]->type==NTVARSTR||node->children[0]->type==NTCAT){
		printf("%s\n",evalExprStr(node->children[0]));
	}
	else{
		printf("%f\n",evalExpr(node->children[0]));
	}
	return;
	
	case NTAFF :
	 
	 if(node->children[0]->type==NTVAR){//Si affectation de reel
		rang=(node->children[0]->var);
		valeur = 	evalExpr(node->children[1]);
		//printf("%s vaut %f\n",rang, valeur);
		if(varsearch(&tabVar,rang)!=NULL)//Si la variable existe deja on remplace la valeur
		{
			varsearch(&tabVar,rang)->val = valeur;
		}
		else//Si elle n'existe pas on l'ajoute a la liste de variables
		{
			varappend(&tabVar,rang,valeur);
		}
	}
	if(node->children[0]->type==NTVARSTR){//Si affection de chaine
		rang=(node->children[0]->var);
		chaine=malloc(sizeof(char)*150);
		strcpy(chaine,evalExprStr(node->children[1]));
		//printf("%s vaut %s\n",rang, chaine);
		if(varstrsearch(&tabVarStr,rang)!=NULL)//Si la variable existe deja on remplace la chaine
		{
			varstrsearch(&tabVarStr,rang)->val = chaine;
		}
		else//Si elle n'existe pas on l'ajoute a la liste de variables
		{
			varstrappend(&tabVarStr,rang,chaine);
		}
	}
	return;
		
	case NTNUM:
	case NTPLUS:
	case NTMIN:
	case NTMULT:
	case NTDIV:
	case NTPOW:
		printf("%f\n", evalExpr(node));
		return;
	case NTWHILE:
		while(evalExpr(node->children[0])==1){
			evalInst(node->children[1]);
		}
	return;
	case NTFOR:
		evalInst(node->children[0]);//instruction de debut
		while(evalExpr(node->children[1])==1){
			evalInst(node->children[3]);
			evalInst(node->children[2]);//Instruction de fin
		}
		
	return;
	case NTIF:
		if(evalExpr(node->children[0])==1){
			evalInst(node->children[1]);
			return;
		}
	return;

	case NTELSE:

		if(evalExpr(node->children[0])==1){
			evalInst(node->children[1]);
			return;
		}else if(node->children[2]!=NULL)
		{
			evalInst(node->children[2]);
			return;
		}
	return;
	
	default:
		printf("Error in evalInst ... Wrong node type: %s\n", node2String(node));
		exit (1);
	};
}

void eval(Node *node) {
	 
	evalInst(node);
}