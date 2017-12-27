#include	<X11/Xlib.h>
#include        <X11/keysym.h>

void init_keyboard (void);
void handle_key (XKeyEvent *key_event, bool pressed);
void set_keyboard (UBYTE row);



