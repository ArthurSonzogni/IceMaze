#ifndef ACTIVITY_MENU
#define ACTIVITY_MENU

#include <smk/Text.hpp>
#include <string>
#include <vector>
#include "activity/Activity.hpp"
#include "activity/BackButton.hpp"

class Menu : public Activity {
 public:
  Menu(smk::Window&);
  ~Menu() override = default;
  void Step() override;
  void Draw() override;
  void OnEnter() override;

  std::vector<std::string> entries;
  int selected = 0;
  int save = 9999;

  std::function<void()> on_change = []{};
  std::function<void()> on_enter = []{};
  std::function<void()> on_escape;

 private:
  float total_scrolling_ = 0.f;
  float scrolling_ = 0.f;
  float cursor_scrolling_ = 0.f;
  float cursor_scrolling_speed_ = 0.f;

  float first_cursor_position = 0.f;
  float last_cursor_position = 0.f;

  bool has_pressed_ = false;

  struct EntryData {
    bool hover = false;
    float hover_position = 0.f;
    float hover_alpha = 0.f;
    glm::vec2 position;
  };
  std::vector<EntryData> entry_data_;

  bool focusable = false;
  int focus_step = 0;

  bool is_using_keyboard_ = true;
  glm::vec2 still_cursor = {0.f, 0.f};

  float button_width_ = 0.f;
  float button_height_ = 0.f;

  BackButton back_button_;
  bool back_button_pressed_ = false;
};

#endif /* end of include guard: ACTIVITY_MENU */
