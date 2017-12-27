#ifndef __CPU_INCLUDED__
#define __CPU_INCLUDED__


typedef struct CPU_Status
{
  UWORD PC;
  UBYTE A;
  UBYTE S;
  UBYTE X;
  UBYTE Y;
  struct Flags
    {
      unsigned int N : 1;
      unsigned int V : 1;
      unsigned int B : 1;
      unsigned int D : 1;
      unsigned int I : 1;
      unsigned int Z : 1;
      unsigned int C : 1;
    } flag;
} CPU_Status;

void CPU_Reset (void);
void CPU_GetStatus (CPU_Status *cpu_status);
void CPU_PutStatus (CPU_Status *cpu_status);
void NMI (void);
void GO (int cycles);

extern volatile int IRQ;
extern volatile unsigned int instcount;


#endif
