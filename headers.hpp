#ifndef MAUH_HEADERS
#define MAUH_HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <I86.h>
#include <dos.h>
#include <conio.h>
#include <malloc.h>
#include <time.h>
#include <stdarg.h>

//#define DEBUG

class RC4 {
    public:
    RC4(char *seed, int len);
    unsigned int rand();
    unsigned char rand8();
    
    private:
    unsigned char state[256];
    unsigned int x,y;
};

#define SEEDSIZE 32
char *init_rc4();

class VGA256Term {
    public:
    
    VGA256Term();
    void draw_term();
    void clear_screen(unsigned char color);
    void printf_term(char *s, ...);
    void printchar_term(char c);
    void move_cursor_offset(unsigned int x, unsigned int y);
    int get_int();
    ~VGA256Term();

    private:
    unsigned char far *video_buffer;

    char term_buff[40][25];
    char term_buff2[40][25];
    int term_x, term_y;

    void Set_Video_Mode(int mode);
    void print_char(char c, int x1, int y1, char color, char bgcolor);
    void set_color(int index, int red, int green, int blue);
    void term_newline();
};

class Room {
    public:
    int ID;
    Room *rooms[3];
    bool hasAnkleBitters;
    bool hasMaltego;
    
    
    Room(int id);
    void setRooms(Room *r1, Room *r2, Room *r3);
    ~Room();
    #ifdef DEBUG
    void debug();
    #endif
};

class Player {
    public:
    Room *location;
    int arrows;
    bool hasDied;
    int supplies;

    Player(Room *location);
    ~Player();
    void doActions();
    #ifdef DEBUG
    void debug();
    #endif
};

class Muppet {
    public:
    Room *location;
    bool hasDied;
    int moveCounter;

    Muppet(Room *location, RC4 *Rand);
    void doActions();
    void runAway();
    ~Muppet();
    #ifdef DEBUG
    void debug();
    #endif
    private:
    RC4 *Rand;
    int prev_loc;
};

#endif
