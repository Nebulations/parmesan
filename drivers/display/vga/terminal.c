// A standard VGA screen is 80 characters wide and 25 characters high.
#define SCREEN_X 80
#define SCREEN_Y 25

static int CURSOR_X = 0;
static int CURSOR_Y = 0;

static volatile unsigned char *video = (unsigned char *)0xB8000;

// general
void terminal_init();
void clear_terminal();

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

void terminal_init() {
    // Initialize the terminal to the end user and disable the hardware
    // hardware cursor in favor of the software cursor.
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"((unsigned char) 0x0A), "Nd"((unsigned short) 0x3D4)
    );
    
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"((unsigned char) 0x20), "Nd"((unsigned short) 0x3D5)
    );

    clear_terminal();
}

void write_char(char c, unsigned int x, unsigned int y) {
    int offset = (y * 80 + x) * 2;
    video[offset] = c;
    video[offset + 1] = 0x0F;
}

char get_char(unsigned int x, unsigned int y) {
    return video[(y * 80 + x) * 2];
}

// void print_hex(unsigned int num) {
//     const char * hex = "0123456789ABCDEF";

//     print_char('0');
//     print_char('x');

//     for (int i = 7; i >= 0; i--) {
//         print_char(hex[(num >> (i * 4)) & 0xF]);
//     }
// }

void print_char(char c) {
    write_char(c, CURSOR_X, CURSOR_Y);
    set_cursor(CURSOR_X + 1, CURSOR_Y);
}

void print_int(int num) {
    char buffer[12];
    int length = 0;

    if (num == 0) {
        print_char('0');
        return;
    }

    while (num > 0) {
        int digit = num % 10;

        buffer[length] = digit + '0';
        length++;

        num /= 10;
    }

    while (length > 0) {
        length--;

        print_char(buffer[length]);
    }
}

void clear_terminal() {
    for (int x = 0; x < SCREEN_X; x++) {
        for (int y = 0; y < SCREEN_Y; y++) {
            write_char(' ', x, y);
        }
    }

    set_cursor(0, 0);
}

void set_cursor(int x, int y) {
    CURSOR_X = x;
    CURSOR_Y = y;

    if (CURSOR_Y > SCREEN_Y) {
        clear_terminal();
        set_cursor(0, 0);
    }
}

int get_cursor_x() { return CURSOR_X; }
int get_cursor_y() { return CURSOR_Y; }

void draw_cursor() { write_char('_', CURSOR_X, CURSOR_Y); }
void undraw_cursor() { write_char(' ', CURSOR_X, CURSOR_Y); }
