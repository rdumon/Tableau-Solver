#include <stdio.h> 
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h> 
#include <stdbool.h> /* malloc, free, rand */


//------------GLOBALS-------------
int Fsize=50;
int cases=10;
char binCon[3]=">^v";
int i;
int j;

/*typedef struct tableau tableau;*/
typedef struct tableau {
  char *root;
  struct  tableau *left;
  struct tableau *right;
  struct tableau *parent;
} node;


//-------------FUNCTIONS----------
int bin(char *);
int parse(char *);
void complete(struct tableau *t);
char *partone(char *g);
char *parttwo(char *g);
char *add_negation(char *g);
void add_node_left(struct tableau *t, struct tableau *node);
void add_node_rigth(struct tableau *t, struct tableau *node);
char *remove_negation(char *g);
int evaluateFormula(char *g);
int negativePropCheck(char *g);
bool closed(struct tableau *t);
bool goUp(struct tableau *t, char *g);


int parse(char *g)
// which returns 1 if a proposition, 2 if neg, 3 if binary, ow 0
{

char *partA=malloc(Fsize); 
char *partB=malloc(Fsize);
int binaryConnective;

 switch(g[0]) {

    case 'p':
      if(g[1] != '\0' ) return 0;
      else return 1;
      break;

    case 'q':
      if(g[1] != '\0' ) return 0;
      else return 1;
      break;

    case 'r':
      if(g[1] != '\0' ) return 0;
      else return 1;
      break;

    case '-':
      return 2;
      break;


    case '(': 
      binaryConnective = bin(g);
      for(int b=1; b<binaryConnective; b++) {
        partA[b-1] = g[b];
      }
      for (int b=binaryConnective; b < strlen(g)-2; b++) {
        partB[b-binaryConnective]=g[b+1];
      }

      if (parse(partA)!=0 && parse(partB)!=0) { 
              return 3; 
      }
      free(partA);
      free(partB);
      return 0;
      break;

    default:
      return 0;
      break;

  }

}


int bin(char *g)
//Given a formula (A*B) returns character "*" 's position
{
int pos;

  for(int mistake=0; mistake <= 50; mistake++) {
    for (int b = 0; b < 3; b++) {
      if (g[mistake] == binCon[b]) {
        pos = mistake;
        int rightBrackets=0, leftBrackets=0, sum = 0;
        for (int count=0; count<pos; count++) {
          if (g[count]== '(') {
            rightBrackets++;
          } else if (g[count] == ')') {
            leftBrackets++;
          }
        }
        sum = rightBrackets - leftBrackets;
        if (sum == 1) {
          return pos; 
        }
      }
    }
  }
  return 0; 
}


void complete(struct tableau *t)
{

  //creates 2 new nodes if the root is not: -PROP PROP or --FORMULA
  if(parse(t->root) != 1 && negativePropCheck(t->root)!= 1 && evaluateFormula(t->root) != 4)
  {

      struct tableau *node1 = malloc(sizeof(struct tableau));
        
        node1->root = NULL;
        node1->left = NULL;
        node1->right = NULL;
        node1->parent = NULL;
        
        struct tableau *node2= malloc(sizeof(struct tableau));
        
        node2->root = NULL;
        node2->left = NULL;
        node2->right = NULL;
        node2->parent = NULL;

       switch(evaluateFormula(t->root))
       {
        case 0:
          printf("%s\n", "This should not happen :)");
        case 1:
          node1->root = partone(t->root);
          node2->root = parttwo(t->root);
          break;

        case 2:
          node1->root = add_negation(partone(remove_negation(t->root)));
          node2->root = add_negation(parttwo(remove_negation(t->root)));
          break;  
        case 3:
          node1->root = partone(remove_negation(t->root));
          node2->root = add_negation(parttwo(remove_negation(t->root)));
          break;

        case 5:
          node1->root = partone(t->root);
          node2->root = parttwo(t->root);
          break;

        case 6:
          node1->root = add_negation(partone(t->root));
          node2->root = parttwo(t->root);
          break;

        case 7:
          node1->root = add_negation(partone(remove_negation(t->root)));
          node2->root = add_negation(parttwo(remove_negation(t->root)));
          break;
       }

      if(evaluateFormula(t->root) < 5)
      {
        add_node_left(t,node1);
        add_node_left(t,node2);
      } else 
      {
        add_node_rigth(t,node2);
        add_node_left(t,node1);
      }

  } 
  else if(evaluateFormula(t->root) == 4)
  {

      struct tableau *node1= malloc(sizeof(struct tableau));
        
        node1->root = NULL;
        node1->left = NULL;
        node1->right = NULL;
        node1->parent = NULL;

      node1->root = remove_negation(remove_negation(t->root));
      add_node_left(t,node1);
  }


  //goes to next nodes
  if(t->left != NULL)
  {
    if(t->right != NULL) 
    {
      complete(t->left);
      complete(t->right);
    } else {
      complete(t->left);
    }
  }

}


char *partone(char *g)
//Given a formula (A*B) this should return A
{
  char *part1 = malloc(Fsize);
  for(int i=1; i<bin(g); i++) {
    part1[i-1] = g[i];
  }
  return part1;
}

char *parttwo(char *g)
//Given a formula (A*B) this should return B
{
  char *part2 = malloc(Fsize);
  int end, difference;
  end = strlen(g)-(strlen(partone(g))+3);
  difference = strlen(partone(g))+2;
  for(int i=0; i<end ; i++) {
    part2[i] = g[i+difference];
  }
  return part2;
}

char *add_negation(char *g)
{
  char *finalString = malloc(Fsize);
  finalString[0]= '-';
  for(int i=1; i<=strlen(g); i++)
  {
    finalString[i] = g[i-1];
  }
  return finalString;
}

char *remove_negation(char *g)
{
  char *finalString = malloc(Fsize);
  for(int i=0; i<=strlen(g); i++)
  {
    finalString[i] = g[i+1];
  }
  return finalString;
}

void add_node_left(struct tableau *t, struct tableau *node)
{

  if(t->left == NULL)
  {
    t->left = node;
    node->parent = t;
  } 
  else if(t->left != NULL && t->right == NULL)
  {
    add_node_left(t->left, node);
  } 
  else if (t->left !=NULL && t->right !=NULL) {
    add_node_left(t->left, node);

    struct tableau *node1= malloc(sizeof(struct tableau));
        
        node1->root = node->root;
        node1->left = NULL;
        node1->right = NULL;
        node1->parent = NULL;

    add_node_left(t->right, node1);
  }

}

void add_node_rigth(struct tableau *t, struct tableau *node)
{

  if(t->left == NULL && t->right == NULL) 
  {
    node->parent = t;
    t->right = node;
  } 
  else if(t->right == NULL && t->left != NULL)
  {
    add_node_rigth(t->left, node);
  } 
  else if(t->right != NULL && t->left != NULL)
  {
    add_node_rigth(t->right, node);

    struct tableau *node1= malloc(sizeof(struct tableau));
        
        node1->root = node->root;
        node1->left = NULL;
        node1->right = NULL;
        node1->parent = NULL;

    add_node_rigth(t->left, node1);
  }

}


int evaluateFormula(char *g)
{
  if(g[0]=='-' && g[1]=='-') return 4;

  switch(g[bin(g)])
  {
    case '^':
      if(g[0]=='-') return 7;
      return 1;
      break;

    case '>':
      if(g[0]=='-') return 3;
      return 6;
      break;

    case 'v':
      if(g[0]=='-') return 2;
      return 5;
      break;
  }
 return 0;
}

int negativePropCheck(char *g){

  if(parse(g) == 2)
  {
    if(g[1] == 'p' || g[1] == 'q' || g[1] == 'r') return 1;
  }

  return 0;
}


bool closed(struct tableau *t){

  if(t->left == NULL && t->right == NULL) 
  {
    if (goUp(t->parent, t->root) == true)
    {
      return true;
    }
    else
    {
      return false;
    }
  } 
  else if(t->right == NULL && t->left != NULL)
  {
    return closed(t->left);
  } 
  else if(t->right != NULL && t->left != NULL)
  {
    return closed(t->left) || closed(t->right);
  }

  return false;
}

bool goUp(struct tableau *t, char *g)
{

  if(strcmp(remove_negation(t->root),g) == 0 || strcmp(remove_negation(g),t->root) == 0 )
  {
    if( (strlen(remove_negation(t->root)) == 1 && strlen(g) == 1) || (strlen(remove_negation(g)) == 1 && strlen(t->root) == 1) )
    {
      return true;
    } 
  } 
  else 
  {
    if(t->parent != NULL)
    {
      return goUp(t->parent, g);
    } 
    else
    {
      return false;
    }
  }

  return false;

}

int main()

{ /*input a string and check if its a propositional formula */


  char *name = malloc(Fsize);
  FILE *fp, *fpout;
 
  /* reads from input.txt, writes to output.txt*/
  if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

  int j;
  for(j=0;j<cases;j++)
    {
      fscanf(fp, "%s",name);/*read formula*/
      switch (parse(name))
  {case(0): fprintf(fpout, "%s is not a formula.  ", name);break;
  case(1): fprintf(fpout, "%s is a proposition.  ", name);break;
  case(2): fprintf(fpout, "%s is a negation.  ", name);break;
  case(3):fprintf(fpout, "%s is a binary.  ", name);break;
  default:fprintf(fpout, "What the f***!  ");
  }

      /*make new tableau with name at root, no children, no parent*/
  
      struct tableau t={name, NULL, NULL, NULL};

      /*expand the root, recursively complete the children*/
      if (parse(name)!=0)
  { complete(&t);
    if (closed(&t)) fprintf(fpout, "%s is not satisfiable.\n", name);
    else fprintf(fpout, "%s is satisfiable.\n", name);
  }
      else fprintf(fpout, "I told you, %s is not a formula.\n", name);
    }
 
  fclose(fp);
  fclose(fpout);
  free(name);

  return(0);

}








