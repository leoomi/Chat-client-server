#include <ncurses.h>
#include <stdarg.h>
#define IN_HEIGHT 3
#define BUFSIZE 1024
#define NAME_SIZE 15

char username[NAME_SIZE];

void init_interface();
WINDOW *create_newwin(int height, int width, int starty, int startx);
void out_printf(const char *str, ...);
void in_scan(char *str);
