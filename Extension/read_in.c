#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define IFNULL(A,B) if(A == NULL) {fprintf(stderr, "%s\n", B);}
#define DELIM ","
#define MAXLINELENGTH 200

char** tokens_init(int tk);
int count_tokens(FILE* fp);
void print_tokens(char** t, int l);

int main(void)
{
  FILE* fp;
  char** tokens;
  char* p;
  int i,tk;
  fp = fopen("authoring tool.txt","rb");
  IFNULL(fp,"Input file failed to open\n")
  tk = count_tokens(fp);
  printf("%d\n",tk);
  tokens = tokens_init(tk);
  for(i=0,p = strtok(tokens[i],DELIM);p != NULL;p = strtok(NULL,DELIM)) {
    printf("%s\n",p);
    strcpy(tokens[i],p);
    i++;
  }
  print_tokens(tokens,tk);
  return 0;
}

void print_tokens(char** t, int l)
{
  int i;
  for(i=0;i<l;i++) {
    printf("%s\n",t[i]);
  }
}

char** tokens_init(int tk)
{
  int i;
  char** t = (char**) calloc(tk,sizeof(char*));
  IFNULL(t,"Lines array failed to initialise\n")
  for(i=0;i<tk;i++) {
    t[i] = (char*) calloc(MAXLINELENGTH, sizeof(char));
    IFNULL(t[i],"Lines array failed to initialise\n")
  }
  return t;
}


int count_tokens(FILE* fp)
{
  int c = 0;
  char ch;
  while(!feof(fp)) {
    ch = fgetc(fp);
    if(ch == ',' || ch == '\n') {
      c++;
    }
  }
  rewind(fp);
  return c;
}
