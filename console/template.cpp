#include "console.h"

#define BOARD_SIZE 10
#define MOVE_DELAY 15
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define SNAKE_BODY_STRING "■"
#define APPLE_STRING "●"

int x = 0;
int y = 0;

void handleInput() {
  if (console::key(console::K_LEFT)) {
    x--;
  }
  if (console::key(console::K_RIGHT)) {
    x++;
  }
  if (console::key(console::K_UP)) {
    y--;
  }
  if (console::key(console::K_DOWN)) {
    y++;
  }
}

void restrictInScreen() {
  // x, y 위치를 화면의 최대 크기에서 벗어나지 않게 한다.
  if (x < 0)
    x = 0;
  if (x >= console::SCREEN_WIDTH)
    x = console::SCREEN_WIDTH - 1;
  if (y < 0)
    y = 0;
  if (y >= console::SCREEN_HEIGHT)
    y = console::SCREEN_HEIGHT - 1;
}

void drawStar() {
  // x, y 위치에 *을 그린다.
  console::draw(x, y, "*");
}

void game() {

  // 콘솔 라이브러리를 초기화한다.
  console::init();

  while (true) {
    // 화면을 지운다.
    console::clear();

    handleInput();
    restrictInScreen();
    drawStar();

    // 화면을 갱신하고 다음 프레임까지 대기한다.
    console::wait();
  }
}

int main() { game(); }