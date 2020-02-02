#ifndef LEVEL_HEADER_GUARD_HPP
#define LEVEL_HEADER_GUARD_HPP

#include <fstream>
#include <functional>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <smk/Sound.hpp>
#include <smk/RenderTarget.hpp>

struct Position {
  int x,y;
  bool operator<(const Position other) const;
};

enum class Direction {
  Up,
  Down,
  Left,
  Right,
  None,
};

class Level {
 public:
  Level();
  Level(std::string filename);

  int height() { return height_; }
  int width() { return width_; }

  char getCase(Position pos);
  void setCase(Position pos, char c);

  void Init(smk::RenderTarget& surface);
  void Step(Direction input_direction,
            std::function<void()> on_win,
            std::function<void()> on_lose);
  void Draw(smk::RenderTarget& surface);

  Direction GetDirection() { return direction_; }

  // For the evolutionnary algorithm.
  int Evaluate();
  static Level Random(int width, int height);
  void Mutate(std::mt19937& random);

 private:
  Direction Bounce(Direction direction, char bouncer);
  static bool CanBounce(Direction direction, char bouncer);
  void Stop();

  void NextStep(std::function<void()> on_win, std::function<void()> on_lose);
  void AnimationStep();
  void teleport();
  void getAutoTileInfo(int& gh, int& dh, int& db, int& gb, int xx, int yy);
  void UpdateView(smk::RenderTarget& surface);

  std::string title_;
  std::string author_;
  int width_;
  int height_;
  Position starting_point_;
  std::vector<int> cases_;

  int nb_cle = 0;
  bool is_moving_ = false;
  int anim = 0;
  Direction direction_ = Direction::Up;

  Position next_position;
  Position current_position;
  Position pos; // Interpolated position.

  int tempo = 0;

  glm::vec2 view_;
  float view_speed = 1.0;

  bool in_evaluate_ = false;
  static std::vector<smk::Sound> sounds;
  static int sound_index;
  void PlaySoundInternal(const smk::SoundBuffer&);
};

int int_of_string(std::string nstr);

#endif
