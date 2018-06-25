#ifndef MINIPSEUDTREE
# define MINIPSEUDTREE
    
enum NodeType {
	NTEMPTY = 0,
	NTINSTLIST = 1,
	
	 

	NTNUM   = 201,
	NTSTR   = 202,
	NTVARSTR   = 203,
	NTVAR   = 222,
	 
	NTCAT  = 320,
	NTPLUS  = 321,
	NTMIN   = 322,
	NTMULT  = 323,
	NTDIV   = 324,
	NTPOW   = 325,
	NTEQ    = 326,
	NTAFF   = 32,
	NTDISP  = 401,

	NTGT    = 327,
	NTLT    = 328,
	NTGET   = 329,
	NTLET   = 330,
	NTNEQ   = 331,
	NTAND = 403,
	NTOR  = 404,

	NTIF 	= 332,
	NTWHILE = 333,
	NTFOR = 335,
	NTELSE	= 334,
	
	NTCLAVIER = 402

};
   
typedef struct Node {
	enum NodeType type;
	union { 
		double val;
		char* var;
		struct Node** children;
	} ;
} Node;

Node* createNode(int type);

Node* nodeChildren(Node* father, Node *child1, Node *child2);
//Rajout d'une fonction pour pouvoir recupérer toutes les branches necessaires au IF (la condition, l'instruction si cond vrai et instr si cond false)
Node* nodeChildrenIF(Node* father, Node *child1, Node *child2, Node *child3);
Node* nodeChildrenFOR(Node* father, Node *child1, Node *child2, Node *child3, Node *child4);
const char* node2String(Node *node);

void printGraph(Node *root);

typedef struct Variable {
	char * name;
	double val;
	struct Variable * next;
} Variable;

Variable * varcreate(char * name, double val);
void varappend(Variable ** liste, char * name, double val);
Variable * varsearch(Variable ** liste, char * name);
void printVariables(Variable * liste);

typedef struct VariableSTR {
	char * name;
	char * val;
	struct VariableSTR * next;
} VariableSTR;

VariableSTR * varstrcreate(char * name, char * val);
void varstrappend(VariableSTR ** liste, char * name, char * val);
VariableSTR * varstrsearch(VariableSTR ** liste, char * name);
void printVariablesStr(VariableSTR * liste);

#endif
