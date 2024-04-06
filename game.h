
#ifndef GAME_H
#define GAME_H

#include "tetromino.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40

#define DROP_DELAY 10

class Game {
private:
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
  bool board_[BOARD_WIDTH][BOARD_HEIGHT];

  // static 테트로미노를 참조
  Tetromino curTetro;
  Tetromino nextTetro;

  int cur_x;
  int cur_y;

  int delay;  // 프레임 count
  int line;  // 부순 line의 수

  bool gameover;

  // 랜덤으로 테트로미노를 생성한다.
  Tetromino getRandomTetro();

  // curTetro를 한 줄 아래로 내릴 수 있는지 여부를 반환한다.
  bool canMoveDown();

  // curTetro를 현재 위치에 고정시킨다.
  void fixTetro();

  // 화면에 게임 클리어 문구를 보여준다.
  void gameClear();

  // 화면에 게임 오버 문구를 보여준다.
  void gameOver();
  
public:
  Game();

  // 게임의 한 프레임을 처리한다.
  void update();

  // 게임 화면을 그린다.
  void draw();

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit();
};
#endif