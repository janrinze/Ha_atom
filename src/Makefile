
CC		=	gcc
#CFLAGS		=	-c -O6
INCLUDE		=	/usr/X11/include/
CFLAGS		=	-Wall -I$(INCLUDE) -DGNU_C -O3 -mcpu=cortex-a15 -marm -fomit-frame-pointer 
LD		=	gcc
LDFLAGS		=
LDLIBS		=	-lX11 -lpthread

INCLUDES	=	system.h \
			cpu.h \
			atom.h \
			monitor.h \
			keyboard.h \
			file.h \
			screen.h

SRC		=	main.c \
			atom.c \
			cpu.c \
			monitor.c \
			screen.c \
			keyboard.c \
			file.c

OBJS		= 	$(SRC:.c=.o)

MAKEDEPEND      = makedepend

.SUFFIXES: .c 

.c.o:
	$(CC) $(CFLAGS) -c $<


all		: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LDLIBS) -o atom -flto

pack:
	tar -cvf atom.tar *.c *.h Makefile
	gzip atom.tar

depend: $(SRC)
	@echo -n "Doing a makedepend ..";$(MAKEDEPEND) -I$(INCLUDE) $(SRC);echo

# DO NOT DELETE THIS LINE -- make depend depends on it.

