#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUTSIZE 500000
#define STACKSIZE 500000

int main (void)  {
	char inputString[INPUTSIZE]; // This string will hold our program
	int stack [STACKSIZE];
	int stackSize = 0;
	int i; // Counter
	int c; // Counter for 'r'
	int tmp; // Stores values for r
	int acc; // Accumulator
	int doexit = 0;
	char yn; // Y/N
	for (;;)  {
	    imsorry:
    	acc = 0;
    	yn;
    	scanf("%s", inputString); // Will also support fscanf in version 2.0
    	for (i = 0; i != INPUTSIZE; i++)  {
    		switch (inputString [i])  {
    			case '+':
    				acc++;
    				break;
    			case '/':
    				if (acc == 30)  {
    					i++;
    				}
    				break;
    			case '?':
    			    printf("EXECUTE \"%c\"? [Y/N]", inputString [i + 1]);
    				while (doexit != 1) {
    					scanf("%c", &yn);
    					if (yn == 'n' || yn == 'N')  {
    						i++;
    						doexit = 1;
    					}
    					if (yn == 'y' || yn == 'Y')  {
    					    doexit = 1;
    					}
    				}
    				doexit = 0;
    				yn = '1';
    				break;
    			case '#':
    				printf("%c", acc);
    				break;
    			case '0':
    				acc = 0;
    				break;
    			case '>':
    				stackSize++;
    				stack [stackSize] = acc;
    				break;
    			case '<':
    				acc = stack[stackSize];
    				stackSize--;
    				break;
    			case 'r':
    				tmp = stack [stackSize];
    				for (c = 0; c != stackSize - 1; c++)  {
    					stack [i + 1] = stack [i];
    				}
    				stack [0] = tmp;
    			case 'e':
    			    printf("\n");
    				goto imsorry;
    			case ' ':
    			case '\t':
    				break;
    			case '\0':
    				printf("ERR, MISSING E\n");
    				goto imsorry;
    			default:
    				printf("ERR, ILLEGAL CHARACTER: %c\n", inputString [i]);
    				goto imsorry;
    		}
    	}
	}
}