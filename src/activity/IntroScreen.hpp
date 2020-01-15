#ifndef ACTIVITY_INTRO_SCREEN
#define ACTIVITY_INTRO_SCREEN

#include <smk/Text.hpp>
#include <string>
#include <vector>
#include "activity/Activity.hpp"

class IntroScreen : public Activity {
 public:
  IntroScreen(smk::Window&);
  ~IntroScreen() override;
  void Draw() override;
  void OnEnter() override;

  std::function<void()> on_enter = [] {};

 private:
  int time = 0;
  float zoom_;
};

#endif /* end of include guard: ACTIVITY_INTRO_SCREEN */
