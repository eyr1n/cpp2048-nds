#include <array>
#include <random>
#include <utility>
#include <vector>

using std::array;
using std::pair;
using std::vector;

namespace cpp2048 {
enum class MoveDirection { UP, DOWN, LEFT, RIGHT, NONE };

class Game {
  int score_ = 0;
  int tiles_[4][4] = {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
  };

  std::random_device dev_;
  std::default_random_engine eng_;
  std::uniform_real_distribution<float> dist_;

  const float random() {
    return dist_(eng_);
  }

  const pair<int, int> add_tile() {
    vector<pair<int, int>> zero_tiles; // {y, x}

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (!tiles_[i][j]) {
          zero_tiles.push_back(std::make_pair(i, j));
        }
      }
    }

    int tile_pos = random() * zero_tiles.size();
    auto [y, x] = zero_tiles[tile_pos];
    tiles_[y][x] = random() < 0.9 ? 2 : 4;
    return zero_tiles[tile_pos];
  }

  void move_tiles(MoveDirection dir) {
    for (int i = 0; i < 4; i++) {
      int pointer = 0;

      for (int j = 0; j < 4; j++) {
        switch (dir) {
          case MoveDirection::UP:
            if (tiles_[j][i]) {
              tiles_[pointer][i] = tiles_[j][i];
              if (pointer != j) {
                tiles_[j][i] = 0;
              }
              pointer++;
            }
            break;

          case MoveDirection::DOWN:
            if (tiles_[3 - j][i]) {
              tiles_[3 - pointer][i] = tiles_[3 - j][i];
              if (pointer != j) {
                tiles_[3 - j][i] = 0;
              }
              pointer++;
            }
            break;

          case MoveDirection::LEFT:
            if (tiles_[i][j]) {
              tiles_[i][pointer] = tiles_[i][j];
              if (pointer != j) {
                tiles_[i][j] = 0;
              }
              pointer++;
            }
            break;

          case MoveDirection::RIGHT:
            if (tiles_[i][3 - j]) {
              tiles_[i][3 - pointer] = tiles_[i][3 - j];
              if (pointer != j) {
                tiles_[i][3 - j] = 0;
              }
              pointer++;
            }
            break;
        }
      }
    }
  }

  void merge_tiles(MoveDirection dir) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 3; j++) {
        switch (dir) {
          case MoveDirection::UP:
            if (tiles_[j][i] == tiles_[j + 1][i]) {
              tiles_[j][i] += tiles_[j + 1][i];
              score_ += tiles_[j][i];
              tiles_[j + 1][i] = 0;
              j++;
            }
            break;

          case MoveDirection::DOWN:
            if (tiles_[3 - j][i] == tiles_[2 - j][i]) {
              tiles_[3 - j][i] += tiles_[2 - j][i];
              score_ += tiles_[3 - j][i];
              tiles_[2 - j][i] = 0;
              j++;
            }
            break;

          case MoveDirection::LEFT:
            if (tiles_[i][j] == tiles_[i][j + 1]) {
              tiles_[i][j] += tiles_[i][j + 1];
              score_ += tiles_[i][j];
              tiles_[i][j + 1] = 0;
              j++;
            }
            break;

          case MoveDirection::RIGHT:
            if (tiles_[i][3 - j] == tiles_[i][2 - j]) {
              tiles_[i][3 - j] += tiles_[i][2 - j];
              score_ += tiles_[i][3 - j];
              tiles_[i][2 - j] = 0;
              j++;
            }
            break;
        }
      }
    }
    move_tiles(dir);
  }

  const bool can_move_tiles(MoveDirection dir) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 3; j++) {
        switch (dir) {
          case MoveDirection::UP:
            if ((!tiles_[j][i] && tiles_[j + 1][i]) ||
                (tiles_[j][i] == tiles_[j + 1][i] && tiles_[j][i])) {
              return true;
            }
            break;

          case MoveDirection::DOWN:
            if ((!tiles_[j + 1][i] && tiles_[j][i]) ||
                (tiles_[j][i] == tiles_[j + 1][i] && tiles_[j][i])) {
              return true;
            }
            break;

          case MoveDirection::LEFT:
            if ((!tiles_[i][j] && tiles_[i][j + 1]) ||
                (tiles_[i][j] == tiles_[i][j + 1] && tiles_[i][j])) {
              return true;
            }
            break;

          case MoveDirection::RIGHT:
            if ((!tiles_[i][j + 1] && tiles_[i][j]) ||
                (tiles_[i][j] == tiles_[i][j + 1] && tiles_[i][j])) {
              return true;
            }
            break;
        }
      }
    }
    return false;
  }

public:
  Game() : eng_(dev_()) {}

  void reset() {
    score_ = 0;
    for (auto &i : tiles_) {
      for (auto &j : i) {
        j = 0;
      }
    }
    add_tile();
    add_tile();
  }

  const pair<int, int> operate(MoveDirection dir) {
    if (dir != cpp2048::MoveDirection::NONE && can_move_tiles(dir)) {
      move_tiles(dir);
      merge_tiles(dir);
      return add_tile();
    }
    return {-1, -1};
  }

  const int &get_score() {
    return score_;
  }

  const int &get_tile(int y, int x) {
    return tiles_[y][x];
  }
};
} // namespace cpp2048
