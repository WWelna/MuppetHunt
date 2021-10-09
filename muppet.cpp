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

inline int getRandomRoom(RC4 *Rand) {
    int randint = (Rand->rand()%20);
    
    if(randint == 0 || randint == 4 || randint == 1 || randint == 7) randint += 11;

    return randint;
}

bool startGame(VGA256Term *Term, RC4 *Rand) {
    Room *rooms[20];
    int x;
    int randint;
    char c;

    for(x=0; x < 20; ++x)
        rooms[x] = new Room(x+1);

    // Ring 1
    rooms[0]->setRooms(rooms[4], rooms[1], rooms[7]);
    rooms[1]->setRooms(rooms[0], rooms[2], rooms[9]);
    rooms[2]->setRooms(rooms[1], rooms[3], rooms[11]);
    rooms[3]->setRooms(rooms[4], rooms[3], rooms[13]);
    rooms[4]->setRooms(rooms[0], rooms[3], rooms[5]);
    // Ring 2
    rooms[5]->setRooms(rooms[4], rooms[6], rooms[14]);
    rooms[6]->setRooms(rooms[5], rooms[7], rooms[16]);
    rooms[7]->setRooms(rooms[6], rooms[0], rooms[7]);
    rooms[8]->setRooms(rooms[7], rooms[16], rooms[18]);
    rooms[9]->setRooms(rooms[8], rooms[1], rooms[10]);
    rooms[10]->setRooms(rooms[9], rooms[18], rooms[11]);
    rooms[11]->setRooms(rooms[10], rooms[2], rooms[12]);
    rooms[12]->setRooms(rooms[11], rooms[19], rooms[13]);
    rooms[13]->setRooms(rooms[12], rooms[3], rooms[14]);
    rooms[14]->setRooms(rooms[13], rooms[15], rooms[5]);
    // Ring 3
    rooms[15]->setRooms(rooms[14], rooms[19], rooms[16]);
    rooms[16]->setRooms(rooms[15], rooms[6], rooms[17]);
    rooms[17]->setRooms(rooms[16], rooms[8], rooms[18]);
    rooms[18]->setRooms(rooms[17], rooms[10], rooms[19]);
    rooms[19]->setRooms(rooms[15], rooms[12], rooms[18]);

    Muppet *Cody = new Muppet(rooms[getRandomRoom(Rand)], Rand);
    
    // Add Ankle Bitters
    x=2;
    while(x > 0) {
        randint = getRandomRoom(Rand);
        if(!rooms[randint]->hasAnkleBitters && !rooms[randint]->hasMaltego) {
            rooms[randint]->hasAnkleBitters = true;
            x -= 1;
        }
    }

    // Add Maltego
    x=2;
    while(x > 0) {
        randint = getRandomRoom(Rand);
        if(!rooms[randint]->hasAnkleBitters && !rooms[randint]->hasMaltego) {
            rooms[randint]->hasMaltego = true;
            x -= 1;
        }
    }
    
    Player *Neal = new Player(rooms[0]);

    while(Neal->hasDied == false && Cody->hasDied == false) {
        Term->printf_term("\nYou are in Cave #%\n\t% Arrows and % Supplies.\n\tTunnels lead to [%, %, %]\n", Neal->location->ID, Neal->arrows, Neal->supplies, Neal->location->rooms[0]->ID, Neal->location->rooms[1]->ID, Neal->location->rooms[2]->ID);
        if( (Cody->location->ID == Neal->location->rooms[0]->ID) || (Cody->location->ID == Neal->location->rooms[1]->ID) || (Cody->location->ID == Neal->location->rooms[2]->ID) )
            Term->printf_term("You hear the Squeal of a Muppet!\n");
        if(Neal->location->rooms[0]->hasMaltego || Neal->location->rooms[1]->hasMaltego || Neal->location->rooms[2]->hasMaltego)
            Term->printf_term("You are blinded by rainbow orbs!\n");
        if(Neal->location->rooms[0]->hasAnkleBitters || Neal->location->rooms[1]->hasAnkleBitters || Neal->location->rooms[2]->hasAnkleBitters)
            Term->printf_term("You clutch your supplies nervously...\n");
        Term->printf_term("Muppet Location #%\n", Cody->location->ID);
        Term->printf_term("[M]ove [W]ait [S]hoot?\n");

top:   c = getch();
        
        switch(c) {
            case 'm':
            case 'M':
invalid1:    Term->printf_term("Where to? :> "); x = Term->get_int();
                if(Neal->location->rooms[0]->ID == x || Neal->location->rooms[1]->ID == x || Neal->location->rooms[2]->ID == x)
                    Neal->location = rooms[x-1];
                else {
                    Term->printf_term("Invalid Room ID\n");
                    goto invalid1;
                }
                break;
            case 's':
            case 'S':
invalid2:    Term->printf_term("Fire arrow to? :> "); x = Term->get_int();
                if(Neal->location->rooms[0]->ID == x || Neal->location->rooms[1]->ID == x || Neal->location->rooms[2]->ID == x) {
                    if(Neal->arrows > 0) {
                        if(Cody->location->ID == x) {
                            Term->printf_term("You hear a squeal!\n");
                            Cody->hasDied = true;
                        } else
                            Term->printf_term("You hear a ting...\n");
                    Neal->arrows -= 1;
                    } else {
                        Term->printf_term("You have no arrows left...\n");
                    }
                } else {
                    Term->printf_term("Invalid Room ID\n");
                    goto invalid2;
                }
                break;
            case 'w':
            case 'W':
                Term->printf_term("Camping for one turn...\n");
                break;
            default:
                goto top;
        }
        
        if(Neal->location->ID == Cody->location->ID) {
            if(rand()%2) {
                Term->printf_term("The Muppet runs away sqealing!\n");
                Cody->runAway();
            } else {
                Term->printf_term("The Muppet attacks!\n");
                Neal->hasDied = true;
            }
        }

        if(Neal->hasDied == false) {
            if(Neal->location->hasAnkleBitters == true) {
                Term->printf_term("Ankle Bitters have stolen from you!\n");
                Neal->supplies -= (Rand->rand()%4)+1; // 1-5 supplies get stolen
            } else if(Neal->location->hasMaltego == true) {
                Term->printf_term("You stare at Maltego and get disoriented...\n");
randagain: randint =  Rand->rand()%20;
                if(Cody->location->ID != rooms[randint]->ID)
                    Neal->location = rooms[randint];
                else goto randagain;
            }
        }

        if(Cody->hasDied == false) Cody->doActions();
        Neal->doActions();
    }

    if(Neal->hasDied == true) Term->printf_term("And... You have died...\n");
    else if(Cody->hasDied == true) {
        Term->printf_term("You've slain the Muppet!\n");
    }
    
    Term->printf_term("Play Again? [Y]es or any key to exit\n");
    c = getch();
    if(c == 'y' || c == 'Y') return true;
    else return false;
}

void print_center(VGA256Term *Term, char *s) {
    int len = strlen(s);
    int offset = (40-len)/2;
    Term->move_cursor_offset(offset, 0);
    Term->printf_term(s);
}

int main(int argc, unsigned char **argv) {
    char *seed = init_rc4();
    VGA256Term *Term = new VGA256Term();
    RC4 *Rand = new RC4(seed, SEEDSIZE);
    free(seed); // No Longer Needed
    
    print_center(Term, "In The Caves, You Must Survive...\n");
    print_center(Term, "The Angry Muppets!\n");
    
    while(startGame(Term, Rand) == true) {
        // clear screen and stuff?
    }

    delete Term;
    return 0;
}
