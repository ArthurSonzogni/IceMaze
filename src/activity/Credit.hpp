#ifndef ACTIVITY_CREDIT
#define ACTIVITY_CREDIT

#include <smk/Text.hpp>
#include <string>
#include <vector>
#include "activity/Activity.hpp"
#include "activity/BackButton.hpp"

class Credit : public Activity {
 public:
  Credit(smk::Window&);
  ~Credit() override;
  void Draw() override;
  void OnEnter() override;
  std::function<void()> on_quit = [] {};

 private:
  glm::vec2 last_cursor_position;
  glm::vec2 view_;
  glm::vec2 view_speed_;
  BackButton back_button_;
};

#endif /* end of include guard: ACTIVITY_CREDIT */
