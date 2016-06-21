/*==================== MAIN ========================= */
/* The purpose of this program is to create an assembler progam. 
	 Specifications for this assignment can be found in the assignment brochure.
   
   Author: Stav & Gady
*/

#include "asm.h"
#include<stdlib.h>

int main(int argc, char *argv[]){
	int i;
	for(i=1;i<argc;i++){
		assemble(argv[i]);
		clean_tab();
		clean_ex();
		clean_en();
	}
	{
		char *a = "C:\\myDocs\\OpenU\\33485 C\\MAM\\14\\Proj\\Proj14\\Debug\\a";
		assemble(a);
	}
	printf("\n");
	return 0; 
}

