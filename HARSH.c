#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define INPUTSIZE 500000
#define STACKSIZE 500000

int main (int argc, char *argv[])  {
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
  char fileUse [1000];
  if (argc == 1 || argc > 3)  {
    printf("First argument either -t or -T, or -f or -F. -T or -t will launch terminal mode, no additional arguments needed.\n-F or -f will load from a file, type space and then the filename afterwards. \nTyping the file extension (.hrs) will cause the interpreter to fail, so please don't. It adds that for you.\n");
    return -1;
  }
  while (fileScan == -1) {
    strcpy(fileUse, argv[1]);
    if (strcmp(fileUse,"-t") == 0 || strcmp(fileUse,"-T") == 0)  {
      if (argc == 3)  {
	printf("First argument either -t or -T, or -f or -F. -T or -t will launch terminal mode, no additional arguments needed.\n-F or -f will load from a file, type space and then the filename afterwards. \nTyping the file extension (.hrs) will cause the interpreter to fail, so please don't. It adds that for you.\n");
	return -1;
      }
      i++;
      fileScan = 0;
      break;
    }
    else if (strcmp(fileUse,"-f") == 0 || strcmp(fileUse,"-F") == 0)  {
      if (argc == 2)  {
	printf("First argument either -t or -T, or -f or -F. -T or -t will launch terminal mode, no additional arguments needed.\n-F or -f will load from a file, type space and then the filename afterwards. \nTyping the file extension (.hrs) will cause the interpreter to fail, so please don't. It adds that for you.\n");
	return -1;
      }
      fileScan = 1;
      strcpy(filename, argv[2]);
      strcat(filename, ".hrs");
      fp = fopen(filename, "r");
      if (!fp)  {
	printf("ILLEGAL FILENAME, PLEASE TRY AGAIN.\n");
	return -1;
      }
      break;
    }
    else {
      printf("First argument either -t or -T, or -f or -F. -T or -t will launch terminal mode, no additional arguments needed.\n-F or -f will load from a file, type space and then the filename afterwards. \nTyping the file extension (.hrs) will cause the interpreter to fail, so please don't. It adds that for you.\n");
    return -1;
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
      fgets(inputString, INPUTSIZE, stdin);
      if (strcmp(inputString, "exit\n") == 0)  {
	return 0;
      }
    }
    else  {
      fgets(inputString, INPUTSIZE, fp);
    }
    for (i = 0; i != INPUTSIZE; i++)  {
      switch (inputString [i])  {
      case 'a':
	acc++;
	break;
      case 'h':
	if (acc == 30)  {
	  i++;
	}
	break;
      case 'q':
	printf("EXECUTE \"%c\"? [Y/N]: ", inputString [i + 1]);
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
      case 'c':
	printf("%c", acc);
	break;
      case 'o':
	acc = 0;
	break;
      case 'u':
	stackSize++;
	stack [stackSize] = acc;
	break;
      case 'p':
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
	break;
      case ' ':
      case '\t':
	break;
      case 'z':
	switch (acc)  {
	case 1:
	  acc++;
	  break;
	case 7:
	  if (acc == 30)  {
	    i++;
	  }
	  break;
	case 8:
	  printf("EXECUTE \"%c\"? [Y/N]: ", inputString [i + 1]);
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
	case 10:
	  printf("%c", acc);
	  break;
	case 3:
	  acc = 0;
	  break;
	case 4:
	  stackSize++;
	  stack [stackSize] = acc;
	  break;
	case 5:
	  acc = stack [stackSize];
	  stackSize--;
	  break;
	case 6:
	  tmp = stack [stackSize];
	  for (c = 0; c != STACKSIZE; c++)  {
	    tmpStack [c + 1] = stack [c];
	  }
	  for (c = 0; c != STACKSIZE; c++)  {
	    stack [c] = tmpStack [c];
	  }
	  stack [0] = tmp;
	  break;
	case 12:
	  printf("\n");
	  if (fileScan == 1)  {
	    fclose(fp);
	    return 0;
	  }
	  goto imsorry;
	case 9:
	  tmpAcc = acc;
	  while (tmpAcc - 1 > i)  {
	    tmpAcc -= i;
	  }
	  i -= tmpAcc + 1;
	  break;
	case 2:
	  acc *= 2;
	  break;
	case 11:
	  printf("%i", acc);
	  break;
	}
	break;
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
