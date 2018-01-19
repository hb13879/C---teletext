#include "rdp.h"

int main(void)
{
  int tk,ch;
  FILE* fp;
  char* string;
  Uch* result;
  Uch* orig; /*record to start of array*/
  char** tokens;
  fp = open_file();
  count_tokens(fp,&tk,&ch);
  string = string_init(ch);
  result = result_init();
  orig = result;
  tokens = read_in(fp,tk);
  parse(tk,&string,&result,tokens,orig);
  print_array(orig);
  write_to_file(orig);
  fclose(fp);
  return 0;
}

void parse(int tk,char** string, Uch** result,char** tokens, Uch* orig)
{
  int i,db;
  db = 0;
  for(i=0;i<tk;i++) {
    strcpy(*string,tokens[i]);
    /*IF statement simply a wrapper to catch junk following correct whole statements*/
    if(Assignment1(string,result));
    else if(Assignment2(string,result,&db));
    else if(Assignment3(string,result));
    else if(InputData(string,result,db,orig));
    else if(HeldGraphics(string,result));
    else if(NewLine(string,result,orig));
    /*else if(End(string));*/
    else {
      fprintf(stderr, "%s %d\n%s\n", "Syntax Error at following token:",i,"Must be either an Assignment, Data input or New line");
      exit(EXIT_FAILURE);
    }
    check_junk(string,i);
  }
}

void check_junk(char** s, int i)
{
  if(**s != '\0') {
    fprintf(stderr, "%s %d\n", "Junk after correct formula in token",i);
  }
}

void write_to_file(Uch* r)
{
  FILE* fp;
  fp = fopen("custom.m7","wb");
  IFNULL(fp,"Output file failed to open\n")
  fwrite(r,1,MAXOUTPUT,fp);
  fclose(fp);
}

FILE* open_file(void)
{
  FILE* fp;
  fp = fopen("authoring tool.txt","rb");
  IFNULL(fp,"Input file failed to open\n")
  return fp;
}

Uch* result_init(void)
{
  Uch* r;
  r = (Uch*) calloc(MAXOUTPUT,sizeof(Uch));
  IFNULL(r,"result string could not be initialised\n");
  return r;
}

char* string_init(int ch)
{
  char* s;
  s = (char*) calloc(ch+1,sizeof(char));
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

char** read_in(FILE* fp,int tk)
{
  char** tokens;
  tokens = tokens_init(tk);
  process_tokens(tokens,fp);
  print_tokens(tokens,tk);
  return tokens;
}

void process_tokens(char** t,FILE* fp)
{
  int i = 0;
  char a[MAXLINELENGTH];
  char* p;
  while(fgets(a,sizeof(a),fp) != NULL) {
    remove_newline(a);
    p = strtok(a,DELIM);
    while(p!=NULL) {
      /*printf("%s\n",strtok(a,DELIM));*/
      strcpy(t[i],p);
      p = strtok(NULL,DELIM);
      i++;
    }
  }
}

void count_tokens(FILE* fp, int* tk, int* chars)
{
  char ch;
  while((ch = fgetc(fp)) != EOF) {
    if(ch == ',' || ch == ';' || ch == '\n') {
      (*tk)++;
    }
    (*chars)++;
  }
  rewind(fp);
}

void remove_newline(char* a)
{
  a[strlen(a) - 1] = ';';
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

void print_array(Uch* a)
{
  int i;
  for(i=0;i<MAXOUTPUT;i++) {
    if(i%COLS == 0) {
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

int Assignment1(char** s, Uch** r)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(!ColParam(s,r)) {
    *s = tmp;
    return 0;
  }
  if(!Colon(s)) {
    *s = tmp;
    return 0;
  }
  if(!Colour(s,r)) {
    *s = tmp;
    return 0;
  }
  return 1;
}

int Assignment2(char** s, Uch** r, int* db)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(!Height(s)) {
    *s = tmp;
    return 0;
  }
  if(!Colon(s)) {
    *s = tmp;
    return 0;
  }
  if(!HeightMd(s,r,db)) {
    *s = tmp;
    return 0;
  }
  return 1;
}

int Assignment3(char** s, Uch** r)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(!GraphMd(s)) {
    *s = tmp;
    return 0;
  }
  if(!Colon(s)) {
    *s = tmp;
    return 0;
  }
  if(!GraphMdParam(s,r)) {
    *s = tmp;
    return 0;
  }
  return 1;
}

int GraphMd(char** s)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(match(s,GRAPHMD)) {
    return 1;
  }
  else {
    *s = tmp;
    return 0;
  }
}

int GraphMdParam(char** s,Uch** r)
{
  char *tmp = *s;
  SkipWhitespace(s);
  if(match(s,GRAPHMD1)) {
    **r = 0x99;
    (*r)++;
    return 1;
  }
  else if(match(s,GRAPHMD2)) {
    **r = 0x9A;
    (*r)++;
    return 1;
  }
  else {
    *s = tmp;
    return 0;
  }
}

int InputData(char** s, Uch** f, int db, Uch* orig)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(!Data(s)) {
    *s = tmp;
    return 0;
  }
  if(!Colon(s)) {
    *s = tmp;
    return 0;
  }
  if(!Speech(s)) {
    *s = tmp;
    return 0;
  }
  if(!DataValue(s,f,db,orig)) {
    *s = tmp;
    return 0;
  }
  if(!Speech(s)) {
    *s = tmp;
    return 0;
  }
  return 1;
}

int DataValue(char** s, Uch** f, int db, Uch* orig)
{
  char* tmp = *s;
  char data[MAXDATA];
  int i,numcharsread;
  SkipWhitespace(s);
  if(sscanf(*s,"%[^\n^\"]%n",data,&numcharsread) == 1) {
    for(i = 0;i<numcharsread;i++) {
      **f = data[i] + 128;
      if(db == 1) {
        *f = wrap((*f) + COLS,orig); /*set next row to the same data since double height*/
        **f = data[i] + 128;
        *f = (*f) - COLS; /*reset pointer*/
      }
      (*f)++;
    }
    (*s)+=numcharsread;
    return 1;
  }
  else {
    *s = tmp;
    return 0;
  }
}

Uch* wrap(Uch* a, Uch* orig)
{
  if(a > (orig + MAXOUTPUT)) {
    return a - COLS;
  }
  return a;
}

int ColParam(char** s,Uch** r)
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
  else if(match(s,PARAM3)) {
    **r = 'g';
    return 1;
  }
  else {
    *s = tmp;
    return 0;
  }
}


int HeldGraphics(char** s,Uch** r)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(match(s,HOLD)) {
    **r = 0x9E;
    (*r)++;
    return 1;
  }
  else if(match(s,RELEASE)) {
    **r = 0x9F;
    (*r)++;
    return 1;
  }
  else {
    *s = tmp;
    return 0;
  }
}

int Height(char** s)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(match(s,HEIGHT)) {
    return 1;
  }
  else {
    *s = tmp;
    return 0;
  }
}

int HeightMd(char** s,Uch** r,int* db)
{
  char *tmp = *s;
  SkipWhitespace(s);
  if(match(s,HEIGHTMD1)) {
    **r = 0x8C;
    (*r)++;
    *db = 0;
    return 1;
  }
  else if(match(s,HEIGHTMD2)) {
    **r = 0x8D;
    (*r)++;
    *db = 1;
    return 1;
  }
  else {
    *s = tmp;
    return 0;
  }
}

void set_background(Uch** r,colour x)
{
  **r = 0x80 + x;
  (*r)++;
  **r = 0x9D;
  (*r)++;
}

void set_foreground(Uch** r,colour x)
{
  **r = 0x80 + x;
  (*r)++;
}

void set_graphics(Uch** r, colour x)
{
  **r = 0x90 + x;
  (*r)++;
}

void set_colour(Uch** r, colour x)
{
  if(!x) {
    **r = 0x9C;
    (*r)++;
  }
  else if(**r == 'b') { /*if previous command was change background*/
    set_background(r,x);
  }
  else if (**r == 'f') {
    set_foreground(r,x);
  }
  else if (**r == 'g') {
    set_graphics(r,x);
  }
}

int Colour(char** s,Uch** r)
{
  char *tmp = *s;
  SkipWhitespace(s);
  if(match(s,COLOUR1)) {
    set_colour(r,red);
    return 1;
  }
  else if(match(s,COLOUR2)) {
    set_colour(r,green);
    return 1;
  }
  else if(match(s,COLOUR3)) {
    set_colour(r,yellow);
    return 1;
  }
  else if(match(s,COLOUR4)) {
    set_colour(r,blue);
    return 1;
  }
  else if(match(s,COLOUR5)) {
    set_colour(r,magenta);
    return 1;
  }
  else if(match(s,COLOUR6)) {
    set_colour(r,cyan);
    return 1;
  }
  else if(match(s,COLOUR7)) {
    set_colour(r,white);
    return 1;
  }
  else if(match(s,COLOUR8)) {
    set_colour(r,black);
    return 1;
  }
  else {
    *s = tmp;
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
    return 0;
  }
}

int NewLine(char** s,Uch** r,Uch* orig)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(match(s,";")) {
    while((((*r)+sizeof(Uch))-orig)%(COLS) != 0) {
      if(**r == 0) {
        **r = 0xa0;
        (*r)++;
      }
      else {
        (*r)++;
      }
    }
    **r = 0xa0;
    (*r)++;
    return 1;
  }
  else {
    *s = tmp;
    return 0;
  }
}

int End(char** s)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(match(s,"\0")) {
    return 1;
  }
  else {
    *s = tmp;
    return 0;
  }
}
