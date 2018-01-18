#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PARAM1 "BACKGROUND"
#define PARAM2 "FOREGROUND"
#define PARAMVALUE1 "RED"
#define PARAMVALUE2 "YELLOW"
#define DATA "DATA"
#define MAXDATA 200
#define TEST "BACKGROUND"
#define MAXSTRING 100
#define MAXOUTPUT ROWS*COLS
#define ROWS 25
#define COLS 40
#define MAXTOKENS 200
#define MAXTOKENLENGTH 100
#define DELIM ","
#define MAXLINELENGTH 100
#define IFNULL(A,B) if(A == NULL) {fprintf(stderr, "%s\n", B);}

void SkipWhitespace(char** s);
int match(char** s, const char* token);
int Assignment(char** s,unsigned char** r);
int InputData(char** s, unsigned char** f);
int DataValue(char** s, unsigned char** f);
int Parameter(char** s, unsigned char** r);
int ParamValue(char** s, unsigned char** r);
int Colon(char** s);
int Data(char** s);
int Speech(char** s);
int NewLine(char** s);
int Comma(char** s);
void print_array(unsigned char* a);
char** read_in(void);
void print_tokens(char** t, int l);
char* string_init(void);
unsigned char* result_init(void);
int count_lines(FILE* fp);
char** tokens_init(int l);
void remove_newline(char* a);
int count_tokens(FILE* fp);
void process_tokens(char** t,FILE* fp);

int main(void)
{
  char* string;
  unsigned char* result;
  unsigned char* orig;
  char** tokens;
  string = string_init();
  result = result_init();
  orig = result;
  tokens = read_in();
  strcpy(string,"DATA:\"hello\",");
  /*printf("%d\n",Assignment(&string,&result));
  printf("%d\n",InputData(&string,&result));
  print_array(orig);*/
  return 0;
}

unsigned char* result_init(void)
{
  unsigned char* r;
  r = (unsigned char*) calloc(MAXOUTPUT,sizeof(unsigned char));
  IFNULL(r,"result string could not be initialised\n");
  return r;
}

char* string_init(void)
{
  char* s;
  s = (char*) calloc(MAXSTRING,sizeof(char));
  IFNULL(s,"String could not be initialised\n")
  return s;
}

void print_tokens(char** t, int l)
{
  int i;
  for(i=0;i<l;i++) {
    printf("%s\n",t[i]);
  }
}

char** read_in(void)
{
  FILE* fp;
  char** tokens;
  int tk;
  fp = fopen("authoring tool.txt","rb");
  IFNULL(fp,"Input file failed to open\n")
  tk = count_tokens(fp);
  printf("%d\n",tk);
  tokens = tokens_init(tk);
  process_tokens(tokens,fp);
  print_tokens(tokens,tk);
  fclose(fp);
  return tokens;
}

void process_tokens(char** t,FILE* fp)
{
  int i = 0;
  char a[MAXLINELENGTH];
  char* p;
  while(fgets(a,sizeof(a),fp) != NULL) {
    remove_newline(a);
    printf("%s\n",a);
    p = strtok(a,DELIM);
    while(p!=NULL) {
      /*printf("%s\n",strtok(a,DELIM));*/
      strcpy(t[i],p);
      p = strtok(NULL,DELIM);
      i++;
    }
  }
}

int count_tokens(FILE* fp)
{
  int c = 0;
  char ch;
  while((ch = fgetc(fp)) != EOF) {
    if(ch == ',' || ch == ';' || ch == '\n') {
      printf("%c ",ch);
      c++;
    }
  }
  rewind(fp);
  return c;
}

void remove_newline(char* a)
{
  a[strlen(a) - 1] = ';';
}

char** tokens_init(int l)
{
  int i;
  char** t = (char**) calloc(l,sizeof(char*));
  IFNULL(t,"Lines array failed to initialise\n")
  for(i=0;i<l;i++) {
    t[i] = (char*) calloc(MAXLINELENGTH, sizeof(char));
    IFNULL(t[i],"Lines array failed to initialise\n")
  }
  return t;
}

char** alltokens_init(int tk)
{
  int i;
  char** t = (char**) calloc(tk,sizeof(char*));
  IFNULL(t,"Lines array failed to initialise\n")
  for(i=0;i<tk;i++) {
    t[i] = (char*) calloc(MAXTOKENLENGTH, sizeof(char));
    IFNULL(t[i],"Lines array failed to initialise\n")
  }
  return t;
}

int count_lines(FILE* fp)
{
  int c = 0;
  char ch;
  while(!feof(fp)) {
    ch = fgetc(fp);
    if(ch == '\n') {
      c++;
    }
  }
  rewind(fp);
  return c;
}

void print_array(unsigned char* a)
{
  int i;
  for(i=0;i<5;i++) {
    if(i%40 == 0) {
      printf("\n");
    }
    printf("%x ",a[i]);
  }
  printf("\n");
}

void SkipWhitespace(char** s)
{
  while(**s == ' ' || **s == '\t') {
    (*s)++;
  }
}

int match(char** s, const char* token)
{
  if(strncmp(*s,token,strlen(token)) == 0) {
    *s = *s + strlen(token);
    return 1;
  }
  else {
    return 0;
  }
}

int Assignment(char** s, unsigned char** r)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(!Parameter(s,r)) {
    *s = tmp;
    printf("prameter\n");
    return 0;
  }
  if(!Colon(s)) {
    *s = tmp;
    printf("colon\n");
    return 0;
  }
  if(!ParamValue(s,r)) {
    *s = tmp;
    printf("paramvalue\n");
    return 0;
  }
  if(!Comma(s)) {
    *s = tmp;
    printf("comma\n");
    return 0;
  }
  return 1;
}

int InputData(char** s, unsigned char** f)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(!Data(s)) {
    *s = tmp;
    printf("inputdata\n");
    return 0;
  }
  if(!Colon(s)) {
    *s = tmp;
    printf("colon\n");
    return 0;
  }
  if(!Speech(s)) {
    *s = tmp;
    printf("speech\n");
    return 0;
  }
  if(!DataValue(s,f)) {
    *s = tmp;
    printf("datavalue\n");
    return 0;
  }
  if(!Speech(s)) {
    *s = tmp;
    printf("speech\n");
    return 0;
  }
  if(!Comma(s)) {
    *s = tmp;
    printf("comma\n");
    return 0;
  }
  return 1;
}

int DataValue(char** s, unsigned char** f)
{
  char* tmp = *s;
  char data[MAXDATA];
  int i,numcharsread;
  SkipWhitespace(s);
  if(sscanf(*s,"%[^\n]%n",data,&numcharsread) == 1) {
    for(i = 0;i<MAXDATA;i++) {
      **f = data[i] + 128;
      (*f)++;
    }
    (*s)+=numcharsread;
    return 1;
  }
  else {
    *s = tmp;
    printf("datavalue\n");
    return 0;
  }
}

int Parameter(char** s,unsigned char** r)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(match(s,PARAM1)) {
    **r = 'b';
    return 1;
  }
  else if(match(s,PARAM2)) {
    **r = 'f';
    return 1;
  }
  else {
    *s = tmp;
    printf("parameter\n");
    return 0;
  }
}

int ParamValue(char** s,unsigned char** r)
{
  char *tmp = *s;
  SkipWhitespace(s);
  if(match(s,PARAMVALUE1)) {
    if(**r == 'b') { /*if previous command was change background*/
      **r = 0x81; /*code for set foreground to red*/
      (*r)++;
      **r = 0x9D; /*code for set background to foreground*/
      (*r)++;
    }
    else {
      **r = 0x81;
      (*r)++;
    }
    return 1;
  }
  else if(match(s,PARAMVALUE2)) {
    if(**r == 'b') { /*if previous command was change background*/
      **r = 0x82; /*code for set foreground to yellow*/
      (*r)++;
      **r = 0x9D; /*code for set background to foreground*/
      (*r)++;
    }
    else {
      **r = 0x82;
      (*r)++;
    }
    return 1;
  }
  else {
    *s = tmp;
    printf("paramvalue\n");
    return 0;
  }
}

int Colon(char** s)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(match(s,":")) {
    return 1;
  }
  else {
    *s = tmp;
    printf("colon\n");
    return 0;
  }
}


int Comma(char** s)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(match(s,",")) {
    return 1;
  }
  else {
    *s = tmp;
    printf("coma\n");
    return 0;
  }
}

int Data(char** s)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(match(s,DATA)) {
    return 1;
  }
  else {
    *s = tmp;
    printf("data\n");
    return 0;
  }
}

int Speech(char** s)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(match(s,"\"")) {
    return 1;
  }
  else {
    *s = tmp;
    printf("speech\n");
    return 0;
  }
}

int NewLine(char** s)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(match(s,"\n")) {
    return 1;
  }
  else {
    *s = tmp;
    printf("newline\n");
    return 0;
  }
}
