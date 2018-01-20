#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum colour{black, red, green, yellow, blue, magenta, cyan, white};
typedef enum colour colour;

typedef unsigned char Uch;

/*Parameters*/
#define PARAM1 "BACKGROUND"
#define PARAM2 "FOREGROUND"
#define PARAM3 "GRAPHICS"
#define COLOUR1 "RED"
#define COLOUR2 "GREEN"
#define COLOUR3 "YELLOW"
#define COLOUR4 "BLUE"
#define COLOUR5 "MAGENTA"
#define COLOUR6 "CYAN"
#define COLOUR7 "WHITE"
#define COLOUR8 "BLACK"
#define DATA "DATA"
#define HEIGHT "HEIGHT"
#define GRAPHMD "GRAPHMD"
#define HOLD "HOLD"
#define RELEASE "RELEASE"
#define HEIGHTMD1 "SINGLE"
#define HEIGHTMD2 "DOUBLE"
#define GRAPHMD1 "CONTIGIOUS"
#define GRAPHMD2 "SEPARATED"
#define END "END"

/*Auxiliary*/
#define FILENAME "authoring_tool.txt"
#define MAXDATA 200
#define MAXSTRING 1000
#define MAXOUTPUT ROWS*COLS
#define ROWS 25
#define COLS 40
#define MAXTOKENS 200
#define MAXTOKENLENGTH 100
#define DELIM ","
#define MAXLINELENGTH 1000
#define IFNULL(A,B) if(A == NULL) {B;}
#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)

/*Recursive Descent Parser Functions*/
void SkipWhitespace(char** s);
int match(char** s, const char* token);
int Assignment1(char** s,unsigned char** r); /*COLPARAM COLON COLOUR*/
int Assignment2(char** s, unsigned char** r,int* db); /*HEIGHT COLON HEIGHTMD*/
int Assignment3(char** s, unsigned char** r); /*GRAPHMD COLON GRAPHMDPARAM*/
int InputData(char** s, unsigned char** f,int db, unsigned char* orig);
int DataValue(char** s, unsigned char** f, int db, unsigned char* orig);
int ColParam(char** s, unsigned char** r);
int Colour(char** s, unsigned char** r);
int Colon(char** s);
int Data(char** s);
int Speech(char** s);
int NewLine(char** s,unsigned char** f,unsigned char* orig);
int End(char** s, Uch** r,Uch* rx);
int Height(char** s);
int HeightMd(char** s,unsigned char** r, int* db);
int GraphMdParam(char** s,unsigned char** r);
int GraphMd(char** s);
int HeldGraphics(char** s,unsigned char** r);
void set_background(unsigned char** r,colour x);
void set_foreground(unsigned char** r,colour x);
void set_graphics(unsigned char** r, colour x);

/*I/O Functions*/
void print_array(unsigned char* a);
char** read_in(FILE* fp,int tk);
void print_tokens(char** t, int l);
char* string_init(int ch);
unsigned char* result_init(void);
char** tokens_init(int tk);
void remove_newline(char* a);
void count_tokens(FILE* fp, int* tk, int* ch);
void process_tokens(char** t,FILE* fp);
FILE* open_file(void);
void write_to_file(unsigned char* r);
void parse(int tk,char** string, unsigned char** result,char** tokens, unsigned char* orig);
unsigned char* wrap(unsigned char* a, unsigned char* orig);
void check_junk(char** s,int i);
void free_tokens(char*** t);
void free_string(char** s);
void free_result(Uch** r);
