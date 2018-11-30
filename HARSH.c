#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUTSIZE 500000
#define STACKSIZE 500000

int main (void)  {
  char inputString [INPUTSIZE]; // This string will hold our program
  int stack [STACKSIZE] = {0};
  int tmpStack [STACKSIZE] = {0};
  int stackSize;
  int i; // Counter
  int c; // Counter for 'r'
  int tmp; // Stores values for r
  int acc; // Accumulator
  int doexit = 0;
  int tmpAcc;
  int fileScan = -1;
  char filename [1000];
  FILE *fp;
  char yn = '1'; // Y/N
  printf("EXECUTE FROM FILE? [Y/N]");
  while (fileScan == -1) {
    scanf("%c", &yn);
    if (yn == 'n' || yn == 'N')  {
      i++;
      fileScan = 0;
    }
    if (yn == 'y' || yn == 'Y')  {
      fileScan = 1;
      printf("FILENAME: ");
      scanf("%s", filename);
      fp = fopen(filename, "r");
    }
  }
  yn = '1';
  doexit = 0;
  for (;;)  {
  imsorry:
    stackSize = -1;
    acc = 0;
    tmpAcc = 0;
    yn = '1';
    if (fileScan == 0)  {
      printf(">>> ");
      scanf("%s", inputString); // Will also support fscanf in version 2.0
    }
    else  {
      fscanf(fp, "%s", inputString);
    }
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
	acc = stack [stackSize];
	stackSize--;
	break;
      case 'r':
	tmp = stack [stackSize];
	for (c = 0; c != STACKSIZE; c++)  {
	  tmpStack [c + 1] = stack [c];
	}
	for (c = 0; c != STACKSIZE; c++)  {
	  stack [c] = tmpStack [c];
	}
	stack [0] = tmp;
	break;
      case 'e':
	printf("\n");
	if (fileScan == 1)  {
	  fclose(fp);
	  return 0;
	}
	goto imsorry;
      case 'b':
	tmpAcc = acc;
	while (tmpAcc - 1 > i)  {
	  tmpAcc -= i;
	}
	i -= tmpAcc + 1;
	break;
      case 'd':
	acc *= 2;
	break;
      case 'n':
	printf("%i", acc);
      case ' ':
      case '\t':
	break;
      case '\0':
	printf("ERR, MISSING E\n");
	if (fileScan == 1)  {
	  fclose(fp);
	  return 0;
	}
	goto imsorry;
      default:
	printf("ERR, ILLEGAL CHARACTER: %c\n", inputString [i]);
	if (fileScan == 1)  {
	  fclose(fp);
	  return 0;
	}
	goto imsorry;
      }
    }
  }
}
