#include<stdio.h>  
#include<ctype.h>
#include<string.h>
#include<malloc.h>


/* define false and True*/
enum { false = 0, true = 1 };


#define NAME 100  /*Max characters allowed for the name of the input file.*/

#define MAX 80	  /*Max characters allowed in one line.*/

/*Directive line types משפטי הנחיה*/

#define DATA 0
#define STRING 1
#define ENTRY 2
#define EXTERN 3
#define no_dir 4


/*Addressing Modes שיטות מיעון Pg 17*/
#define InstantAddressing 0				  /*מיעון מידי*/
#define DirectAddressing 1			  	  /*מיעון ישיר*/
#define InstantDynamicAddressing 2 /*מיעון מידי דינמי*/
#define DirectRegisterAddressing 3  /*מיעון אוגר ישיר*/
#define no_operand 7


#define no_arg 6

#define no_reg 30
#define labelMax 40

#define str(s) #s

#define forever while(true) /* infinie loop.*/

#define HASHSIZE 101

/* Newline */
#define NewLine 10

#define AlreadyExists 0
#define Success 1
#define OutOfMemory 2


#define no_ins 16

enum {
	SyntaxError =1, labelAsInstruction, labelAsDirective, DefinedLabelAlready, labelTooLong
	, UnrecognizedDirective	=6, unrecognizedInsruction, firstCharlabelNotletter, line2Long, badArgument 
	, labelOnly	=11, IllegalAddressing, labelAsRegister, EmptyQuotationMarks, labelNotDefined
	, labelAlreadyDefinedExternal =16,	InternallabelAsexternal, expectingArgument
	, NotPossible =-99
};

/*
!@#$%^&*
אין תיעוד מספק בקובץ ה 
header 
זה בדיוק המקום לתעד בצורה הדוקה

האם יש צורך בHASH או שזה רק מסבך את הפתרון
נכון שזה "מהיר יותר" אבל כל גודל הזכרון הוא 1000,
אז זה לא משמעותי
*/

///// cpuOpcode
typedef union cpuOpcode
{
	struct
	{
		unsigned short era : 2;
		unsigned short opDest : 2;
		unsigned short opSource : 2;
		unsigned short opcode : 4;
		unsigned short group : 2;
		unsigned short unused : 3;
	} u;
	unsigned short fullReg;
} cpuOpcode;


typedef union cpuOpcode2
{
	struct
	{
		unsigned short era : 2;
		unsigned short direct_operand : 13;
		unsigned short unused : 1;
	} u;
	unsigned short fullReg;
} cpuOpcode2;

///// end

/*------------------------------------STRACTURES------------------------------------------*/

struct keywords {
	char *word;
};
	 
struct boolian{
	unsigned int flag : 1;
};

struct nlist{
	struct nlist *next;
	char *name;
	int value;
};



/*--------------------- GLOBAL VARIABLES ---------------------------------------- */

int ic, dc, ac ;  /* Global variables for "instruction counter", "data counter" and "address counter" (respectively).*/
int er;
int error  ;  /* check errors  */

char LABEL[labelMax];

int inscode   ;
int dirtype   ;
int regist    ;





/* Function declaration */

/* first_pass.c */
int assemble(char *);
int read_line(char *);
int check_ext(char *);
//void as_extens(char *, char *);
char *file_name(char *fname);
void add_extens(char *, char *, char *);
int find_dir(char *);
int find_reg(char *);
char *skipspace(char *);
int look4label(char *);
int look4dir(char *);
int look4ins(char *);
int dir2value(char *, int );
int ins2value(char *, int );
char *go2comma(char *);
int islabel(char *);
char *nextfield(char *);
int isnumber(char *);
int lgh2blank(char * );
int cont(char *);
int ignoreline(char *);
/* error.c */
void write_error(int);
int write_error2(int, char *);

/* second_pass.c  */
int addresscode(FILE *);
int print(FILE *, int, int, int, int, int,char);
int dir2ob(FILE *,char *);
int write2files(FILE *,FILE *,FILE *,char *,int);
int second_pass(char*,char*);
int write2ent(FILE *, char *);
char *cpyarg(char *,char);
int wrt(FILE *, FILE * , char *);
int addmeth(char *);
int print(FILE *, int , int , int , int , int ,char  );
int addrval(FILE *, char *,int *, char *,int);



/* LAB_tab.c */
int install(char *, int);
struct nlist *lookup(char *);
unsigned hash(char *);
void clean_tab();
/* en_tab.c  */
int ins2entry(char *);
struct nlist *look4entry(char *);
void clean_en();
char *strDup(char *);
/* ex_tab.c  */
int install2ext(char *);
struct nlist *look4ext(char *);
void clean_ex();
