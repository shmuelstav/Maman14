/* =================== er.c =========================*/ 

/* 
The purpose of this file is to handle possible error[s] on "as" file.
*/
#define _CRT_NONSTDC_NO_WARNINGS
#include "asm.h"

/*---------------- WRITE_ERROR ---------------------
write_error: This function uses the global variable 
             "error" to determine whether an error 
			 should be written to the screen. 
*/
void write_error(int ln){

	switch (error){
		case SyntaxError:
			printf("line %d: Syntax error. \n",ln);
			break ;
		case labelAsInstruction:
			printf("line %d: label cannot be an instruction.\n", ln);
			break ;
		case labelAsDirective:
			printf("line %d: label cannot be a directive. \n", ln);
			break;
		case DefinedLabelAlready:
			printf("line %d: Previously defined label. \n",ln);
			break;
		case labelTooLong:
			printf("line %d: label contains more than " str(labelMax) "characters. \n",ln); // How to Expand labelMax inside quoted string
			break;
		case UnrecognizedDirective:
			printf("line %d: Unrecognized directive. \n",ln);
			break;
		case unrecognizedInsruction:
			printf("line %d: unrecognized insruction. \n",ln);
			break;
		case firstCharlabelNotletter:
			printf("line %d: The first character of the label must be a letter. \n", ln);
			break;
		case line2Long:
			printf("line %d: To many characters for one line. The number of characters per line must be less than 80.\n",ln	);
			break;
		case badArgument:
			printf("line %d: bad argument[s]\n",ln);
			break;
		case labelOnly:
			printf("Line %d: line should contain more than a label\n",ln);
			break;
		case IllegalAddressing:
			printf("Line %d: Illegal addressing, check operands. \n",ln);
			break;
		case labelAsRegister:
			printf("Line %d: label cannot be a register \n",ln);
			break;
		case EmptyQuotationMarks:
            printf("Line %d:no character[s] between the quotation marks ",ln);
            break;
		case labelNotDefined:
			printf("Line %d: label was not defined in the file \n",ln);
			break;
		case labelAlreadyDefinedExternal:
             printf("Line %d: label already defined as external \n",ln);
             break;
        case InternallabelAsexternal:
             printf("Line %d: cannot define label as external. ",ln);
             printf("label already defined as internal.\n");
             break;
		case expectingArgument:
			printf("Line %d: Error: expecting argument...\n",ln);
			break;
	}

}


/*---------------- write_error2 ---------------------------------------
write_error2: This function uses the global variable 
             "error" to determine whether an error 
	         should be written to the screen.
--------------------------------------------------------
*/
int write_error2(int ln, char *line){
	
	switch( error ){
		case 1:
			printf("Line %d: unrecognized label \n",ln);
			break;
		case 2:
			printf("Line %d: Error: expecting argument... \n",ln);
			break;
		case 3:
			printf("Line %d: To few actual argument[s] \n",ln);
			break;
		case 4:
			printf("Line %d: misseing quotation[s] character on .string directive",ln);
			break;
		case 5:
			line=skipspace(line);
			line=nextfield(line);
			if( line == NULL )
				return 0;
			printf("Line %d: file has no internal label name \"%s\" ",ln,line);
			break;
	
	}
	return NotPossible; 
}

