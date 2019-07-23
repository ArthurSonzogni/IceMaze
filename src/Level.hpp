#ifndef LEVEL_HEADER_GUARD_HPP
#define LEVEL_HEADER_GUARD_HPP

#include <fstream>
#include <functional>
#include <random>
#include <sstream>
#include <string>
#include <vector>

struct Position {
  int x;
  int y;

  // clang-format off
  bool operator<(const Position other) const {
    if (x < other.x) return true;
    if (x > other.x) return false;
    if (y < other.y) return true;
    if (y > other.y) return false;
    return false;
  }
  // clang-format on
};

namespace smk {
class Screen;
}

enum class Direction {
  Up,
  Down,
  Left,
  Right,
};

class Level {
 public:
  Level();
  Level(std::string filename);

  char getCase(Position pos);
  void setCase(Position pos, char c);

  void Init(smk::Screen& screen);
  void Step(smk::Screen& screen,
            std::function<void()> on_win,
            std::function<void()> on_lose);
  void Draw(smk::Screen& screen);

  int height() { return height_; }
  int width() { return width_; }
  Position starting_point() { return starting_point_; }
  std::string author() { return author_; }
  std::string title() { return title_; }

  int Evaluate(smk::Screen& screen);
  static Level Random(int width, int height);
  void Mutate(std::mt19937& random);

 private:
  Direction Bounce(Direction direction, char bouncer);
  static bool CanBounce(Direction direction, char bouncer);
  void Stop();

  void NextStep(smk::Screen& screen,
                std::function<void()> on_win,
                std::function<void()> on_lose);
  void AnimationStep();
  bool GetNewDirectionFromInput(smk::Screen& screen);
  void teleport();
  void getAutoTileInfo(int& gh, int& dh, int& db, int& gb, int xx, int yy);
  void UpdateView(smk::Screen& screen);

  std::string title_;
  std::string author_;
  int width_;
  int height_;
  Position starting_point_;
  std::vector<int> cases_;

  int nb_cle = 0;
  bool ismoving = false;
  bool mouvement = false;
  int anim = 0;
  Direction direction = Direction::Up;

  Position next_position;
  Position current_position;
  Position pos;

  int tempo = 0;

  float view_x;
  float view_y;
  float view_speed = 1.0;

  bool in_evaluate_ = false;
};

int int_of_string(std::string nstr);

#endif
