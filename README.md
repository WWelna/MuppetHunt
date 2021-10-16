# Muppet Hunt
A silly game inspired by [Hunt The Wumpus](https://en.wikipedia.org/wiki/Hunt_the_Wumpus), but more... interesting!
![Game Play](https://i.imgur.com/lVhsPxxl.png)

## Gameplay Mechanics
* The cave system is a [dodecahedron](https://en.wikipedia.org/wiki/File:Hunt_the_Wumpus_map.svg) as in Hunt The Wumpus.
* Every 3 turns, The Muppet randomly moves to a cave nearby.
* You have 5 arrows that only fire into bordering caves.
* You have a 50% chance of dying when you encounter The Muppet.
* Every move depletes 1 supply out of the starting 20 supplies.
* You can camp and consume 1 supply rather than shooting or moving.
* There is 2 Maltego and 2 Ankle Biter hazards.
* The Maltego hazard randomly teleports the player to a location that doesn't have a Muppet. It is possible to be teleported to another Maltego hazard...
* The Ankle Biter hazard steals between 1 to 5 of your supplies.
* The Muppet is immune to hazards. 
* There is 2 caves that have a cache of supplies between 1 to 5

## Build Info
This is fully coded and compiled using [Open Watcom 1.9 C++ Compiler](www.openwatcom.org/). Output build is a 16-bit MSDOS COM Executable. Game is designed to be run on [DosBox](https://dosbox.com) or any DOS compatible setup. I used [DosBox-X](https://dosbox-x.com) for development and highly recommend it.

## TODO
* ~~Add Neural Network to The Muppet so it actively hunts and learns from player.~~ UPDATE: This would make the game too easy, as all you would have to do is back up to the previous cave, and fire an arrow to win the game. The randomness of the current AI with remembering the last cave so it does not go backwards, appears to be the most optimal.
* DosBox Launcher

## License
 
Copyright (C) 2021 William Welna (wwelna@occultusterra.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
