#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <fcntl.h>
#include    <ctype.h>
#include    <signal.h>
#include    <time.h>

#define FALSE   0
#define TRUE    1

#include    "system.h"
#include    "cpu.h"
#include    "atom.h"
#include    "screen.h"
#include    "keyboard.h"
#include    "monitor.h"

int    countdown;

// forward declarations
void Atom_OS (char *filename);
void load_image (char *filename, int addr);
void load_mem (char *filename);
void save_mem (char *filename);


void sigint_handler ()
{
    printf ("*** break ***\n");

    if (monitor () == 1)
    {
        signal (SIGINT, sigint_handler);
        return;
    }

#ifdef REDIRECT_STDERR
    fclose (stderr);
#endif

    exit_screen ();

    exit (0);
}

int main (int argc, char **argv)
{
    int     error;
    char * persistentmem="persistent.ram";

#ifdef REDIRECT_STDERR
    freopen ("error.log", "w", stderr);
#endif

    printf ("Acorn Atom Emulator\n");

    init_memory ();
    init_screen ();
    init_keyboard ();

    signal (SIGINT, sigint_handler);

    error = FALSE;

    /********
    for (i=1;i<argc;i++)
    {
        if (*argv[i] == '-')
        {
        }
        else
            error = TRUE;
    }
    ********/

    if (error)
    {
        printf ("Usage: %s\n", argv[0]);
        exit (1);
    }
    if (argc>1) persistentmem=argv[1];

    Atom_OS (persistentmem);

    exit_screen();

    return 0;
}
#define PH_ONE 30000
#define PH_TWO (9*PH_ONE)

void Atom_OS (char * persistentmem)
{
    time_t start;
    double mips=0.0;
    instcount=0;
    int loopflag=1;
    pthread_t * tid;
    int err;
    
    load_image ("akernel.rom", 0xF000);
    load_image ("abasic.rom", 0xC000);
    load_image ("afloat.rom", 0xD000);
    load_image("pcharme.rom", 0xA000);
    if (persistentmem) load_mem(persistentmem);

    attrib[0xb000] = HARDWARE;
    attrib[0xb002] = HARDWARE;
    memory[0xb002] = 0x40;

    CPU_Reset ();

    start=time(NULL);
    
    err = pthread_create(&tid, NULL, &GO, (void*)PH_TWO);

    while (loopflag)
    {
      int t;
      time_t now;
      double dt;

      int i;
      update_screen();

      memory[0xb002] = (memory[0xb002] & 0x7f);
      usleep(10000);
      memory[0xb002] = (memory[0xb002] & 0x7f) | 0x80;
      loopflag=atom_hardware();
      if (loopflag==0) break;
      usleep(10000);

      now=time(NULL);
      dt=difftime(now,start);
      if (dt>=1.0)
      {
          dt=dt*1000000.0;
          mips=0.9*mips+0.1*(double)instcount/dt;
          printf(" MIPS: %f \n " ,mips);
          start=now;
          instcount=0;
      }
   }
   if (persistentmem)
      save_mem(persistentmem);
}


void load_image (char *filename, int addr)
{   int start_addr = addr;
    int    fd;

    fd = open (filename, O_RDONLY);
    if (fd != -1)
    {
        UBYTE b;

        while (read (fd, &b, 1) == 1)
            memory[addr++] = b;

        close (fd);
        SetMemory(start_addr, addr - 1, ROM);
    }
    else
        printf("load image: could not find %s\n", filename);
}

void load_mem (char *filename)
{
  int fd=open(filename,O_RDONLY);
  printf("trying to load %s\n",filename);
  if (fd!=-1)
    read(fd,memory,0xA000);
}

void save_mem (char *filename)
{
  int fd=open(filename,O_WRONLY|O_CREAT ,0600);
  printf("trying to save %s\n",filename);
  if (fd!=-1)
    write(fd,memory,0xA000);
}

