#define SCREEN_X 80
#define SCREEN_Y 25

static int CURSOR_X = 0;
static int CURSOR_Y = 0;

static volatile unsigned char *video = (unsigned char *)0xB8000;

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

void write_char(char c, unsigned int x, unsigned int y) {
    int offset = (y * 80 + x) * 2;
    video[offset] = c;
    video[offset + 1] = 0x0F;
}

char get_char(unsigned int x, unsigned int y) {
    return video[(y * 80 + x) * 2];
}

void print(char * message) {
    for (int i = 0; message[i] != '\0'; i++) {
        if (i >= SCREEN_X) {
            set_cursor(0, CURSOR_Y + 1);
        }

        write_char(message[i], CURSOR_X, CURSOR_Y);

        set_cursor(CURSOR_X + 1, CURSOR_Y);
    }
}

void print_hex(unsigned int num) {
    const char * hex = "0123456789ABCDEF";

    print_char('0');
    print_char('x');

    for (int i = 7; i >= 0; i--) {
        print_char(hex[(num >> (i * 4)) & 0xF]);
    }
}

void print_char(char c) {
    write_char(c, CURSOR_X, CURSOR_Y);
    set_cursor(CURSOR_X + 1, CURSOR_Y);
    // draw_cursor();
}

void println(char * message) {
    if (CURSOR_X != 0) {
        CURSOR_X = 0;
        CURSOR_Y++;
    }

    print(message);
}

void clear_screen() {
    for (int x = 0; x < SCREEN_X; x++) {
        for (int y = 0; y < SCREEN_Y; y++) {
            write_char(' ', x, y);
        }
    }
}

void set_cursor(int x, int y) {
    // write_char(CURSOR_ORIGINAL, CURSOR_X, CURSOR_Y);
    // undraw_cursor();

    CURSOR_X = x;
    CURSOR_Y = y;

    if (CURSOR_Y > SCREEN_Y) {
        clear_screen();
        set_cursor(0, 0);
    }

    // CURSOR_ORIGINAL = get_char(x, y);
    // write_char('_', x, y);
}

int get_cursor_x() { return CURSOR_X; }
int get_cursor_y() { return CURSOR_Y; }

void draw_cursor() { write_char('_', CURSOR_X, CURSOR_Y); }
void undraw_cursor() { write_char(' ', CURSOR_X, CURSOR_Y); }

inline void outb(unsigned short port, unsigned char value) {
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

void disable_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}
