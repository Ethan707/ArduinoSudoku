#ifndef _STATE_H_
#define _STATE_H_
enum difficulty { EASY, NORMAL, TOUGH };
enum states { Menu, Game, Stat };
struct Record {
    int run[3];
    unsigned long time[3];
};
#endif