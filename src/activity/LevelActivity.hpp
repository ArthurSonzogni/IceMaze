#ifndef LEVEL_ACTIVITY_HPP
#define LEVEL_ACTIVITY_HPP

#include "activity/Activity.hpp"
#include "Level.hpp"

class LevelActivity : public Activity {
 public:
  LevelActivity(smk::Window&);
  ~LevelActivity() override;
  void Draw() override;

  std::function<void()> on_win = []{};
  std::function<void()> on_lose = []{};
  std::function<void()> on_escape = []{};

  Level level;
};

#endif /* end of include guard: LEVEL_ACTIVITY_HPP */
