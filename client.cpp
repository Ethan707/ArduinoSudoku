#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include <SD.h>
#include <SPI.h>
#include <TouchScreen.h>

#include "global_define.h"
#include "interface.h"
#include "sudoku.h"

// touch screen pins, obtained from the documentation
#define YP A3 // must be an analog pin, use "An" notation!
#define XM A2 // must be an analog pin, use "An" notation!
#define YM 9  // can be a digital pin
#define XP 8  // can be a digital pin

// width/height of the display when rotated horizontally
#define TFT_WIDTH 480
#define TFT_HEIGHT 320

// calibration data for the touch screen, obtained from documentation
// the minimum/maximum possible readings from the touch point

#define TS_MINX 100
#define TS_MINY 110
#define TS_MAXX 960
#define TS_MAXY 910

// thresholds to determine if there was a touch
#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define BLOCK_SIZE 32
#define SPACE 2
#define DARK_GRAY 0x7BEF
#define LIGHT_GRAY 0xBDF7
#define RED 0x2965

#define SD_CS 10

#define BUTTON_LEFT 50
#define BUTTON_RIGHT 250

// the values of buttons in main menu for touching screen
#define GAME_BUTTON_BOTTOM 200
#define GAME_BUTTON_TOP 250
#define STATS_BUTTON_BOTTOM 280
#define STATS_BUTTON_TOP 330

// the values of back button in stats menu for touching screen
#define STATS_BACK_TOP 450
#define STATS_BACK_BOTTOM 400

// the values of difficulty button in difficulty menu for touching screen
#define EASY_TOP 200
#define EASY_BOTTOM 150
#define NORMAL_TOP 270
#define NORMAL_BOTTOM 220
#define TOUGH_TOP 325
#define TOUGH_BOTTOM 285
#define RESUME_TOP 400
#define RESUME_BOTTOM 350
#define BACK_TOP 460
#define BACK_BOTTOM 415

// the values of button and message in game menu for touching screen
#define TIME_MESSAGE_LEFT 15
#define TIME_MESSAGE_TOP 60

#define NUM_LIST_TOP 470
#define NUM_LIST_BOTTOM 440
#define NUM_LIST_LEFT 8
#define NUM_LIST_RIGHT 304
#define GAME_TOP 50
#define GAME_BOTTOM 10
#define GAME_BACK_RIGHT 310
#define GAME_BACK_LEFT 217
#define GAME_TIPS_RIGHT 95
#define GAME_TIPS_LEFT 4

#define RADIUS 4
#define BUTTON_LEFT_MARGIN 7
#define BLOCK_LEFT_MARGIN 5
#define BLOCK_TOP_MARGIN 93
#define BLOCK_NUM_LET_MARGIN 13
#define BLOCK_NUM_TOP_MARGIN 98
#define CROSS_SPACE 5

states currentState;
difficulty diff;
Sudoku sudoku(EASY);

Record record;
unsigned long CurrentTime;

MCUFRIEND_kbv tft;
Sd2Card card;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
unsigned long getSeed();
void readRecord();

void setup() {
    init();
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    tft.begin(ID);
    Serial.print("Initializing SD card...");
    if (!SD.begin(SD_CS)) {
        Serial.println("failed!");
        Serial.println("Is the card inserted properly?");
        while (true) {
        }
    }
    Serial.println("OK");

    Serial.print("Initializing SPI communication for raw reads...");
    if (!card.init(SPI_HALF_SPEED, SD_CS)) {
        Serial.println("failed!");
        while (true) {
        }
    }

    Serial.println("OK");
    Serial.println();
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(0);
    tft.setTextSize(2);

    record = {{0, 0, 0}, {0, 0, 0}};
    readRecord();
    currentState = Menu;
    unsigned long seed = getSeed();
    randomSeed(seed);
}

// creat the random seed from the A7 pin on the Arduino
unsigned long getSeed() {
    unsigned long seed = 0;
    for (int i = 0; i < 20; i++) {
        int val = analogRead(A7);
        seed = seed << 1;
        seed = seed + (val & 1);
        delay(5);
    }
    return seed;
}

// read the record from the sd card
void readRecord() {
    File myFile;
    int run = 0;
    unsigned long time = 0;
    if (!SD.exists("stats.txt")) {
        myFile = SD.open("stats.txt", FILE_WRITE);
        // if the file opened okay, write to it:
        if (myFile) {
            for (int i = 0; i < 3; i++) {
                myFile.println(record.run[i]);
            }
            for (int i = 0; i < 3; i++) {
                myFile.println(record.time[i]);
            }
            myFile.close();
        }
    } else {
        // re-open the file for reading:
        myFile = SD.open("stats.txt");
        if (myFile) {
            // read from the file until there's nothing else in it:
            int index = 0;
            while (myFile.available()) {
                if (index < 3) {
                    int tmp = myFile.read();
                    if (tmp <= '9' && tmp >= '0') {
                        run = run * 10 + (tmp - '0');
                    } else if (tmp == 10) {
                        record.run[index] = run;
                        index++;
                        run = 0;
                    }
                } else {
                    int tmp = myFile.read();
                    if (tmp <= '9' && tmp >= '0') {
                        time = time * 10 + (tmp - '0');
                    } else if (tmp == 10) {
                        record.time[index - 3] = time;
                        index++;
                        time = 0;
                    }
                }
            }
            myFile.close();
        }
    }
}

// delete the record in the sd card and write the new one in it
void updateRecord() {
    File myFile;
    SD.remove("stats.txt");
    myFile = SD.open("stats.txt", FILE_WRITE);
    // if the file opened okay, write to it:
    if (myFile) {
        for (int i = 0; i < 3; i++) {
            myFile.println(record.run[i]);
        }
        for (int i = 0; i < 3; i++) {
            myFile.println(record.time[i]);
        }
        myFile.close();
    }
}

bool resumeGame() {
    File myFile;
    if (!SD.exists("game.txt")) {
        // Serial.println("Hello World");
        return false;
    } else {
        // re-open the file for reading:
        myFile = SD.open("game.txt");
        if (myFile) {
            // read from the file until there's nothing else in it:
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    int tmp;
                    do {
                        tmp = myFile.read();
                    } while (tmp > '9' || tmp < '0');
                    sudoku.setResumeGrid(i, j, tmp - '0');
                }
            }

            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    int tmp;
                    do {
                        tmp = myFile.read();
                    } while (tmp > '9' || tmp < '0');
                    sudoku.setResumeSolution(i, j, tmp - '0');
                }
            }

            int tmp;

            do {
                tmp = myFile.read();
            } while (tmp > '9' || tmp < '0');
            sudoku.setResumeLevel(tmp - '0');

            // skip the '\r' and '\n'
            myFile.read();
            myFile.read();
            unsigned long t = 0; // the time of the game need to be resumed
            do {
                tmp = myFile.read();
                if (tmp <= '9' && tmp >= '0') {
                    t = t * 10 + (tmp - '0');
                }
            } while (tmp != 10);
            sudoku.setTime(t);

            myFile.close();
            return true;
        }
    }
}
// delete the game record in the sd card and write the new one in it
void updateResume() {
    File myFile;
    if (SD.exists("game.txt")) {
        SD.remove("game.txt");
    }
    myFile = SD.open("game.txt", FILE_WRITE);
    if (myFile) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                myFile.println(sudoku.getValue(i, j));
            }
        }
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                myFile.println(sudoku.getSolution(i, j));
            }
        }
        myFile.println(sudoku.getLevel());
        myFile.println(sudoku.getTime());
        myFile.close();
    }
}

void mainMenu() {
    display_main_menu(&tft);
    // Check for touchscreen press and draw dots for each restaurant

    while (true) {
        TSPoint touch = ts.getPoint();

        // Necessary to resume TFT display functions
        pinMode(YP, OUTPUT);
        pinMode(XM, OUTPUT);

        // If there was an actual touch, draw the dots
        if (touch.z >= MINPRESSURE && touch.z <= MAXPRESSURE) {
            int16_t screen_x = map(touch.y, TS_MINX, TS_MAXX, TFT_WIDTH - 1, 0);
            int16_t screen_y =
                map(touch.x, TS_MINY, TS_MAXY, TFT_HEIGHT - 1, 0);

            if (screen_y > BUTTON_LEFT && screen_y < BUTTON_RIGHT) {
                if (screen_x > GAME_BUTTON_BOTTOM &&
                    screen_x < GAME_BUTTON_TOP) {
                    currentState = Game;
                    return;
                }
                if (screen_x > STATS_BUTTON_BOTTOM &&
                    screen_x < STATS_BUTTON_TOP) {
                    currentState = Stat;
                    return;
                }
            }
        }
    }
}

void statMenu() {
    display_stat_menu(&tft, record);
    while (true) {
        TSPoint touch = ts.getPoint();

        // Necessary to resume TFT display functions
        pinMode(YP, OUTPUT);
        pinMode(XM, OUTPUT);

        // If there was an actual touch, draw the dots
        if (touch.z >= MINPRESSURE && touch.z <= MAXPRESSURE) {
            int16_t screen_x = map(touch.y, TS_MINX, TS_MAXX, TFT_WIDTH - 1, 0);
            int16_t screen_y =
                map(touch.x, TS_MINY, TS_MAXY, TFT_HEIGHT - 1, 0);

            if (screen_y > BUTTON_LEFT && screen_y < BUTTON_RIGHT) {
                if (screen_x > STATS_BACK_BOTTOM && screen_x < STATS_BACK_TOP) {
                    // go back
                    currentState = Menu;
                    return;
                }
            }
        }
    }
}

void difficultyMenu(bool &resume) {
    resume = resumeGame();
    display_diff_menu(&tft, resume);
    while (true) {
        TSPoint touch = ts.getPoint();

        // Necessary to resume TFT display functions
        pinMode(YP, OUTPUT);
        pinMode(XM, OUTPUT);

        // If there was an actual touch, draw the dots
        if (touch.z >= MINPRESSURE && touch.z <= MAXPRESSURE) {
            int16_t screen_x = map(touch.y, TS_MINX, TS_MAXX, TFT_WIDTH - 1, 0);
            int16_t screen_y =
                map(touch.x, TS_MINY, TS_MAXY, TFT_HEIGHT - 1, 0);

            if (screen_y > BUTTON_LEFT && screen_y < BUTTON_RIGHT) {
                if (screen_x > EASY_BOTTOM && screen_x < EASY_TOP) {
                    resume = false;
                    diff = EASY;
                    return;
                }
                if (screen_x > NORMAL_BOTTOM && screen_x < NORMAL_TOP) {
                    resume = false;
                    diff = NORMAL;
                    return;
                }
                if (screen_x > TOUGH_BOTTOM && screen_x < TOUGH_TOP) {
                    resume = false;
                    diff = TOUGH;
                    return;
                }
                if (!resume) {
                    if (screen_x > RESUME_BOTTOM && screen_x < RESUME_TOP) {
                        // go back
                        currentState = Menu;
                        return;
                    }
                } else {
                    if (screen_x > RESUME_BOTTOM && screen_x < RESUME_TOP) {
                        // go back
                        resume = true;
                        return;
                    }
                    if (screen_x > BACK_BOTTOM && screen_x < BACK_TOP) {
                        // go back
                        currentState = Menu;
                        return;
                    }
                }
            }
        }
    }
}

void gameover() {
    record.run[diff]++;
    unsigned long second = (millis() - CurrentTime + sudoku.getTime()) / 1000;
    if (record.time[diff] == 0) {
        record.time[diff] = second;
    } else {
        record.time[diff] = min(record.time[diff], second);
    }
    currentState = Stat;

    tft.setCursor(TIME_MESSAGE_LEFT, TIME_MESSAGE_TOP);
    tft.setTextSize(2);
    tft.print("Bingo!!!      Time:");
    tft.print(second);
    tft.println("s");
    delay(2000);
    updateRecord();
    if (SD.exists("game.txt")) {
        SD.remove("game.txt");
    }
}

void startGaming() {
    int currDigit = 1; //当前选择的数字
    CurrentTime = millis();
    while (true) {
        TSPoint touch = ts.getPoint();

        // Necessary to resume TFT display functions
        pinMode(YP, OUTPUT);
        pinMode(XM, OUTPUT);

        // If there was an actual touch, draw the dots
        if (touch.z >= MINPRESSURE && touch.z <= MAXPRESSURE) {
            int16_t screen_x = map(touch.y, TS_MINX, TS_MAXX, TFT_WIDTH - 1, 0);
            int16_t screen_y =
                map(touch.x, TS_MINY, TS_MAXY, TFT_HEIGHT - 1, 0);
            if (screen_x >= NUM_LIST_BOTTOM && screen_x <= NUM_LIST_TOP &&
                screen_y >= NUM_LIST_LEFT && screen_y <= NUM_LIST_RIGHT) {
                int newDigit = 9 - (screen_y - 8) / 33;
                if (newDigit != currDigit) {
                    // erase the last draw
                    tft.drawRoundRect(7 + (currDigit - 1) *
                                              (BLOCK_SIZE + SPACE),
                                      NUM_LIST_BOTTOM, BLOCK_SIZE, BLOCK_SIZE,
                                      RADIUS, TFT_BLACK);
                    currDigit = newDigit;
                    tft.drawRoundRect(7 + (currDigit - 1) *
                                              (BLOCK_SIZE + SPACE),
                                      NUM_LIST_BOTTOM, BLOCK_SIZE, BLOCK_SIZE,
                                      RADIUS, TFT_RED);
                }
                delay(200);
                continue;
            }

            if (screen_x >= GAME_BOTTOM && screen_x <= GAME_TOP) {
                // back button
                if (screen_y >= GAME_BACK_LEFT && screen_y <= GAME_BACK_RIGHT) {
                    sudoku.setTime(millis() - CurrentTime);
                    updateResume();

                    currentState = Game;
                    return;
                }
                // tip button
                if (screen_y >= GAME_TIPS_LEFT && screen_y <= GAME_TIPS_RIGHT) {
                    int grid_x, grid_y, tipValue;
                    sudoku.getTips(grid_y, grid_x, tipValue);
                    sudoku.fillGrid(grid_y, grid_x, tipValue);
                    tft.setCursor(
                        BLOCK_NUM_LET_MARGIN + grid_x * (SPACE + BLOCK_SIZE) +
                            (grid_x / 3 * 3),
                        BLOCK_NUM_TOP_MARGIN + grid_y * (SPACE + BLOCK_SIZE) +
                            (grid_y / 3 * 3));
                    // erase the grid at first

                    tft.fillRoundRect(
                        BLOCK_LEFT_MARGIN + (BLOCK_SIZE + SPACE) * grid_x +
                            (grid_x / 3 * 3),
                        BLOCK_TOP_MARGIN + (BLOCK_SIZE + SPACE) * grid_y +
                            (grid_y / 3 * 3),
                        BLOCK_SIZE, BLOCK_SIZE, RADIUS, RED);
                    // fill in the new Digit
                    tft.println(tipValue);
                    if (sudoku.countResult()) {
                        // bingo!
                        gameover();
                        return;
                    }
                }
                delay(400);
                continue;
            }

            screen_x -= 90;
            int16_t grid_x = (310 - screen_y) / (BLOCK_SIZE + SPACE),
                    grid_y = (screen_x - 8) / (BLOCK_SIZE + SPACE); // range 0-8
            if (grid_y < 0 || grid_y > 8 || grid_x > 8 || grid_x < 0) {
                continue;
            }

            if (sudoku.isModifiable(grid_y, grid_x)) {
                if (currDigit == sudoku.getSolution(grid_y, grid_x)) {
                    sudoku.fillGrid(grid_y, grid_x, currDigit);
                    tft.setCursor(
                        BLOCK_NUM_LET_MARGIN + grid_x * (SPACE + BLOCK_SIZE) +
                            (grid_x / 3 * 3),
                        BLOCK_NUM_TOP_MARGIN + grid_y * (SPACE + BLOCK_SIZE) +
                            (grid_y / 3 * 3));
                    // erase the grid at first

                    tft.fillRoundRect(
                        BLOCK_LEFT_MARGIN + (BLOCK_SIZE + SPACE) * grid_x +
                            (grid_x / 3 * 3),
                        BLOCK_TOP_MARGIN + (BLOCK_SIZE + SPACE) * grid_y +
                            (grid_y / 3 * 3),
                        BLOCK_SIZE, BLOCK_SIZE, 4, RED);
                    // fill in the new Digit
                    tft.println(currDigit);

                    if (sudoku.countResult()) {
                        // bingo!
                        gameover();
                        return;
                    }
                } else { // wrong -> X
                    tft.fillRoundRect(
                        CROSS_SPACE + (BLOCK_SIZE + SPACE) * grid_x +
                            (grid_x / 3 * 3),
                        BLOCK_TOP_MARGIN + (BLOCK_SIZE + SPACE) * grid_y +
                            (grid_y / 3 * 3),
                        BLOCK_SIZE, BLOCK_SIZE, RADIUS, RED);
                    tft.drawLine(
                        (BLOCK_LEFT_MARGIN + CROSS_SPACE) +
                            (BLOCK_SIZE + SPACE) * grid_x + (grid_x / 3 * 3),
                        (BLOCK_TOP_MARGIN + CROSS_SPACE) +
                            (BLOCK_SIZE + SPACE) * grid_y + (grid_y / 3 * 3),
                        (BLOCK_SIZE + SPACE) * grid_x + (grid_x / 3 * 3) +
                            BLOCK_SIZE,
                        (BLOCK_TOP_MARGIN - CROSS_SPACE) +
                            (BLOCK_SIZE + SPACE) * grid_y + (grid_y / 3 * 3) +
                            BLOCK_SIZE,
                        TFT_RED);
                    tft.drawLine(
                        (BLOCK_LEFT_MARGIN + CROSS_SPACE) +
                            (BLOCK_SIZE + SPACE) * grid_x + (grid_x / 3 * 3),
                        (BLOCK_TOP_MARGIN - CROSS_SPACE) +
                            (BLOCK_SIZE + SPACE) * grid_y + (grid_y / 3 * 3) +
                            BLOCK_SIZE,
                        (BLOCK_SIZE + SPACE) * grid_x + (grid_x / 3 * 3) +
                            BLOCK_SIZE,
                        (BLOCK_TOP_MARGIN + CROSS_SPACE) +
                            (BLOCK_SIZE + SPACE) * grid_y + (grid_y / 3 * 3),
                        TFT_RED);
                    delay(300);
                    tft.fillRoundRect(
                        BLOCK_LEFT_MARGIN + (BLOCK_SIZE + SPACE) * grid_x +
                            (grid_x / 3 * 3),
                        BLOCK_TOP_MARGIN + (BLOCK_SIZE + SPACE) * grid_y +
                            (grid_y / 3 * 3),
                        BLOCK_SIZE, BLOCK_SIZE, RADIUS, RED);
                }
            }

            delay(200);
        }
    }
}

void processGame() {
    bool resume = false;
    difficultyMenu(resume);
    if (currentState == Menu) {
        return;
    }

    // if user doesn't choose to resume the game, then initialize a new game
    if (!resume) {
        sudoku.newGame(diff);
    }
    drawTheGrid(&tft, diff, sudoku);
    startGaming();
}

int main() {
    setup();
    while (true) {
        if (currentState == Menu) {
            mainMenu();
        }
        if (currentState == Game) {
            processGame();
        }
        if (currentState == Stat) {
            statMenu();
        }
    }
    return 0;
}
