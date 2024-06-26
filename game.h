
#ifndef GAME_H
#define GAME_H

#include <time.h>
#include "tetromino.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40

#define DROP_DELAY 60

class Game {
private:
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
  bool board_[BOARD_WIDTH][BOARD_HEIGHT];

  // Tetromino 클래스에 기본 생성자가 없어서, 아무 값으로 초기화해줌
  Tetromino curTetro=Tetromino::I;
  Tetromino nextTetro=Tetromino::I;
  Tetromino holdTetro=Tetromino::I;

  int cur_x;
  int cur_y;

  int delay;  // 프레임 count
  int lines;  // 부순 line의 수

  bool existHold;
  bool canHold;

  bool gameclear;
  bool gameover;
  bool esc;

  clock_t startTime;
  clock_t endTime;


  // name 모양의 테트로미노를 생성한다.
  Tetromino getTetro(std::string name);

  // 랜덤으로 테트로미노를 생성한다.
  Tetromino getRandomTetro();

  // 현재 테트로미노를 현재 위치에 고정시킨다.
  void fixTetro();

  // 화면에 게임 클리어 문구를 보여준다.
  void gameClear(std::string clearTime);

  // 화면에 게임 오버 문구를 보여준다.
  void gameOver();

  // 문자열 s를 y 줄에 중앙 정렬로 출력한다.
  void drawCenter(std::string s, int y);

  // 완성된 line을 제거한다.
  void checkAndRemoveLine();

  // BOARD의 y번째 줄이 꽊 찼는지 여부를 반환한다.
  bool isFullLine(int y);

  // curTetro가 (x, y)에 있을 때, 테두리 혹은 기존 블록에 부딪히는지 여부를 반환한다.
  bool isConflict(Tetromino& tetro, int x, int y);

  // 현재 테트로미노를 보관한다.
  void hold();

  // 현재 테트로미노를 바로 바닥에 고정시킨다 (하드 드롭)
  void hardDrop();

  // 현재 테트로미노를 빠르게 떨어뜨린다 (소프트 드롭)
  void softDrop();

  // 현재 시간(분/초/밀리초)을 --:--.-- 형태의 string으로 반환한다.
  std::string getCurTime();
  
public:
  // 게임의 한 프레임을 처리한다.
  void update();

  // 게임 화면을 그린다.
  void draw();

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit();

  Game();
};
#endif