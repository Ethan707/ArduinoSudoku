#ifndef _SURFACE_H_
#define _SURFACE_H_

#include "global_define.h"
#include "sudoku.h"
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

void display_main_menu(MCUFRIEND_kbv *tft);
void display_stat_menu(MCUFRIEND_kbv *tft, Record record);
void display_diff_menu(MCUFRIEND_kbv *tft, bool resume);
void drawTheGrid(MCUFRIEND_kbv *tft, difficulty level, Sudoku sudoku);
#endif