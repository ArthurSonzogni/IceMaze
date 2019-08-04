#ifndef ACTIVITY_CREDIT
#define ACTIVITY_CREDIT

#include <smk/Text.hpp>
#include <string>
#include <vector>
#include "activity/Activity.hpp"

class Credit : public Activity {
 public:
  Credit(smk::Screen&);
  ~Credit() override;
  void Draw() override;
  void OnEnter() override;
  std::function<void()> on_quit = [] {};

 private:
  int view_x = 0;
  int view_y = 0;
};

#endif /* end of include guard: ACTIVITY_CREDIT */
