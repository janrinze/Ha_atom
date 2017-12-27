#include	<stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include	"system.h"
#include	"cpu.h"
#include    "atom.h"
#include    "keyboard.h"


static int  control = 0;
static int  shift = 0;
static int  alt = 0;
static int  keys_pressed = 0;
static int  shifted_keys = 0;
static UBYTE keys[16];


void init_keyboard (void)
{   int i;

    for (i = 0; i < 16; i++)
        keys[i] = 0x3f;

    control = 0;
    shift = 0;
    alt = 0;
    keys_pressed = 0;
    shifted_keys = 0;
}

void handle_key (XKeyEvent *key_event, bool pressed)
{
    KeySym  keysym;
    char    buffer[128];
    UWORD   keycode;
    int keyboard_status;

    XLookupString (key_event, buffer, 128, &keysym, &keyboard_status);

#define KEY(S,R,P) keycode = (S << 10) | (R << 6) | (1 << P)
#define SHIFT(X) ((X >> 10) & 1)
#define ROW(X)   ((X >> 6) & 15)
#define POS(X)   (X & 63)

#ifdef DEBUG
    printf("keysym : %ld, %d\n", keysym, pressed);
#endif

    keycode = 0xFFFF;

    switch (keysym)
    {
        case XK_Shift_L :
        case XK_Shift_R :
            if (pressed)
                shift=1;
            else
                shift=0;
            break;
        case XK_Control_L :
        case XK_Control_R :
            if (pressed)
                control = 1;
            else
                control = 0;
            break;
        case XK_Caps_Lock :
            /* ignore */
            break;
        case XK_Shift_Lock :
            /* ignore */
            break;
        case XK_Alt_L :
        case XK_Alt_R :
            if (pressed)
                alt = 1;
            else
                alt = 0;
            break;
        case XK_F1 : nCPU_Reset(); break;
        case XK_F5 :
        case XK_F2 :
        case XK_F3 :
        case XK_F4 :
        case XK_F6 :
        case XK_F7 :
            break;
        case XK_KP_0 :
        case XK_KP_1 :
        case XK_KP_2 :
        case XK_KP_3 :
        case XK_KP_4 :
        case XK_KP_5 :
        case XK_KP_6 :
        case XK_KP_7 :
        case XK_KP_8 :
        case XK_KP_9 :                    break;
        case XK_Home :                    break;
        case XK_Insert :                  break;
        case 0xff08:
        case XK_Delete :      KEY(0,4,1); break;
        case XK_Left :        KEY(1,3,0); break;
        case XK_Up :          KEY(0,2,0); break;
        case XK_Right :       KEY(0,3,0); break;
        case XK_Down :        KEY(1,2,0); break;
        case XK_Escape :      KEY(0,0,5); break;
        case XK_Tab :                     break;
        case XK_exclam :      KEY(1,2,1); break;
        case XK_quotedbl :    KEY(1,1,1); break;
        case XK_numbersign :  KEY(1,0,1); break;
        case XK_dollar :      KEY(1,9,2); break;
        case XK_percent :     KEY(1,8,2); break;
        case XK_ampersand :   KEY(1,7,2); break;
        case XK_quoteright :  KEY(1,6,2); break;
        case XK_at :          KEY(0,7,3); break;
        case XK_parenleft :   KEY(1,5,2); break;
        case XK_parenright :  KEY(1,4,2); break;
        case XK_less :        KEY(1,1,2); break;
        case XK_greater :     KEY(1,9,3); break;
        case XK_equal :       KEY(1,0,2); break;
        case XK_question :    KEY(1,8,3); break;
        case XK_minus :       KEY(0,0,2); break;
        case XK_plus :        KEY(1,2,2); break;
        case XK_asterisk :    KEY(1,3,2); break;
        case XK_slash :       KEY(0,8,3); break;
        case XK_colon :       KEY(0,3,2); break;
        case XK_semicolon :   KEY(0,2,2); break;
        case XK_comma :       KEY(0,1,2); break;
        case XK_period :      KEY(0,9,3); break;
        case XK_underscore :  KEY(1,0,2);            break;
        case XK_bracketleft : KEY(0,8,0); break;
        case XK_bracketright :KEY(0,6,0); break;
        case XK_asciicircum : KEY(0,5,0); break;
        case XK_backslash :   KEY(0,7,0); break;
        case XK_bar :                     break;
        case XK_0 :           KEY(0,3,1); break;
        case XK_1 :           KEY(0,2,1); break;
        case XK_2 :           KEY(0,1,1); break;
        case XK_3 :           KEY(0,0,1); break;
        case XK_4 :           KEY(0,9,2); break;
        case XK_5 :           KEY(0,8,2); break;
        case XK_6 :           KEY(0,7,2); break;
        case XK_7 :           KEY(0,6,2); break;
        case XK_8 :           KEY(0,5,2); break;
        case XK_9 :           KEY(0,4,2); break;
        case XK_space :       KEY(0,9,0); break; 
        case XK_Return :      KEY(0,6,1); break;
        case XK_a :           KEY(0,6,3); break;
        case XK_A :           KEY(1,6,3); break;
        case XK_b :           KEY(0,5,3); break;
        case XK_B :           KEY(1,5,3); break;
        case XK_c :           KEY(0,4,3); break;
        case XK_C :           KEY(1,4,3); break;
        case XK_d :           KEY(0,3,3); break;
        case XK_D :           KEY(1,3,3); break;
        case XK_e :           KEY(0,2,3); break;
        case XK_E :           KEY(1,2,3); break;
        case XK_f :           KEY(0,1,3); break;
        case XK_F :           KEY(1,1,3); break;
        case XK_g :           KEY(0,0,3); break;
        case XK_G :           KEY(1,0,3); break;
        case XK_h :           KEY(0,9,4); break;
        case XK_H :           KEY(1,9,4); break;
        case XK_i :           KEY(0,8,4); break;
        case XK_I :           KEY(1,8,4); break;
        case XK_j :           KEY(0,7,4); break;
        case XK_J :           KEY(1,7,4); break;
        case XK_k :           KEY(0,6,4); break;
        case XK_K :           KEY(1,6,4); break;
        case XK_l :           KEY(0,5,4); break;
        case XK_L :           KEY(1,5,4); break;
        case XK_m :           KEY(0,4,4); break;
        case XK_M :           KEY(1,4,4); break;
        case XK_n :           KEY(0,3,4); break;
        case XK_N :           KEY(1,3,4); break;
        case XK_o :           KEY(0,2,4); break;
        case XK_O :           KEY(1,2,4); break;
        case XK_p :           KEY(0,1,4); break;
        case XK_P :           KEY(1,1,4); break;
        case XK_q :           KEY(0,0,4); break;
        case XK_Q :           KEY(1,0,4); break;
        case XK_r :           KEY(0,9,5); break;
        case XK_R :           KEY(1,9,5); break;
        case XK_s :           KEY(0,8,5); break;
        case XK_S :           KEY(1,8,5); break;
        case XK_t :           KEY(0,7,5); break;
        case XK_T :           KEY(1,7,5); break;
        case XK_u :           KEY(0,6,5); break;
        case XK_U :           KEY(1,6,5); break;
        case XK_v :           KEY(0,5,5); break;
        case XK_V :           KEY(1,5,5); break;
        case XK_w :           KEY(0,4,5); break;
        case XK_W :           KEY(1,4,5); break;
        case XK_x :           KEY(0,3,5); break;
        case XK_X :           KEY(1,3,5); break;
        case XK_y :           KEY(0,2,5); break;
        case XK_Y :           KEY(1,2,5); break;
        case XK_z :           KEY(0,1,5); break;
        case XK_Z :           KEY(1,1,5); break;
        default :
            keycode = 0xFFFF;
            printf ("Received unhandled key %x\n", (int)keysym);
            break;
    }


    if (keycode != 0xFFFF)
        if (pressed)
        {   keys_pressed++;
            if (SHIFT(keycode))
                shifted_keys=1;
            keys[ROW(keycode)] &= ~POS(keycode);
        }
        else
        {   keys_pressed--;
            if (SHIFT(keycode))
                shifted_keys=0;
            keys[ROW(keycode)] |= POS(keycode);
        }
    memory[ 0xb001 ] = keys[ memory[0xb000] & 0x0f ]
                     | (  (keys_pressed ? shifted_keys : shift)
                        ? 0 : 0x80)
                     | (control ? 0 : 0x40);
    memory[ 0xb002 ] = (memory[ 0xb002 ] & 0xbf)
                     | (alt ? 0 : 0x40 );
#ifdef DEBUG
    {   int i;
        printf("%04x r:%d p:%d s:%d %02x->%02x  ", keysym, 
               ROW(keycode), POS(keycode), SHIFT(keycode),
               memory[ 0xb000 ], memory[ 0xb001 ]);
        for (i = 0; i < 10; i++)
            printf("%02x ", keys[i]);
        printf("\n");
    }
#endif
}

void set_keyboard (UBYTE row)
{
    memory[ 0xb001 ] = keys[row] 
                     | (  (keys_pressed ? shifted_keys : shift) 
                        ? 0 : 0x80) 
                     | (control ? 0 : 0x40);
    memory[ 0xb002 ] = (memory[ 0xb002 ] & 0xbf)
                     | (alt ? 0 : 0x40 );
}
