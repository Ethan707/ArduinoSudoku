#include "sudoku.h"

#include <Arduino.h>
using namespace std;

Sudoku::Sudoku(difficulty level) { this->level = level; }

void Sudoku::newGame(difficulty level) {
    this->level = level;
    time = 0;
    // initialize the grid
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            this->grid[i][i] = 0;
        }
    }

    initiate_grid();
    getRandomNumberList();
    getRandomSpaceList();
    solver();
    // print_grid();
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            solution[i][j] = this->grid[i][j];
        }
    }
    generator();
}

void Sudoku::initiate_grid() {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            grid[i][j] = 0;
}

bool Sudoku::isExist(int row, int colum, int num) {
    for (int i = 0; i < 9; i++) {
        if (this->grid[row][i] == num) {
            return true;
        }
    }
    for (int i = 0; i < 9; i++) {
        if (this->grid[i][colum] == num) {
            return true;
        }
    }
    int square_row = row / 3;
    int square_colum = colum / 3;
    for (int i = square_row * 3; i < (square_row + 1) * 3; i++) {
        for (int j = square_colum * 3; j < (square_colum + 1) * 3; j++) {
            if (this->grid[i][j] == num) {
                return true;
            }
        }
    }
    return false;
}

void Sudoku::getRandomNumberList() {
    for (int i = 0; i < 9; i++) {
        NumberList[i] = i + 1;
    }
    for (int i = 8; i >= 0; i--) {
        int randomIndex = random(1, i + 1); // 1 to 9
        int tmp = NumberList[randomIndex];
        NumberList[randomIndex] = NumberList[i];
        NumberList[i] = tmp;
    }
}

void Sudoku::getRandomSpaceList() {
    for (int i = 0; i < 81; i++) {
        SpaceList[i] = i;
    }

    for (int i = 80; i >= 0; i--) {
        int randomIndex = random(1, i + 1);
        int tmp = SpaceList[randomIndex];
        SpaceList[randomIndex] = SpaceList[i];
        SpaceList[i] = tmp;
    }
}

bool Sudoku::findEmpty(int &row, int &colum) {
    for (row = 0; row < 9; row++) {
        for (colum = 0; colum < 9; colum++) {
            if (this->grid[row][colum] == 0) {
                return true;
            }
        }
    }
    return false;
}

bool Sudoku::solver() {
    // to generate a sudoku game
    this->mode = 1;
    isFound = 0;
    dfs_find_solution();
    isFound = 0;
}

void Sudoku::dfs_find_solution() {
    int row, colum;
    // mode 1 generate a game
    if (this->mode == 1) {
        if (isFound)
            return;
        if (!findEmpty(row, colum)) {
            isFound = 1;
            return; // a solution is found
        }
    }
    // mode 2 generate puzzles
    if (this->mode == 2) {
        if (puzzleNum > 1)
            return;
        if (!findEmpty(row, colum)) {
            puzzleNum++;
            return;
        }
    }
    for (int i = 0; i < 9; i++) {
        if (!isExist(row, colum, NumberList[i])) {
            this->grid[row][colum] = NumberList[i];
            dfs_find_solution();
            if (isFound)
                return;
            this->grid[row][colum] = 0;
        }
    }
}

void Sudoku::getTips(int &grid_y, int &grid_x, int &num) {
    int randNumber = random(1, this->leftNum + 1);
    int count = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            if (this->grid[i][j] == 0) {
                count++;
                if (count == randNumber) {
                    fillGrid(i, j, this->solution[i][j]);
                    grid_y = i;
                    grid_x = j;
                    num = solution[i][j];
                    return;
                }
            }
        }
}

void Sudoku::generator() {
    int preservedNum;
    if (this->level == TOUGH)
        preservedNum = 30;
    if (this->level == NORMAL)
        preservedNum = 35;
    if (this->level == EASY)
        preservedNum = 40;
    this->leftNum = 81 - preservedNum;
    int count = 81;
    for (int i = 0; i < 81 && count > preservedNum; i++) {
        int x = (this->SpaceList[i]) / 9;
        int y = (this->SpaceList[i]) % 9;
        int temp = this->grid[x][y];
        this->grid[x][y] = 0;
        this->mode = 2;
        this->puzzleNum = 0;
        dfs_find_solution();
        if (this->puzzleNum == 1) {
            count--;
        } else {
            this->grid[x][y] = temp;
        }
    }
}

int Sudoku::getValue(int x, int y) { return this->grid[x][y]; }

int Sudoku::getSolution(int x, int y) { return this->solution[x][y]; }

bool Sudoku::isModifiable(int x, int y) {
    if (this->grid[x][y] == 0) {
        return true;
    }
    return false;
}

void Sudoku::fillGrid(int x, int y, int num) { this->grid[x][y] = num; }

void Sudoku::setResumeGrid(int x, int y, int num) { this->grid[x][y] = num; }

void Sudoku::setResumeSolution(int x, int y, int num) {
    this->solution[x][y] = num;
}

void Sudoku::setResumeLevel(int x) {
    if (x == 0) {
        level = EASY;
    } else if (x == 1) {
        level = NORMAL;
    } else {
        level = TOUGH;
    }
}

bool Sudoku::countResult() {
    this->leftNum--;
    if (this->leftNum == 0)
        return true;
    else
        return false;
}

void Sudoku::print_grid() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            Serial.print(this->grid[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }
    // system("pause");
    return;
}

void Sudoku::setTime(unsigned long time) { this->time = time; }

unsigned long Sudoku::getTime() { return time; }

int Sudoku::getLevel() { return level; }

void Sudoku::setLevel(int l) { level = l; }