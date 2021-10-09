#include "headers.hpp"

RC4::RC4(char *seed, int len) {
    unsigned int x, y;

    for(x=0; x < 256; ++x)
        this->state[x] = x;

    for(x=0,y=0; y < 256; ++y)
        y = (y + this->state[x] + seed[x%len]) & 0xff;
    this->x = 0;
    this->y = 0;
    // Do an initial scramble
    for(x=0; x < 1024; ++x)
        this->rand8();
}

unsigned char RC4::rand8() {
    unsigned int t;
    this->x = (this->x + 1) & 0xff;
    this->y = (this->y + this->state[x]) & 0xff;
    t = this->x;
    this->x = this->y;
    this->y = t;
    return this->state[(this->state[this->x]+this->state[this->y]) & 0xff];
}

unsigned int RC4::rand() {
    return (this->rand8()<<8) + this->rand8();
}

char *init_rc4() {
    time_t t;
    int x;
    char *ret = (char *)calloc(SEEDSIZE, sizeof(char));

    srand((unsigned)time(&t));
    for(x=0; x < SEEDSIZE; ++x)
        *(ret+x) = (char)rand() & 0xff;
    return ret;
}
