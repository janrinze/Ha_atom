#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "system.h"
#include "atom.h"
#include "cpu.h"
#include "monitor.h"
#include "file.h"

void save_to_file(char *name)
{   FILE *f = fopen(name, "w");
    int p;

    if (f == NULL)
    {   printf("could not open `%s'\n", name);
        return;
    }
    p = memory[18] << 8;

    if (memory[p] != 13)
    {   printf("%x contains %x\n", p, memory[p]);
        return;
    }
    p++;
    printf("at %x\n", p);
    while (memory[p] != 0xff)
    {    int ln = memory[p] << 16 | memory[p + 1];

         printf("%5d", ln);
         fprintf(f, "%5d", ln);
         p += 2;

         while (memory[p] != 13)
         {   fputc(memory[p], stdout);
             fputc(memory[p], f);
             p++;
         }
         fputc('\n', stdout);
         fputc('\n', f);
         p++;
    }
    fclose(f);
}

void load_from_bin_file(FILE *f, int p)
{   char ch = (char)fgetc(f);

    while (!feof(f))
    {    memory[p++] = ch;
         ch = (char)fgetc(f);
    }
}
    
void read_line(FILE *f, char buffer[])
{   int i = 0;
    char ch = (char)fgetc(f);

    while (!feof(f) && ch != '\n')
    {   if (i < 299)
            buffer[i++] = ch;
        ch = (char)fgetc(f);
    }
    buffer[i] = '\0';
}

void load_from_html_file(FILE *f)
{   char buffer[300];
    int p;
    bool incode = FALSE,
         inside = FALSE;

    read_line(f, buffer);

    while (buffer[0] != '\0' || !feof(f))
    {   int i = 0, ln = 0;

        if (strncmp(buffer, "<!--at ", 7) == 0)
        {   if (inside)
                memory[p] = 0xff;
            sscanf(buffer + 7, "%x", &p);
            memory[18] = p >> 8;

            p = memory[18] << 8;
            printf("at %x\n", p);

            memory[p++] = 13;
            inside = TRUE;
        }
        else if (strncmp(buffer, "<PRE>", 5) == 0)
            incode = TRUE;
        else if (strncmp(buffer, "</PRE>", 6) == 0)
            incode = FALSE;
        else if (strncmp(buffer, "<!--end", 7) == 0)
        {   if (inside)
                memory[p] = 0xff;
            inside = FALSE;
        }
        else if (inside && incode)
        {   while (buffer[i] == ' ')
                i++;
            while (isdigit(buffer[i]))
                ln = 10 * ln + buffer[i++] - '0';
    
            printf("%5d%s\n", ln, buffer + i);

            memory[p] = (ln >> 8) & 0xff;
            memory[p + 1] = ln & 0xff;
            p += 2;

            while (buffer[i] != '\0')
                if (!strncmp(buffer + i, "&amp;", 5))
                {   memory[p++] = '&';
                    i += 5;
                }
                else if (!strncmp(buffer + i, "&lt;", 4))
                {   memory[p++] = '<';
                    i += 4;
                }
                else if (!strncmp(buffer + i, "&gt;", 4))
                {   memory[p++] = '>';
                    i += 4;
                }
                else
                    memory[p++] = buffer[i++];
            memory[p++] = 13;
        }

        read_line(f, buffer);
    }
    if (inside)
        memory[p] = 0xff;
}
     

void load_from_txt_file(FILE *f)
{   char buffer[300];
    int p;
    bool inside = FALSE;

    read_line(f, buffer);

    if (strncmp(buffer, "at ", 3) != 0)
    {   p = memory[18] << 8;
        printf("at %x\n", p);

        memory[p++] = 13;
        inside = TRUE;
    }

    while (buffer[0] != '\0')
    {   int i = 0, ln = 0;

        if (strncmp(buffer, "at ", 3) == 0)
        {   if (inside)
                memory[p] = 0xff;
            sscanf(buffer + 3, "%x", &p);
            memory[18] = p >> 8;

            p = memory[18] << 8;
            printf("at %x\n", p);

            memory[p++] = 13;
            inside = TRUE;
        }
        else
        {   while (buffer[i] == ' ')
                i++;
            while (isdigit(buffer[i]))
                ln = 10 * ln + buffer[i++] - '0';
    
            printf("%5d%s\n", ln, buffer + i);

            memory[p] = (ln >> 8) & 0xff;
            memory[p + 1] = ln & 0xff;
            p += 2;

            while (buffer[i] != '\0')
                memory[p++] = buffer[i++];
            memory[p++] = 13;
        }

        read_line(f, buffer);
    }
    if (inside)
        memory[p] = 0xff;
}
     

void load_from_file(char *name)
{   FILE *f = fopen(name, "r");
    int l = strlen(name);

    if (f == NULL)
    {   printf("could not open `%s'\n", name);
        return;
    }

    if (l > 4 && !strcmp(name + l - 4, ".bin"))
        load_from_bin_file(f, memory[18] << 8);
    else if (l > 5 && !strcmp(name + l - 5, ".html"))
        load_from_html_file(f);
    else
        load_from_txt_file(f);

    fclose(f);
}
 
