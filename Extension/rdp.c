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

void SkipWhitespace(char** s);
int match(char** s, const char* token);
int Assignment(char** s);
int InputData(char** s);
int DataValue(char** s);
int Parameter(char** s);
int ParamValue(char** s);
int Colon(char** s);
int Data(char** s);
int Speech(char** s);
int NewLine(char** s);
int Comma(char** s);
void change(char** p);

int main(void)
{
  char* string;
  string = (char*) calloc(MAXSTRING,sizeof(char));
  strcpy(string,"BACKGROUND:YELLOW");
  printf("%d\n",Assignment(&string));
  printf("%d\n",Data(&string));
  return 0;
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

int Assignment(char** s)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(!Parameter(s)) {
    *s = tmp;
    printf("prameter\n");
    return 0;
  }
  if(!Colon(s)) {
    *s = tmp;
    printf("colon\n");
    return 0;
  }
  if(!ParamValue(s)) {
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

int InputData(char** s)
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
  if(!DataValue(s)) {
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

int DataValue(char** s)
{
  char* tmp = *s;
  char data[MAXDATA];
  int numcharsread;
  SkipWhitespace(s);
  if(sscanf(*s,"%[^\n]%n",data,&numcharsread) == 1) {
    *s+=numcharsread;
    return 1;
  }
  else {
    *s = tmp;
    printf("datavalue\n");
    return 0;
  }
}

int Parameter(char** s)
{
  char* tmp = *s;
  SkipWhitespace(s);
  if(match(s,PARAM1) || match(s,PARAM2)) {
    return 1;
  }
  else {
    *s = tmp;
    printf("parameter\n");
    return 0;
  }
}

int ParamValue(char** s)
{
  char *tmp = *s;
  SkipWhitespace(s);
  if(match(s,PARAMVALUE1) || match(s,PARAMVALUE2)) {
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
