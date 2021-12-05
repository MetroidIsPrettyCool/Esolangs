#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define byte unsigned char
#define COUNTFROM 255

/*
  Concept:
  Simple to use outside of variables. Program has fixed 255 one byte unsigned array of memory.
  Every once and a while, according to some arbitrary triggers, the memory array shuffles randomly, and a few values are randomized (0 - 255 inclusive).
  This shuffle can be triggered on purpose, and is not so often as to make computation impossible.
  Incentivizing some kind of error-correcting memory system that can handle the shuffle is a priority.
  Have a static register to make at least one thing saty consistent, enabling recovery.
  User input should not be in a fixed place in memory.
  Use of the static register for most operations should be discouraged.

  What if instead the user has to place memory shuffles and the interpreter will refuse a program with insufficient shuffles?
  I like this idea.

  Memory is initialized to contain their location as their value (i.e. memory[203] = 203, memory [17] = 17)

  Instructions:

  _ isn't an instruction, instead indicating an argument

  Control flow

  { - Mark the start of a type 1 loop                                  0
  }__ - Jump to the start of loop if the variable isn't zero           1
  [__ - Jump to the end of loop if the variable isn't zero             2
  ] - Mark the end of a type 2 loop, shuffle mem                       3

  . - Exit the program                                                 4

  Arithmetic ops
  +__ - Add the static register to a variable                          5
  -__ - Subtract the static register from a variable                   6
  *__ - Multiply the static register with a variable                   7
  /__ - Divide a variable by the static register                       8
  %__ - Modulus, you get the point                                     9
  >__ - Take the stat reg and variable and store in the variable      16

  Memory

  #__ - Swap static reigster with the variable                        10
  ~__ - Copy static register into variable                            11

  ? - Read a character from the user and place it somewhere in memory 12
  '__ - Output the variable as a number                               13
  "__ - Output the variable as a character                            14


  $ - Trigger the memory shuffle                                      15

  @@ - Evaluates to the variable at the value of the static register

*/

struct Op {
  int id;
  int arg; // Points to the variable (either via the statreg or in memory) used as an argument
};         // 0-255 for the memory val, -1 for what the statreg points to

void nop     (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  *countdown++;
}

void t1loop  (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  if (memory [arg] != 0) {
    int nest = 1;
    while (nest != 0) {
      (*opPtr)--;
      if (*opPtr == -1) {
	printf("error, mismatched loop\n");
	printf("\n");
	*opPtr = -2;
	*countdown = 1;
	return;
      }
      if (operations [*opPtr].id == 0) nest--;
      if (operations [*opPtr].id == 1) nest++;
    }
  }
}

void t2strt  (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  if (memory [arg] == 0) {
    int nest = 1;
    while (nest != 0) {
      (*opPtr)++;
      if (*opPtr == opSize) {
	printf("error, mismatched loop\n");
	printf("\n");
	*opPtr = -2;
	*countdown = 1;
	return;
      }
      if (operations [*opPtr].id == 2) nest++;
      if (operations [*opPtr].id == 3) nest--;
    }
  }
}

void t2loop  (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  *countdown++; 
  int nest = 1;
  while (nest != 0) {
    (*opPtr)--;
    if (*opPtr == -1) {
      printf("error, mismatched loop\n");
      printf("\n");
      *opPtr = -2;
      *countdown = 1;
      return;
    }
    if (operations [*opPtr].id == 2) nest--;
    if (operations [*opPtr].id == 3) nest++;
  }
  (*opPtr)--;
}

void exitprg (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  *opPtr = -2;
  *countdown = 1;
}

void add     (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  memory [arg] += *statReg;
}

void sub     (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  memory [arg] -= *statReg;
}

void mul     (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  memory [arg] *= *statReg;
}

void dvd     (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  memory [arg] /= *statReg;
}

void mod     (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  memory [arg] %= *statReg;
}

void greater (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  memory [arg] = (memory [arg] > *statReg) ? memory [arg] : *statReg;
}

void swap    (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  byte tmp = memory [arg];
  memory [arg] = *statReg;
  *statReg = tmp;
}

void copy    (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  memory [arg] = *statReg;
}

void in      (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  memory [rand() % 256] = (byte)getchar();
}

void numout  (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  printf("%d", memory [arg]);
}

void charout (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  printf("%c", memory [arg]);
}

void shuffle (struct Op* operations, int opSize, byte* memory, byte* statReg, int* opPtr, int* countdown, byte arg) {
  *countdown = COUNTFROM;

  // Shuffle the array
  byte tmp;
  for (int j, i = 255; i > 0; i--) {
    j = rand() % (i + 1);
    tmp = memory [i];
    memory [i] = memory [j];
    memory [j] = tmp;
  }
}

char* charName (char in) {
  if (in ==   0) return "NUL";
  if (in ==   1) return "SOH";
  if (in ==   2) return "STX";
  if (in ==   3) return "ETX";
  if (in ==   4) return "EOT";
  if (in ==   5) return "ENQ";
  if (in ==   6) return "ACK";
  if (in ==   7) return "BEL";
  if (in ==   8) return "BS";
  if (in ==  12) return "FF";
  if (in ==  14) return "SO";
  if (in ==  15) return "SI";
  if (in ==  16) return "DLE";
  if (in ==  17) return "DC1";
  if (in ==  18) return "DC2";
  if (in ==  19) return "DC3";
  if (in ==  20) return "DC4";
  if (in ==  21) return "NAK";
  if (in ==  22) return "SYN";
  if (in ==  23) return "ETB";
  if (in ==  24) return "CAN";
  if (in ==  25) return "EM";
  if (in ==  26) return "SUB";
  if (in ==  27) return "ESC";
  if (in ==  28) return "FS";
  if (in ==  29) return "GS";
  if (in ==  30) return "RS";
  if (in ==  31) return "US";
  if (in == 127) return "DEL";
}

int getIdFromC (char in) {
  if (in == '{')  return  0;
  if (in == '}')  return  1;
  if (in == '[')  return  2;
  if (in == ']')  return  3;
  if (in == '.')  return  4;
  if (in == '+')  return  5;
  if (in == '-')  return  6;
  if (in == '*')  return  7;
  if (in == '/')  return  8;
  if (in == '%')  return  9;
  if (in == '#')  return 10;
  if (in == '~')  return 11;
  if (in == '?')  return 12;
  if (in == '\'') return 13;
  if (in == '"')  return 14;
  if (in == '$')  return 15;
  if (in == '>')  return 16;
  return -1;
}

int getArgFromC (char in1, char in2) {
  if (in1 == '@' && in2 == '@') return -1;

  int result;
  if      (in1 > 47 && in1 <  58) result = (in1 - 48) * 16;
  else if (in1 > 96 && in1 < 103) result = (in1 - 87) * 16;
  else return -10;

  if      (in2 > 47 && in2 <  58) result += (in2 - 48);
  else if (in2 > 96 && in2 < 103) result += (in2 - 87);
  else return -11;

  return result;
}

int main (int argc, char* argv []) {
  srand((unsigned) time(NULL)); // Seed rand
  FILE* inputStream;
  if (argc == 2) {
    inputStream = fopen(argv[1], "r");
    if (!inputStream) {
      printf("please pass only the name of a valid file\n");
      return 0;
    }
  }
  else {
    printf("please pass only the name of a valid file\n");
    return 0;
  }
  
  byte memory [256];
  byte statReg = 0; // Static register
  char* source; // Source code of the prgoram read in from stdin / the file
  int sourceSize;
  struct Op* operations; // Stores the opcodes etc. in an array for faster processing
  int opSize;
  int opPtr = 0; // Opcode pointer, prgoram exits when this is set to -1 by '.'
  int countdown = COUNTFROM; // Count down until memory should be shuffled
  
  // Read in the program
  int inChar;
  source = (char*)malloc(sizeof(char));
  sourceSize = 1;
  while ((inChar = fgetc(inputStream)) != EOF) {
    if (inChar != ' ' && inChar != '\t' && inChar != '\n' && inChar != '\v') { // Ignore whitespace
      source [sourceSize - 1] = inChar;
      sourceSize++;
      source = realloc(source, sizeof(char) * sourceSize);
    }
  }
  sourceSize--;
  
  // Convert the program into op codes
  operations = (struct Op*)malloc(sizeof(char));
  opSize = 1;
  for (int i = 0; i < sourceSize; i++) {
    operations [opSize - 1].id = getIdFromC(source [i]);
    if (operations [opSize - 1].id == -1) {
      printf("error, illegal character\n");
      return -1;
    }
    if (operations [opSize - 1].id !=  0 && operations [opSize - 1].id !=  3 && // If the opCode requires an argument
	operations [opSize - 1].id !=  4 && operations [opSize - 1].id != 12 &&
	operations [opSize - 1].id != 15) {
      i += 2;
      operations [opSize - 1].arg = getArgFromC(source [i - 1], source [i]);
      if (operations [opSize - 1].arg == -10) {
	printf("error, illegal character\n");
	return -1;
      }
      if (operations [opSize - 1].arg == -11) {
	printf("error, illegal character\n");
	return -1;
      }
    }
    opSize++;
    operations = realloc(operations, sizeof(struct Op) * opSize);
  }
  opSize--;
  
  free(source);
  
  // Init memory
  for (int i = 0; i != 256; i++) {
    memory [i] = i;
  }

  // Set up function lookup table
  //                   operations   ops  mem    strg  oPtr  cdwn  arg
  void (*funcLuT []) (struct Op*, int, byte*, byte*, int*, int*, byte) = {
    nop, t1loop, t2strt, t2loop, exitprg, add, sub, mul, dvd,
    mod, swap, copy, in, numout, charout, shuffle, greater};
  
  // Main loop
  while (opPtr != -1) {
    countdown--;
    (*funcLuT[operations [opPtr].id])(operations, opSize, memory, &statReg, &opPtr, &countdown, (operations [opPtr].arg == -1 ? statReg : operations [opPtr].arg));
    if (countdown == 0) {
      printf("error, program excessively crunchy\n");
      break;
    }
    opPtr++;
    if (opPtr == opSize) {
      printf("error, reached EOF\n");
      break;
    }
  }
  free(operations);
  
}
