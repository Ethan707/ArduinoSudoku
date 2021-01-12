#include "interface.h"
#include "global_define.h"
#include "sudoku.h"
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

#define BLOCK_SIZE 32

// main menu
#define TITLE_LEFT_MARGIN 75
#define TITLE_TOP_MARGIN 100

#define SUBHEAD_LEFT_MARGIN 125
#define SUBHEAD_TOP_MARGIN 140

#define MAIN_BUTTON_TOP_MARGIN 200
#define MAIN_GAP 75
#define MAIN_WORD_LEFT_MARGIN 130
#define MAIN_WORD_TOP_MARGIN 215

#define MOVE 25

// stats menu
#define STATS_TITLE_LEFT 120
#define STATS_TITLE_TOP 40
#define STATS_ICON_LEFT_MARGIN 15
#define STATS_ICON_TOP_MARGIN 100
#define STATS_GAP 95
#define STATS_ICON_WIDTH 130
#define STATS_ICON_HEIGHT 80

#define STATS_WORD_TOP 130
#define STATS_EASY_LEFT 45
#define STATS_NORMAL_LEFT 28
#define STATS_TOUGH_LEFT 35

#define STATS_WORD_LEFT 160
#define STATS_RUN_TOP 120
#define STATS_TIME_TOP STATS_RUN_TOP + 20

#define STATS_BACK_LEFT_MARGIN 130
#define STATS_BACK_TOP_MARGIN 410
#define STATS_BUTTON_TOP_MARGIN 395

// difficulty menu
#define DIFF_BUTTON_TOP_MARGIN 150
#define DIFF_EASY_MARGIN 130
#define DIFF_NORMAL_MARGIN 120
#define DIFF_TOUGH_MARGIN 122
#define DIFF_BACK_MARGIN 130
#define DIFF_RESUME_MARGIN 120
#define DIFF_WORD_TOP_MARGIN 165
#define DIFF_GAP 65

#define BUTTON_LEFT 67
#define BUTTON_WIDTH 200
#define BUTTON_HIGHT 50
#define BUTTON_RADIUS 4

// game interface
#define GAME_BUTTON_TOP_MARGIN 8
#define GAME_BUTTON_WIDTH BLOCK_SIZE * 3
#define GAME_BUTTON_HEIGHT BLOCK_SIZE + 10
#define GAME_TITLE_HEIGHT 15

#define GAME_EASY_WIDTH 125
#define GAME_TOUGH_WIDTH 118
#define GAME_NORMAL_WIDTH 108

#define GAME_WORD_TOP_MARGIN 18
#define GAME_BACK_LEFT_MARGIN 15
#define GAME_TIPS_LEFT_MARGIN 233

#define INITIALIZED_LEFT_MARGIN 3
#define INITIALIZED_TOP_MARGIN 93
#define GAME_GAP 2
#define LINES_WIDTH 1
#define LINES_LENGTH 309

#define NUM_MARGIN_LEFT 13
#define NUM_MARGIN_TOP 98

#define NUM_BUTTON_LEFT 7
#define NUM_BUTTON_TOP 440
#define NUM_BUTTON_WORD_LEFT 16
#define NUM_BUTTON_WORD_TOP 445

#define DARK_GRAY 0x2965
#define CYAN 0x5654

void display_main_menu(MCUFRIEND_kbv *tft) {
    // title
    tft->fillScreen(TFT_BLACK);
    tft->setCursor(TITLE_LEFT_MARGIN, TITLE_TOP_MARGIN);
    tft->setTextSize(5);
    tft->println("SUDOKU");

    tft->setCursor(SUBHEAD_LEFT_MARGIN, SUBHEAD_TOP_MARGIN);
    tft->setTextSize(1);
    tft->println("Arduino Version");

    // button
    tft->setTextSize(3);
    tft->fillRoundRect(BUTTON_LEFT, MAIN_BUTTON_TOP_MARGIN, BUTTON_WIDTH,
                       BUTTON_HIGHT, BUTTON_RADIUS, TFT_BLUE);
    tft->setCursor(MAIN_WORD_LEFT_MARGIN, MAIN_WORD_TOP_MARGIN);
    tft->println("PLAY");
    tft->fillRoundRect(BUTTON_LEFT, MAIN_BUTTON_TOP_MARGIN + MAIN_GAP,
                       BUTTON_WIDTH, BUTTON_HIGHT, BUTTON_RADIUS, TFT_ORANGE);
    tft->setCursor(MAIN_WORD_LEFT_MARGIN, MAIN_WORD_TOP_MARGIN + MAIN_GAP);
    tft->println("STAT");
}

void display_stat_menu(MCUFRIEND_kbv *tft, Record record) {
    int min = 0, second = 0;
    tft->fillScreen(TFT_BLACK);

    // title
    tft->setCursor(STATS_TITLE_LEFT, STATS_TITLE_TOP);
    tft->setTextSize(3);
    tft->println("STATS");

    tft->fillRoundRect(STATS_ICON_LEFT_MARGIN, STATS_ICON_TOP_MARGIN,
                       STATS_ICON_WIDTH, STATS_ICON_HEIGHT, BUTTON_RADIUS,
                       CYAN);
    tft->setCursor(STATS_EASY_LEFT, STATS_WORD_TOP);
    tft->println("EASY");
    tft->setTextSize(2);
    tft->setCursor(STATS_WORD_LEFT, STATS_RUN_TOP);
    tft->print("Run: ");
    tft->println(record.run[0]);
    tft->setCursor(STATS_WORD_LEFT, STATS_TIME_TOP);
    tft->print("Time: ");
    min = record.time[0] / 60;
    second = record.time[0] % 60;
    if (min != 0) {
        tft->print(min);
        tft->print("min");
    }
    tft->print(second);
    tft->print("s");

    tft->setTextSize(3);
    tft->fillRoundRect(STATS_ICON_LEFT_MARGIN,
                       STATS_ICON_TOP_MARGIN + STATS_GAP, STATS_ICON_WIDTH,
                       STATS_ICON_HEIGHT, BUTTON_RADIUS, CYAN);
    tft->setCursor(STATS_NORMAL_LEFT, STATS_WORD_TOP + STATS_GAP);
    tft->println("NORMAL");
    tft->setTextSize(2);
    tft->setCursor(STATS_WORD_LEFT, STATS_RUN_TOP + STATS_GAP);
    tft->print("Run: ");
    tft->println(record.run[1]);
    tft->setCursor(STATS_WORD_LEFT, STATS_TIME_TOP + STATS_GAP);
    tft->print("Time: ");
    min = record.time[1] / 60;
    second = record.time[1] % 60;
    if (min != 0) {
        tft->print(min);
        tft->print("min");
    }
    tft->print(second);
    tft->print("s");

    tft->setTextSize(3);
    tft->fillRoundRect(STATS_ICON_LEFT_MARGIN,
                       STATS_ICON_TOP_MARGIN + 2 * STATS_GAP, STATS_ICON_WIDTH,
                       STATS_ICON_HEIGHT, BUTTON_RADIUS, CYAN);
    tft->setCursor(STATS_TOUGH_LEFT, STATS_WORD_TOP + 2 * STATS_GAP);
    tft->println("TOUGH");
    tft->setTextSize(2);
    tft->setCursor(STATS_WORD_LEFT, STATS_RUN_TOP + 2 * STATS_GAP);
    tft->print("Run: ");
    tft->println(record.run[2]);
    tft->setCursor(STATS_WORD_LEFT, STATS_TIME_TOP + 2 * STATS_GAP);
    tft->print("Time: ");
    min = record.time[2] / 60;
    second = record.time[2] % 60;
    if (min != 0) {
        tft->print(min);
        tft->print("min");
    }
    tft->print(second);
    tft->print("s");

    // back button
    tft->setTextSize(3);
    tft->fillRoundRect(BUTTON_LEFT, STATS_BUTTON_TOP_MARGIN, BUTTON_WIDTH,
                       BUTTON_HIGHT, BUTTON_RADIUS, TFT_RED);
    tft->setCursor(STATS_BACK_LEFT_MARGIN, STATS_BACK_TOP_MARGIN);
    tft->println("BACK");
}

void display_diff_menu(MCUFRIEND_kbv *tft, bool resume) {
    // title
    tft->fillScreen(TFT_BLACK);
    tft->setCursor(TITLE_LEFT_MARGIN, TITLE_TOP_MARGIN - MOVE);
    tft->setTextSize(5);
    tft->println("SUDOKU");

    tft->setCursor(SUBHEAD_LEFT_MARGIN, SUBHEAD_TOP_MARGIN - MOVE);
    tft->setTextSize(1);
    tft->println("Arduino Version");

    // EASY BUTTON
    tft->setTextSize(3);
    tft->fillRoundRect(BUTTON_LEFT, DIFF_BUTTON_TOP_MARGIN, BUTTON_WIDTH,
                       BUTTON_HIGHT, BUTTON_RADIUS, CYAN);
    tft->setCursor(DIFF_EASY_MARGIN, DIFF_WORD_TOP_MARGIN);
    tft->println("EASY");

    // NORMAL BUTTON
    tft->fillRoundRect(BUTTON_LEFT, DIFF_BUTTON_TOP_MARGIN + DIFF_GAP,
                       BUTTON_WIDTH, BUTTON_HIGHT, BUTTON_RADIUS, CYAN);
    tft->setCursor(DIFF_NORMAL_MARGIN, DIFF_WORD_TOP_MARGIN + DIFF_GAP);
    tft->println("NORMAL");

    // TOUGH BUTTON
    tft->fillRoundRect(BUTTON_LEFT, DIFF_BUTTON_TOP_MARGIN + 2 * DIFF_GAP,
                       BUTTON_WIDTH, BUTTON_HIGHT, BUTTON_RADIUS, CYAN);
    tft->setCursor(DIFF_TOUGH_MARGIN, DIFF_WORD_TOP_MARGIN + 2 * DIFF_GAP);
    tft->println("TOUGH");

    // RESUME BUTTON
    if (!resume) {
        tft->fillRoundRect(BUTTON_LEFT, DIFF_BUTTON_TOP_MARGIN + 3 * DIFF_GAP,
                           BUTTON_WIDTH, BUTTON_HIGHT, BUTTON_RADIUS, TFT_RED);
        tft->setCursor(DIFF_BACK_MARGIN, DIFF_WORD_TOP_MARGIN + 3 * DIFF_GAP);
        tft->println("BACK");
    } else {
        tft->fillRoundRect(BUTTON_LEFT, DIFF_BUTTON_TOP_MARGIN + 3 * DIFF_GAP,
                           BUTTON_WIDTH, BUTTON_HIGHT, BUTTON_RADIUS, TFT_RED);
        tft->setCursor(DIFF_RESUME_MARGIN, DIFF_WORD_TOP_MARGIN + 3 * DIFF_GAP);
        tft->println("RESUME");

        tft->fillRoundRect(BUTTON_LEFT, DIFF_BUTTON_TOP_MARGIN + 4 * DIFF_GAP,
                           BUTTON_WIDTH, BUTTON_HIGHT, BUTTON_RADIUS, TFT_RED);
        tft->setCursor(DIFF_BACK_MARGIN, DIFF_WORD_TOP_MARGIN + 4 * DIFF_GAP);
        tft->println("BACK");
    }
}

void drawTheGrid(MCUFRIEND_kbv *tft, difficulty level, Sudoku sudoku) {
    tft->fillScreen(TFT_BLACK);
    tft->setTextSize(3);

    // Title
    if (level == EASY) {
        tft->setCursor(GAME_EASY_WIDTH, GAME_TITLE_HEIGHT);
        tft->print("EASY");
    }
    if (level == TOUGH) {
        tft->setCursor(GAME_TOUGH_WIDTH, GAME_TITLE_HEIGHT);
        tft->print("TOUGH");
    }
    if (level == NORMAL) {
        tft->setCursor(GAME_NORMAL_WIDTH, GAME_TITLE_HEIGHT);
        tft->print("NORMAL");
    }

    // back button
    tft->fillRoundRect(3, GAME_BUTTON_TOP_MARGIN, GAME_BUTTON_WIDTH,
                       GAME_BUTTON_HEIGHT, BUTTON_RADIUS, CYAN);
    tft->setCursor(GAME_BACK_LEFT_MARGIN, GAME_WORD_TOP_MARGIN);
    tft->print("BACK");

    // tips button
    tft->fillRoundRect(221, GAME_BUTTON_TOP_MARGIN, GAME_BUTTON_WIDTH,
                       GAME_BUTTON_HEIGHT, BUTTON_RADIUS, CYAN);
    tft->setCursor(GAME_TIPS_LEFT_MARGIN, GAME_WORD_TOP_MARGIN);
    tft->print("TIPS");

    int margin_left = INITIALIZED_LEFT_MARGIN,
        margin_top = INITIALIZED_TOP_MARGIN;

    // draw the grid
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            margin_left += GAME_GAP;
            tft->fillRoundRect(margin_left, margin_top, BLOCK_SIZE, BLOCK_SIZE,
                               BUTTON_RADIUS, DARK_GRAY);
            margin_left += BLOCK_SIZE;
            if ((j + 1) % 3 == 0) {
                margin_left += GAME_GAP + LINES_WIDTH;
            }
        }
        margin_left = INITIALIZED_LEFT_MARGIN;
        margin_top += GAME_GAP + BLOCK_SIZE;
        if ((i + 1) % 3 == 0) {
            margin_top += GAME_GAP + LINES_WIDTH;
        }
    }

    // draw the lines divided the grid into 9 small grid
    margin_left = INITIALIZED_LEFT_MARGIN;
    margin_top = INITIALIZED_TOP_MARGIN;
    for (int i = 0; i < 2; i++) {
        margin_left += (GAME_GAP * 4 + BLOCK_SIZE * 3);
        tft->drawLine(margin_left, margin_top, margin_left,
                      margin_top + LINES_LENGTH, TFT_WHITE);
        margin_left++;
    }

    margin_left = INITIALIZED_LEFT_MARGIN + GAME_GAP;
    margin_top = INITIALIZED_TOP_MARGIN;
    for (int i = 0; i < 2; i++) {
        margin_top += (GAME_GAP * 3 + BLOCK_SIZE * 3);
        tft->drawLine(margin_left, margin_top, margin_left + LINES_LENGTH,
                      margin_top, TFT_WHITE);
        margin_top += GAME_GAP + LINES_WIDTH;
    }

    // print the number on the grid
    margin_left = NUM_MARGIN_LEFT;
    margin_top = NUM_MARGIN_TOP;
    int value;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            tft->setCursor(margin_left, margin_top);
            value = sudoku.getValue(i, j);
            // value = i;
            if (value != 0) {
                tft->println(value);
            }
            if ((j + 1) % 3 == 0) {
                margin_left += GAME_GAP + LINES_WIDTH;
            }
            margin_left += BLOCK_SIZE + GAME_GAP;
        }
        margin_left = NUM_MARGIN_LEFT;
        margin_top += BLOCK_SIZE + GAME_GAP;
        if ((i + 1) % 3 == 0) {
            margin_top += GAME_GAP + LINES_WIDTH;
        }
    }

    // print the number button
    margin_left = NUM_BUTTON_LEFT;
    margin_top = NUM_BUTTON_TOP;
    for (int i = 0; i < 9; i++) {
        tft->fillRoundRect(margin_left, margin_top, BLOCK_SIZE, BLOCK_SIZE,
                           BUTTON_RADIUS, DARK_GRAY);
        margin_left += BLOCK_SIZE + GAME_GAP;
    }
    tft->setTextSize(3);
    tft->setCursor(NUM_BUTTON_WORD_LEFT, NUM_BUTTON_WORD_TOP);
    for (int i = 1; i <= 9; i++) {
        tft->println(i);
        tft->setCursor(NUM_BUTTON_WORD_LEFT + (BLOCK_SIZE + GAME_GAP) * i,
                       NUM_BUTTON_WORD_TOP);
    }
    tft->drawRoundRect(NUM_BUTTON_LEFT, NUM_BUTTON_TOP, BLOCK_SIZE, BLOCK_SIZE,
                       BUTTON_RADIUS, TFT_RED);
}