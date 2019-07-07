#ifndef ACTIVITY_MENU_HPP
#define ACTIVITY_MENU_HPP

#include <smk/Text.hpp>
#include <string>
#include <vector>
#include "activity/Activity.hpp"

class Menu : public Activity {
 public:
  Menu(smk::Screen&);
  ~Menu() override;
  void Draw() override;

  std::vector<std::string> entries;
  int selected = 0;

  std::function<void()> on_enter = [] {};
  std::function<void()> on_escape = [] {};
  std::function<void()> on_change = [] {};

 private:
  int time = 0;
};

#endif /* end of include guard: ACTIVITY_MENU_HPP */
