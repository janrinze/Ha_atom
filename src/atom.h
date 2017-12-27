#ifndef A800_INCLUDED
#define	A800_INCLUDED



#define RAM 0
#define ROM 1
#define HARDWARE 2
#define VIDEO 4

#define TITLE   "Acorn Atom Emulator, Version 0.1"


extern UBYTE memory[65536];

extern UBYTE attrib[65536];

#define GetByte(addr)       memory[addr] /* Atom_GetByte(addr) */
#define PutByte(addr,byte)  \
	if (attrib[addr] == RAM) memory[addr] = byte;  else \
	if (attrib[addr] != ROM) if (Atom_PutByte(addr,byte)) break;

void init_memory (void);
void SetMemory (int addr1, int addr2, int kind);
UBYTE Atom_GetByte(UWORD addr);
bool Atom_PutByte(UWORD addr, UBYTE byte);

#define Escape(x) 

#endif

