/* Copyright (C) 2021 William Welna (wwelna@occultusterra.com)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include "headers.hpp"

#include "font8x8_basic.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define GLYPH_SIZE 8

#define TERM_WIDTH 40
#define TERM_HEIGHT 25

#define VGA256 0x13
#define TEXT_MODE 0x03

/* 
inline void put_pixel(int x, int y, unsigned char color) { // Not used anymore :'(
    VGA[((y<<8)+(y<<6))+x] = color;
} */

VGA256Term::VGA256Term() {
    this->video_buffer = (unsigned char far *)0xA0000000L;
    this->term_x = 0;
    this->term_y = 0;
    this->Set_Video_Mode(VGA256);
    this->clear_screen(15); // 15 = White
    // Make sure this stuff is clear
    memset(this->term_buff, 0, TERM_WIDTH*TERM_HEIGHT);
    memset(this->term_buff2, 0, TERM_WIDTH*TERM_HEIGHT);
}

void VGA256Term::print_char(char c, int x1, int y1, char color, char bgcolor) {
    int x,y;
    char *glyph;
    if(c == 0) glyph = (char *)font8x8_basic[0];
    else glyph = (char *)font8x8_basic[c-32];
    for(y=0; y < GLYPH_SIZE; ++y)
        for(x=0; x < GLYPH_SIZE; ++x)
            if(glyph[y] & (1 << x))
                this->video_buffer[(((y1+y)<<8)+((y1+y)<<6))+(x1+x)] = color;
            else
                this->video_buffer[(((y1+y)<<8)+((y1+y)<<6))+(x1+x)] = bgcolor;
}

void VGA256Term::draw_term() {
    int x,y;
    for(y=0; y < TERM_HEIGHT; ++y)
        for(x=0; x < TERM_WIDTH; ++x)
            if(term_buff2[x][y] != term_buff[x][y])
                this->print_char(term_buff[x][y], x*GLYPH_SIZE, y*GLYPH_SIZE, 0, 15); // 0 = Black, 15 = White
    memcpy(term_buff2, term_buff, TERM_WIDTH*TERM_HEIGHT);
}

void VGA256Term::term_newline() {
    unsigned int x, y;
    if(term_y >= TERM_HEIGHT-1) {
        for(y=1; y < TERM_HEIGHT; ++y)
            for(x=0; x < TERM_WIDTH; ++x) // Move everything up by 1, erasing the first line
                term_buff[x][y-1] = term_buff[x][y];
        for(x=0; x < TERM_WIDTH; ++x) // clear last line
            term_buff[x][TERM_HEIGHT-1] = 0;
       term_y = TERM_HEIGHT-1;
    } else term_y += 1;
    term_x = 0; // Back to start
}

void VGA256Term::move_cursor_offset(unsigned int x, unsigned int y) {
    if( (term_x + x) < TERM_WIDTH && (term_y + y) < TERM_HEIGHT) {
        term_x += x;
        term_y += y;
    }
}

void VGA256Term::printf_term(char *s, ...) {
    char buffer[8]; // Shouldn't have a number longer than 7 digits...
    unsigned int x=0;
    unsigned int arg_stack_count=0;
    unsigned int arg_int;
    va_list ap;

    while(*(s+x) != 0) {
        if(*(s+x) == '%') arg_stack_count += 1;
        ++x;
    }

    va_start(ap, s);

    for(x=0; *(s+x) != 0; ++x) {
        if(*(s+x) == '\n')
            this->term_newline();
        else if(*(s+x) == '\t')
            this->move_cursor_offset(3, 0); // 3 space tabs
        else if(*(s+x) == '%') { // Print decimal
            arg_int = va_arg(ap, int);
            memset(buffer, 0, 8);
            itoa(arg_int, buffer, 10);
            this->printf_term(buffer); // recursive call to print string sequence
        } else if(term_x < TERM_WIDTH && term_y < TERM_HEIGHT) { // Within bounds
            term_buff[term_x][term_y] = *(s+x);
            term_x += 1;
        } else if(term_x > TERM_WIDTH) { // do screen wrap
            this->term_newline();
            term_buff[term_x][term_y] = *(s+x);
            term_x += 1;
        }
    }
    va_end(ap);
    this->draw_term();
}

void VGA256Term::printchar_term(char c) {
    if(term_x < TERM_WIDTH && term_y < TERM_HEIGHT) {
        term_buff[term_x][term_y] = c;
        term_x += 1;
    } else if (term_x > TERM_WIDTH) { // screen wrap
        this->term_newline();
        term_buff[term_x][term_y] = c;
        term_x += 1;
    }
}

int VGA256Term::get_int() {
    char c;
    int ret=0;
    while ((c=getch()) != '\r')
        if(c < 58 && c > 47) {
            ret = ((ret << 3) + (ret << 1)) + (c-48);
            this->printchar_term(c); this->draw_term();
        }
    this->term_newline();
    return ret;
}

void VGA256Term::clear_screen(unsigned char color) {
    _fmemset(this->video_buffer, color, SCREEN_WIDTH*SCREEN_HEIGHT);
}

void VGA256Term::Set_Video_Mode(int mode) {
   union REGS inregs, outregs;

   inregs.h.ah = 0;
   inregs.h.al = (unsigned char)mode;
   int86(0x10, &inregs, &outregs);
}

void VGA256Term::set_color(int index, int red, int green, int blue) {
    outp(0x03c8, index);
    outp(0x03c9, red);
    outp(0x03c9, green);
    outp(0x03c9, blue);
}

/*
inline int sgn(int x) {
    return (x < 0) ? -1 : (x > 0);
} */

VGA256Term::~VGA256Term() {
    this->Set_Video_Mode(TEXT_MODE);
}
