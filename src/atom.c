#include    <stdio.h>

#include	"system.h"
#include	"cpu.h"
#include    "atom.h"
#include "screen.h"


UBYTE memory[65536],
      attrib[65536];

void init_memory ()
{
	int	i;

	for (i=0;i<65536;i++)
	{
		memory[i] = 0;
		attrib[i] = RAM;
	}
}



void SetMemory (int addr1, int addr2, int kind)
{
  int	i;

  for (i=addr1;i<=addr2;i++)
    {
      attrib[i] = kind;
    }
}

static UBYTE video_val = 0;

bool Atom_PutByte (UWORD addr, UBYTE byte)
{   /* Always in hardware */

    if (attrib[addr] == VIDEO)
    {
        update_video(addr, byte);
    }
    else
    {
        if (addr == 0xb000)
        {   UBYTE video = byte & 0xf0;
            set_keyboard(byte & 0x0f);

            if (video != video_val)
            {
                 printf("Video mode %x %x\n", byte, byte >> 5);
                set_mode(byte & 0x10, byte >> 5);
                video_val = video;
            }
        }
        memory[addr] = byte;
    }
    return FALSE;
}

UBYTE Atom_GetByte (UWORD addr)
{
/***
    if (addr == 0xb001)
        printf("                      Read from %x = %x\n", addr, memory[addr]);
***/

    return memory[addr];
}

