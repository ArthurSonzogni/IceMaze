#ifndef LEVEL_HEADER_GUARD_HPP
#define LEVEL_HEADER_GUARD_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <vector>

typedef struct {
  int x;
  int y;
} position;

namespace smk {
  class Screen;
}

class Level {
 public:
  Level();
  Level(std::string filename);
  char getCase(int x, int y);
  void setCase(int x, int y, int c);
  void getAutoTileInfo(int& gh, int& dh, int& db, int& gb, int xx, int yy);
  void teleport(position& pos);

  void Step(smk::Screen& screen,
            std::function<void()> on_win,
            std::function<void()> on_lose);
  void Draw(smk::Screen& screen);

  int height() { return height_; }
  int width() { return width_; }
  position starting_point() { return starting_point_; }
  std::string author() { return author_; }
  std::string title() { return title_; }

 private:
  std::string title_;
  std::string author_;
  int width_;
  int height_;
  position starting_point_;
  std::vector<int> cases_;

  enum class Direction {
    Up,
    Down,
    Left,
    Right,
  };

  int nb_cle = 0;
  bool ismoving = false;
  bool mouvement = false;
  int mouvement_compteur = 0;
  Direction direction = Direction::Up;
  position pos;

  int tempo = 0;

  float view_x;
  float view_y;
  float view_speed = 1.0;

  static Direction Bounce(Direction direction, char bouncer);
  static bool CanBounce(Direction direction, char bouncer);
  void Stop();

  void NextStep(smk::Screen& screen,
                    std::function<void()> on_win,
                    std::function<void()> on_lose);
  void ContinueMovement();
  bool GetNewDirectionFromInput(smk::Screen& screen);
};

int int_of_string(std::string nstr);

#endif
