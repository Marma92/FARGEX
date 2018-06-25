#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "minipseudtree.h"

# define PRINTTAB 2

Node* createNode(int type) {
	Node* newnode = (Node *) malloc(sizeof(Node));
	newnode->type = type;
	newnode->children = NULL;
	return newnode;
}

Node* nodeChildren(Node* father, Node *child1, Node *child2) { 
	father->children = (Node **) malloc(sizeof(Node*) * 2);
	father->children[0] = child1;
	father->children[1] = child2;
	return father;
}

Node* nodeChildrenIF(Node* father, Node *child1, Node *child2, Node *child3) { 
	father->children = (Node **) malloc(sizeof(Node*) * 3);
	father->children[0] = child1;
	father->children[1] = child2;
	father->children[2] = child3;
	return father;
}

Node* nodeChildrenFOR(Node* father, Node *child1, Node *child2, Node *child3, Node *child4) { 
	father->children = (Node **) malloc(sizeof(Node*) * 3);
	father->children[0] = child1;
	father->children[1] = child2;
	father->children[2] = child3;
	father->children[3] = child4;
	return father;
}

const char* node2String(Node *node) {	
	char *res;
	switch ( node->type ) {
	case NTEMPTY:    return "NTEMPTY";
	case NTINSTLIST: return "NTINSTLIST";
    case NTAFF: return "NTAFF";
	case NTDISP: return "NTDISP";
	case NTFOR: return "NTFOR";
	case NTCLAVIER : return "NTCLAVIER";
	 
case NTVAR :
res = (char *)malloc(sizeof(char) * 32);
		sprintf(res, "NTVAR -> %s", node->var);
		return res; 
	case NTNUM:
		res = (char *)malloc(sizeof(char) * 32);
		sprintf(res, "NTNUM -> %f", node->val);
		return res;
	case NTSTR:
		res = (char *)malloc(sizeof(char) * 32);
		sprintf(res, "NTSTR -> %s", node->var);
		return res;
	case NTVARSTR:
		res = (char *)malloc(sizeof(char) * 32);
		sprintf(res, "NTVARSTR -> \"%s\"", node->var);
		return res;
	 

	case NTPLUS:  return "NTPLUS";
	case NTCAT:  return "NTCAT";
	case NTMIN:   return "NTMIN";
	case NTMULT:  return "NTMULT";
	case NTDIV:   return "NTDIV";
	case NTPOW:   return "NTPOW";
	case NTIF:	  return "NTIF";
	case NTELSE:	  return "NTELSE";
	case NTWHILE: return "NTWHILE";
// Return pour comparer
	case NTGT:   return "NTGT";
	case NTLT:   return "NTLT";
	case NTGET:   return "NTGET";
	case NTLET:   return "NTLET";
	case NTEQ:   return "NTEQ";
	case NTNEQ:   return "NTNEQ";
	case NTAND:   return "NTAND";
	case NTOR:   return "NTOR";


	
	default: printf("unk nodetype : %d\n",node->type);return "UNK";
	};
}

const char* makeSpaces(int depth, int fdepth) {
	int nbspaces = depth * PRINTTAB;	
	char *spaces = (char *)malloc(sizeof(char) * nbspaces);
	if (depth == fdepth)		
		memset(spaces, ' ', nbspaces);
	else {
		int midspaces = fdepth * PRINTTAB;
		int endline = (depth - fdepth) * PRINTTAB - 1;
		memset(spaces, ' ', midspaces);
		spaces[midspaces] = '\\';
		char *tmpline =  (char *)malloc(sizeof(char) * endline);
		memset(tmpline, '_', endline);
		strcat(spaces, tmpline);
		free(tmpline);
	}
	return spaces;
}

void printGraphRecu(Node *node, int n) {
	
	int i;
	for ( i=0;i<n;i++)printf(" ");
	printf("%s\n", node2String(node));
	
	// Hack : No children only if null or number 
	if ((node->children != NULL) && 
		(node->type != NTNUM)  && 
		(node->type != NTVAR)  ) {
		printGraphRecu(node->children[0], n+1);
		printGraphRecu(node->children[1], n+1);
		//Si on est dans un IF alors on affiche la deuxieme possibilité en fonction du resultat de la condition
		if(node->type == NTELSE && node->children[2]!=NULL)
			printGraphRecu(node->children[2], n+1);
	}
}
void printGraphRec(Node *node, int depth, int fdepth) {
	printf("%s%s\n", makeSpaces(depth, fdepth), node2String(node));
	
	// Hack : No children only if null or number 
	if ((node->children != NULL) && 
		(node->type != NTNUM)  ) {
		printGraphRec(node->children[0], depth + 1, depth);
		printGraphRec(node->children[1], depth + 1, depth);
	}
}

void printGraph(Node * root) {
	printGraphRecu(root, 0);
}

Variable * varcreate(char * name, double val)
{
    Variable * temp=malloc(sizeof(Variable));
    temp->name=name;
	temp->val=val;
    temp->next=NULL;
    return temp;
}
void varappend(Variable ** liste, char * name, double val){
	while(*liste)
    {
        liste=&(*liste)->next;
    }
    *liste=varcreate(name,val);
}

Variable * varsearch(Variable ** liste, char * name)
{
    while(*liste)
	{
		if(strcmp((*liste)->name,name)==0)
        {
            return (*liste);
        }
        else{
            liste=&(*liste)->next;
        }
    }
	return NULL;
}
void printVariables(Variable * liste){
    if(!liste)
        return;
    Variable * temp;
    temp=liste;
    while(temp->next!=NULL)
    {
        printf("[%s,%f] ",temp->name,temp->val);
        temp=temp->next;
    }
    printf("[%s,%f] \n",temp->name,temp->val);
}
VariableSTR * varstrcreate(char * name, char * val)
{
    VariableSTR * temp=malloc(sizeof(Variable));
    temp->name=name;
	temp->val=val;
    temp->next=NULL;
    return temp;
}
void varstrappend(VariableSTR ** liste, char * name, char * val){
	while(*liste)
    {
        liste=&(*liste)->next;
    }
    *liste=varstrcreate(name,val);
}

VariableSTR * varstrsearch(VariableSTR ** liste, char * name)
{
    while(*liste)
	{
		if(strcmp((*liste)->name,name)==0)
        {
            return (*liste);
        }
        else{
            liste=&(*liste)->next;
        }
    }
	return NULL;
}
void printVariablesStr(VariableSTR * liste){
    if(!liste)
        return;
    VariableSTR * temp;
    temp=liste;
    while(temp->next!=NULL)
    {
        printf("[%s,%s] ",temp->name,temp->val);
        temp=temp->next;
    }
    printf("[%s,%s] \n",temp->name,temp->val);
}
