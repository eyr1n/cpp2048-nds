#include <cstdio>
#include <nds.h>

#include "cpp2048.hpp"

int main() {
  consoleDemoInit();

  cpp2048::Game game;

  auto display_tiles = [&](const int y = -1, const int x = -1) {
    iprintf("\n cpp2048-nds       by @rin4046\n\n\n");
    iprintf(" score: %d\n\n", game.get_score());

    iprintf(" +------+------+------+------+\n");
    for (int i = 0; i < 4; i++) {
      iprintf(" |      |      |      |      |\n ");
      for (int j = 0; j < 4; j++) {
        int num = game.get_tile(i, j);
        int color = 31 + (num > 4) + (num > 16) + (num > 64) * 2 + (num > 256);
        if (i == y && j == x) {
          iprintf("|\e[%dm[%4d]\e[39m", color, num);
        } else if (game.get_tile(i, j)) {
          iprintf("|\e[%dm %4d \e[39m", color, num);
        } else {
          iprintf("|      ");
        }
      }
      iprintf("|\n |      |      |      |      |\n");
      iprintf(" +------+------+------+------+\n");
    }
  };

  game.reset();
  display_tiles();

  while (true) {
    auto dir = cpp2048::MoveDirection::NONE;
    swiWaitForVBlank();
    scanKeys();

    switch (keysDown()) {
      case KEY_UP:
        dir = cpp2048::MoveDirection::UP;
        break;
      case KEY_DOWN:
        dir = cpp2048::MoveDirection::DOWN;
        break;
      case KEY_LEFT:
        dir = cpp2048::MoveDirection::LEFT;
        break;
      case KEY_RIGHT:
        dir = cpp2048::MoveDirection::RIGHT;
        break;
      case KEY_START:
        game.reset();
        consoleClear();
        display_tiles();
        break;
    }

    auto [y, x] = game.operate(dir);
    if (y != -1) {
      consoleClear();
      display_tiles(y, x);
    }
  }

  return 0;
}
