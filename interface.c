#include "interface.h"
WINDOW *out_win;
WINDOW *in_win;

void init_interface(){
  initscr();			// Start curses mode 	       
  cbreak();			// Line buffering disabled

  /*Creates input and output windows, with a input window of IN_HEIGHT*/
  in_win = create_newwin(IN_HEIGHT, COLS, LINES-IN_HEIGHT, 0);
  out_win = create_newwin(LINES-IN_HEIGHT, COLS, 0, 0);
  scrollok(out_win, TRUE);

  wprintw(in_win, "[%s]: ", username);
  wrefresh(in_win);
}

WINDOW *create_newwin(int height, int width, int starty, int startx){
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
	
  wrefresh(local_win);		/* Show that box 		*/
	
  return local_win;
}

//Printf like function that prints on the output window
void out_printf(const char *str, ...){
  char dest[BUFSIZE];
  va_list argptr;
  va_start(argptr, str);
  vsprintf(dest, str, argptr);
  va_end(argptr);
  wprintw(out_win, dest);
  wrefresh(out_win);
};

//scan fuctions, reads from the input window
void in_scan(char *str){
  wgetstr(in_win,str);
  wclear(in_win);
  wprintw(in_win, "[%s]: ", username);
  wrefresh(in_win);
}
