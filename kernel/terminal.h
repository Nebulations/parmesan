#ifndef TERMINAL_H
#define TERMINAL_H

#define SCREEN_X 80
#define SCREEN_Y 25

// general
void clear_screen();

// stdio
void write_char(char c, unsigned int x, unsigned int y);
char get_char(unsigned int x, unsigned int y);
void print_char(char c);
void print_hex(unsigned int num);
void print(char *message);
void println(char * message);

// cursor
void set_cursor(int x, int y);
int get_cursor_x();
int get_cursor_y();
void draw_cursor();
void undraw_cursor();

void outb(unsigned short port, unsigned char value);
void disable_cursor();

#endif
