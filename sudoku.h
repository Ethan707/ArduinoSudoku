#ifndef _SUDOKU_H_
#define _SUDOKU_H_

#define INITIAL_VALUE 0
#include "global_define.h"

class Sudoku {
  private:
    int grid[9][9];
    int solution[9][9];
    int NumberList[9];
    int SpaceList[81];
    int mode;
    bool isFound;
    int puzzleNum;
    difficulty level;
    int leftNum;
    unsigned long time;

  public:
    Sudoku(difficulty level);
    void newGame(difficulty level);
    int solutionCounter();
    bool solver();
    void generator();
    void getRandomNumberList();
    void getRandomSpaceList();
    void dfs_find_solution();
    bool isExist(int row, int colum, int num);
    void initiate_grid();
    void print_grid();
    bool findEmpty(int &row, int &colum);
    int getValue(int x, int y);
    int getSolution(int x, int y);
    bool isModifiable(int x, int y);
    void fillGrid(int x, int y, int num);
    void setResumeSolution(int x, int y, int num);
    void setResumeGrid(int x, int y, int num);
    void setResumeLevel(int x);
    bool countResult();
    void getTips(int &grid_y, int &grid_x, int &num);
    void setTime(unsigned long time);
    unsigned long getTime();
    int getLevel();
    void setLevel(int l);
};

#endif