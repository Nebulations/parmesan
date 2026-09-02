#ifndef TERMINAL_H
#define TERMINAL_H

#define SCREEN_X 80
#define SCREEN_Y 25

// general
void terminal_init();
void clear_terminal();

// stdio
void write_char(char c, unsigned int x, unsigned int y);
char get_char(unsigned int x, unsigned int y);
void print_char(char c);
void print_int(int num);
void print(char * message);

// cursor
void set_cursor(int x, int y);
int get_cursor_x();
int get_cursor_y();
void draw_cursor();
void undraw_cursor();

#endif
