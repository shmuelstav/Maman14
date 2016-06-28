/* ================= first_pass.c ===================*/ 
//iguguigui
/* !!!Gady Renames 

ch --> filename

*/
/*
The purpose of this source file is to complete the "first pass" as specified on the assignment project.
*/
#define _CRT_NONSTDC_NO_WARNINGS
#include "asm.h"

struct keywords instruction[] ={
  {"mov"},{"cmp"},{"add"},{"sub"},{"not"},{"clr"},{"lea"},{"inc"},
  {"dec"},{"jmp"},{"bne"},{"red"},{"prn"},{"jsr"},{"rts"},{"stop"},
};
/*
0 mov	2	0,1,2,3	1,3
1 cmp	2	0,1,2,3	0,1,2,3
2 add	2	0,1,2,3	1,3
3 sub	2	0,1,2,3	1,3
4 not	1	_	1,3
5 clr	1	_	1,3
6 lea	2	1	1,3
7 inc	1	_	1,3
8 dec	1	_	1,3
9 jmp	1	_	1,3
10 bne	1	_	1,3
11 red	1	_	1,3
12 prn	1	_	0,1,2,3
13 jsr	1	_	1,3
14 rts	0	_	_
15 stop	0	_	_
*/

struct keywords directive[] = {
  {"data"},{"string"},
  {"entry"},{"extern"},
};

struct keywords reg[]={
  {"r0"},{"r1"},{"r2"},{"r3"},{"r4"},{"r5"},{"r6"},{"r7"},
};


/*------------------------- ASSEMBLE ---------------------
assemble: This is the main function of this file.
          It activates the "first pass" on the input file (the file ending with "*.as") 
		  file. It then calls read_line, and write_error on
		  each iteration.
*/
int assemble(char *ch){
	FILE *as ;
	char as_name[NAME] ;
	char line[MAX];
		        
	int  ln=1;      /* ln counts line number in the source */
		
	struct boolian longline; //{0}
		

    er=0;	
	ic=0; /* initiating instruction counter. */
	dc=0; /* initiating data counter. */
	ac=0; /* initiating address counter. */
	
	
	//as_extens(ch,as_name);
	add_extens(ch, as_name, ".as");

	as=fopen(as_name,"r");
	
	if(as == NULL){
		printf("\n\n File \"%s\" could not open  \n\n",as_name);
		return false;
	}
		               
	forever {
		
		/* initiating instruction codes and directive values*/
		inscode=no_ins;
		//dirtype=no_dir;
		/*________________________________________________*/
		error=0;
		longline.flag = false;
		
		/*------- Set a breaking point.------------------*/
		if(fgets(line,MAX,as)==NULL) // WHY DO WE READ ONLY MAX
			break;		/* Finish reading file, exiting loop. */
		/*_______________________________________________*/

		if(! ignoreline(line)){
			/*------- Checking line's length ---------------*/
			if(strlen(line)>MAX-2){ /*starts with 0 so max -1 and use > so -2 */
				longline.flag=true;  /* Marks long line.*/
				error=line2Long;
				er++;
				write_error(ln);
				ln--;
			}			
			/*_____________________________________________*/

			if(!longline.flag){  /* If line is not to long, proceed...	*/
				read_line(line); 
				if (error != 0)  /* Count errors */
					er++;
				write_error(ln);			
			}
		}
		ln++;	/*count line numbers in the source code.*/
	}
	/* End of loop. */
	
	fclose(as);
	second_pass(line,ch);  /* executing the second pass on "as" file.*/
	
	if( er > 1 )
		printf("\n File name %s contains %d errors.\n",as_name,er);
	if( er == 1 )
		printf("\n File name %s contains %d error.",as_name,er); 
	return true;
}

/*----------------------- read_line  ----------------------------------------------
read_line: Read a line from "as" file determine whether a label exists
		   determine whether the line contains a directive or 
		   instruction and calls the appropriate function[s] accordingly.
-----------------------------------------------------------------------------------
*/
int read_line(char *line){
	
	/*char *p=line  ;*/
	int foundInstruction=false;
	int label=false;
	
	/*----- Remarks and blank lines -----*/
	if (*line == ';') //why we already ignored remarks ;
		return false ;
	line=skipspace(line);
	if(line == NULL)
		return false;
	/*___________________________________*/
	

	/* first character can either be a dot or a letter. */
	if( !isalpha(*line) && *line != '.'  ){   
		error= SyntaxError;
		return false;
	}
	/*__________________________________________________ */

	/*If there is a label, "look4label" detect it, 
 	  put it in the symbol table, mark it with label.
	  flag and move to the next field. */	
	if(look4label(line)){ 
		label=true ;			/* Mark label.*/
		line=nextfield(line) ; /* Go to the next field.*/
	}
	
	if (line == NULL){
		error= labelOnly;
		return false;
	}

	/* Looking for directive...-------------*/
	if(*line == '.' ){
		look4dir(line);
		if(dirtype == no_dir){ 
			error= UnrecognizedDirective;
			return false;
		}
		dir2value(line,label);
		return true;
	}
	/*--------------------------------------*/
	
	/* Looking for instruction...*/
	foundInstruction=look4ins(line);
	if(!foundInstruction && !error){
		error= unrecognizedInsruction;
		return false;
	}
	if( foundInstruction ){
		ins2value(line,label);
	}
	return true;
}


/*---------------------look4label---------------------------------
look4label: return true on label appearance. Else return false.
*/
int look4label(char *field ){
	char *p=field;
	
	int  i=0, c=0;
	
	while( *p != ':' && *p != NewLine && i<labelMax){
		LABEL[i]=*p;
		p++;
		i++;
	}
	LABEL[i]='\0';
	
	while( *p != ':' && *p != NewLine && *p!='\0')  /* problem here 1*/
		p++;
	if ( *p == ':' ){						   
		if (i >= labelMax){
			error=labelTooLong;		/* label is to long.*/
			return false;
		}
		for(c=0;c<i;c++){ /* checking label characters.*/
			if(!isdigit(LABEL[c]) && !isalpha(LABEL[c])){
			error=SyntaxError;
			return false;
			}
		}
			
		if ( *field == '.' || isdigit(*field)  ){ /* label cannot start with a dot or number.*/
			error=firstCharlabelNotletter;
			return false;
		}		
		if (look4ins(LABEL)){   /* label cannot be an instruction.*/
			error= labelAsInstruction;
			return false;
		}
		if (find_dir(LABEL)){   /* label cannot be a directive.*/
			error=labelAsDirective;
			return false;
		}
		if(find_reg(LABEL)){  /* label cannot be a register.*/
			error=labelAsRegister;
			return false;
		}
		if(!error)
			return true;
	}
	return false;
}



/*----------- SKIPSPACE -----------------------------------------------
skipspace: Skips on blank character[s] and returns the
		   a pointer to the first non blank character.
--------------------------------------------------------
*/
char *skipspace(char *line){
	if(line == NULL)
		return NULL;
	if(strlen(line) <= 1)       
		return NULL;
	while(*line==' ' || *line=='\t'){
		if (*line == '\0' || *line==NewLine)
			break;
		line++;
		
	} 
	return line;
}
/* ----------------------     -------------------------------------------------- */


	


/*------------------ MAKE_EXTENS ---------------------------------------
add_extens: Generates the appropriate extension to the 
             files.
---------------------------------------------------------
*/
void add_extens(char *ch, char *s, char *ext){
	strcpy(s,ch) ;
	strcat(s,ext) ;	
}


/*  --------------- file_name------------------------
file_name: This function returns the file name without the extension.
------------------------------------------------------
 */ //maybe keep original and we woulnd not need this function?
char *file_name(char *fname){
	int i, j=0;
	char *ch, *fi, *fn=fname;
	ch= (char *) malloc(sizeof(char*));
	fi=fname;
	while(*fn!='\0'){
		j++;
		fn++;
	}
	while(*fn!='.'){
		j--;
		fn--;
	}
	fn--;
	for(i=0;i<j;i++){
		*ch=*fname;
		ch++;
		fname++;
	}
	*ch='\0';
	for(i=0;i<j;i++)
		ch--;
	return ch;
}
/*----------------- NEXTFIELD ----------------------------------------------
nextfield: Returns a pointer to the next "field" of the line
		   i.e. it choose the first character after "blank".		
*/
char *nextfield(char *line){
	if( line == NULL)
		return NULL;
	while( *line != ' ' && *line != '\t' && *line != NewLine && *line != '\0' ){
		line++;
	}
	line++;
	line=skipspace(line);
	return line;
}


/*------------------- FIND_DIR -----------------------------------------
find_dir: Finds out whether p is a legitimate directive 
          name.
---------------------------------------------------------
*/
int find_dir(char *p){

	int i;
	for(i=0;i<4;i++){
		if ( strcmp(directive[i].word , p) == 0 ){
			return true ;
		}
	}
	return false ;
}


/*----------------------- look4dir ------------------------------------------------
look4dir: Returns true if p is a legitimate directive name, else it returns false.  
-----------------------------------------------------------------------------------
*/
//Look4dir and find_dir are the same?
int look4dir(char *p){
	if(p == NULL)
		return false;
	if ( strncmp("data",p+1,4)==0 && lgh2blank(p+1)==4){  
		dirtype= DATA;  /* Mark directive type. */
		return true;
	}
	if (strncmp("string",p+1,6)==0 && lgh2blank(p+1)==6 ){
		dirtype= STRING;
		return true;
	}
	if( strncmp("entry",p+1,5)==0 &&  lgh2blank(p+1)==5){
		dirtype= ENTRY;
		return true;
	}
	if( strncmp("extern",p+1,6)==0 && lgh2blank(p+1)==6){
		dirtype= EXTERN;
		return true;
	}
	/* if no dir was found*/
	dirtype = no_dir;
	return false ;
}

/*--------------------- look4ins ------------------------------------------
look4ins: Returns true if p is a legitimate instruction name, else
it returns false.
---------------------------------------------------------------------------
*/
int look4ins(char *p) { //Check if magic number
	int i;
	for (i = 0; i<15; i++) {
		if (strncmp(instruction[i].word, p, 3) == 0 && lgh2blank(p) == 3) {
			inscode = i;
			return true;
		}
	}
	if (strncmp(instruction[15].word, p, 4) == 0 && lgh2blank(p) == 4) {
		inscode = 15;
		return true;
	}
	return false;
}


/*-------------------- lgh2blank ----------------------------------------
lghblank: Returns the length of the string up to "blank"
		  character.	 
*/
int lgh2blank(char *line){
	int i=0;
	while ( *line>32 && *line!=NewLine && *line!='\0'){
		line++;
		i++;
	}
	return i;
}



/*---------------- dir2value ---------------------------------
dir2value: Inserts value to label and updates the value of dc.
--------------------------------------------------------------
*/
int dir2value(char *line, int withLabel){
	int numberOfDigits=0 ;
	char *xlab;
	
	line=nextfield(line);
	
	if(withLabel){ 
		if(dirtype==DATA || dirtype == STRING){ 
			if(look4ext(LABEL) != NULL ){  
				error=labelAlreadyDefinedExternal;
				return false;
			}
			if(! install(LABEL,(dc+ic)) ){ /* insert label into the label symbol table.*/ //Why DC+IC and not just DC
				error = DefinedLabelAlready;		/* redifinition of label.*/
				return false;
			}
		}
	}
	
	line=skipspace(line);
	if( line == NULL ){
		error=expectingArgument;
		return false;
	}
	
	if(dirtype == DATA){
		while( line != NULL && *line!=NewLine && *line!='\0'){
			if(*line == '-' || *line == '+')
				line++;
			while( isdigit(*line) ){
				line++;
				numberOfDigits++;
			}
			dc++;
			line=skipspace(line);
			if( line==NULL || *(line+1)=='\0')
				return 0;
			if( *line != ',' || numberOfDigits==0 ){
				error=badArgument;
				return 0;
			}
			line++;
			line=skipspace(line);
			if( line==NULL)
				return 0;
			numberOfDigits=0;
		}
	}
	if(dirtype == STRING){
		if(line==NULL){
			error=badArgument;
			return 0;
		}
		if(*line!='"'){
			error=badArgument;
			return 0;
		}
	
		line++;

		if( *line == '"' ){
			error=EmptyQuotationMarks;
			return 0;
		}
		while(line!=NULL && *line != '"' && *line!=NewLine && *line!='\0'){
			/*count++;*/
			line++;
			dc++;
		}
		dc++;  /* length of the string plus 1 for \0*/
		if( line==NULL ){
			error=badArgument;
			return 0;
		}
		line++;
		line=skipspace(line);
		if(line==NULL)
			return 0;
		if( *line!=' ' && *line!='\t' && *line!=NewLine && *line!='\0'){
			error=badArgument;
			return 0;
		}
		dc++; /* for the line that marks the end of string.*/
	} 
	if(dirtype == ENTRY ){
		xlab=cpyarg(line,' ');
		ins2entry(xlab);
	}
	if (dirtype == EXTERN) {
		xlab = cpyarg(line, ' ');
		if (lookup(xlab)) {
			error = InternallabelAsexternal; /* already defined as a non ext label.*/
			return 0;
		}
		install2ext(xlab); /* put label in external symbole tabel.*/
	}
	return NotPossible;
}


/*------------------ gotocomma ------------------------------------------
goto2comma: Takes a string and return a pointer, pointing 
			to where the comma appears in the string. If 
			no comma in the string, it returns zero.
*/ //maybe instead of  recursive use the clear space?
char *go2comma(char *line){
	if(line==NULL || *line == '\0')
		return NULL;
	if(*line==',')
		return line;
	line++;
	return go2comma(line);
}

/*--------------------- addmeth -----------------------------------------
addmeth: returns the code number for each 
         operand.
---------------------------------------------------------- 
*/ ////////Addressing Modes////////////////////////////
int addmeth(char *line){
	int address=1; // setting a default even thou we'll overright it

	if(line == NULL )
		return no_operand;	

	if (*line == '#')
	{
		line++;
		if (*line == '-' || *line == '+')
			line++;
		if (!isnumber(line)) //Do we need to check if the rest is also number?
		{
			error = IllegalAddressing;
			return no_operand;
		}
		return InstantAddressing;
	}
	if (find_reg(line))
		return DirectRegisterAddressing;
	////////
	if (islabel(line))
		//fix the []
		return DirectAddressing;

	if (isInstantDynamic(line))
		return InstantDynamicAddressing;

	/* this is change
	
	if (islabel(line) == '[') // need to find it after label
		return InstantDynamicAddressing;
	
	*/
	if (islabel(line) == '[') // need to find it after label
		return InstantDynamicAddressing;
		
		// if no closing ]
		error= IllegalAddressing;
		
	return no_operand;
	
}

	
/*---------------- ins2value ---------------------------------------
ins2value: Inserting  a value to label and updating the value of ic. 
--------------------------------------------------------------------
*/
int ins2value(char *line, int lab_in_tab){
	int first=no_arg, second=no_arg;
	int code=inscode;
	
    line=nextfield(line);
	if( line == NULL && code <14 ){ /*commands without operator*/
		error=IllegalAddressing;
		return false;
	}
	if(lab_in_tab){ /* label exists.*/
		if(look4ext(LABEL) != NULL ){
			error=labelAlreadyDefinedExternal;
			return false;
		}
		if( install(LABEL,(ic+dc)) == 0 ){ /* put label in the label symbol table.   */
			error=DefinedLabelAlready;			/* redefinition of label.*/
			return false;
		}
	}

	/*-Instruction with no operands.-*/
	if( code == 14 || code == 15){
		ic++;
	}
	/*_____________________________*/
	
	/*-Instruction with one operand.-*/
	if(code >= 4 && code <= 13 && code != 6){
		second=addmeth(line);
		
		if (second == no_operand){
			error=IllegalAddressing;
			return false;
		}
		if( (code !=12) && (second == 2 || second == 0)){
			error=IllegalAddressing;
			return false;
		}
		ic++; /*for the command*/
		//if(second <3)
		//	ic++; /* for the argument*/
	}
	/*______________________________*/

	/*-Instruction with two operands.-*/
	if( (code >=0 && code<=3) || code == 6 ){
		first=addmeth(line);
		line=go2comma(line);
		if(line == NULL){
			error=IllegalAddressing;
			return false;
		}
		line++;
		if(line == NULL){
			error=IllegalAddressing;
			return false;
		}
		line=skipspace(line);
		if(line == NULL){
			error=IllegalAddressing;
			return false;
		}
		second=addmeth(line);
		if( (code==6) && first != 1){
			error=IllegalAddressing;
			return false;
			}
		if(code != 2 && (second == 2 || second == 0)){
			error=IllegalAddressing;
			return false;
		}
		ic++; /*for the command*/
		ic++; /* for first*/
		ic++; /* for second*/
		if (first == DirectRegisterAddressing && second == DirectRegisterAddressing)
			ic--; /* if both Registers we just need one word*/
		}
	return 0;
}
	
/*------------cont---------------------------
cont: To make notation easier.
---------------------------------------------*/
int cont(char *line){
	if(line==NULL)
		return false;
	if(*line != ' ' && *line != '\t'){
		if(*line != NewLine &&  *line !='\0' && line != NULL) // could be &&
			return true;
	}
	return false;
}


/*--------------- isnumber ----------------------
isnumber: Check to see whether a string is a positive 
	  number or not. Return 1 if the string is a 
	  positive number 0 otherwise.       
-------------------------------------------------
*/
int isnumber(char *line){
	if(line == NULL)
		return false;
	while( cont(line) && *line != ',' ){
		if( ! isdigit(*line) )
			return false;
		line++;
	}
	return true;
}

/*-------------------- islabel ----------------------
islabel: returns 1 if the syntax of the string compliance  with the syntax of label.
----------------------------------------------------
				
*/
int islabel(char *line){
	int i=0;
	if(line == NULL)
		return false;
	while( cont(line) && *line != ',' ){
		if(i==0 &&  !isalpha(*line) )
			return false;
		i++;
		if( !isdigit(*line)  &&  !isalpha(*line))
			return false;
		if(i == labelMax){
			error= labelTooLong;
			return false;
		}
		line++;
	}
	return true;
}
/*-------------------- isInstantDynamic----------------------
isInstantDynamic: returns 1 if the answer is true
----------------------------------------------------

*/
int isInstantDynamic(char *line) {
	char *e;
	int index;
	e = strchr(line, '[');
	//check if the sign '[' exist
	index = (int)(e - line);
	if (e <= 0) {
		return 0;
	}

	//check if before the sign is label
	else {
		char str2[80];
		strncpy(str2, line, index);
		str2[index] = '\0';   /* null character manually added */
		if (!islabel(str2))
			return 0;
		else {
			line = line + index;
			//check if there is the sign '-' after 1 or 2 digits
			if (*(line + 2) == '-') {
				if (!isdigit(*(line + 1)))
					return 0;
				else line = line + 3;
			}
			else {
				if (*(line + 3) == '-') {
					if ((*(line + 1) == '1') && (*(line + 2) == '0' || *(line + 2) == '1' || *(line + 2) == '2' || *(line + 2) == '3'))
						line = line + 4;
					else return 0;
				}
			}
			if (*(line + 1) == ']') {
				if (!isdigit(*(line)))
					return 0;
				else line = line + 2;
			}
			else {
				if (*(line + 2) == ']') {
					if ((*(line + 1) == '1') && (*(line + 2) == '0' || *(line + 2) == '1' || *(line + 2) == '2' || *(line + 2) == '3'))
						line = line + 3;
					else return 0;
				}
				else return 0;
			}
			return 1;
		}
	}
}
/*----------------------- find_reg ---------------------------------------------
find_reg: Returns 1, if register appears, 0 otherwise.
----------------------------------------------------------------
*/
int find_reg(char *p){
	int i;
	for(i=0;i<8;i++){
		if ( strncmp(reg[i].word, p ,2) == 0 ){
			regist=i;
			return 1 ;
		}
	}
	return 0 ;
}

/*-------------------------ignoreline----------------------------------
check if we can ignore this line
 * --------------------------------------------------------------------*/
int ignoreline(char *line){
	if(line == NULL)
		return true; /* ignore the line it doesn't exist.*/
	if(*line==';')  // can we delete the 2 lines since the while would do it anyway?
		return true; /* this is comment line*/
	/*Ignore the preleading spaces*/ 
	while(line!=NULL && (*line==' ' || *line == '\t') && *line!='\0' ){ //was <=32
		line++;
	if(*line==';')
		return true;
	}
	if(*line=='\0')
		return  true;
	line++; //why do we need it?
	if(*line=='\0')
		return  true;
	else 
		return false;
}
