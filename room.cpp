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

Room::Room(int id) {
    this->ID = id;
    this->hasAnkleBitters = false;
    this->hasMaltego = false;
}

void Room::setRooms(Room *r1, Room *r2, Room *r3) {
    this->rooms[0] = r1;
    this->rooms[1] = r2;
    this->rooms[2] = r3;
}

Room::~Room() {
}

#ifdef DEBUG
void Room::debug() {
    printf("ROOM ID #%d Borders [%d %d %d]\n", this->ID, this->rooms[0]->ID, this->rooms[1]->ID, this->rooms[2]->ID);
}
#endif
