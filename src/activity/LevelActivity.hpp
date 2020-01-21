#ifndef LEVEL_ACTIVITY_HPP
#define LEVEL_ACTIVITY_HPP

#include "activity/Activity.hpp"
#include "activity/BackButton.hpp"
#include "Level.hpp"

class LevelActivity : public Activity {
 public:
  LevelActivity(smk::Window&);
  ~LevelActivity() override;
  void OnEnter() override;
  void Step() override;
  void Draw() override;

  std::function<void()> on_win = []{};
  std::function<void()> on_lose = []{};
  std::function<void()> on_escape = []{};

  Level level;
 private:
  glm::vec2 cursor_anchor_;
  bool ready_for_input_ = false;
  BackButton back_button_;
};

#endif /* end of include guard: LEVEL_ACTIVITY_HPP */
