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

Muppet::Muppet(Room *location) {
    this->location = location;
    this->moveCounter = 0;
    this->hasDied = false;
}

void Muppet::doActions() {
    if(this->moveCounter == 3) {
        this->runAway();
        this->moveCounter = 0;
    }
    this->moveCounter++;
}

void Muppet::runAway() {
    int randRoom = (rand()%3);
    this->location = this->location->rooms[randRoom];
}

Muppet::~Muppet() {
}

#ifdef DEBUG
void Muppet::debug() {
    printf("Muppet Location #%d moveCounter %d hasDied %d\n", this->location->ID, this->moveCounter, this->hasDied);
}
#endif
