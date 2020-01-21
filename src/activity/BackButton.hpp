// Copyright 2019 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#ifndef ACTIVITY_BACK_BUTTON_HPP
#define ACTIVITY_BACK_BUTTON_HPP

#include <memory>
#include "activity/Activity.hpp"

class BackButton : public Activity {
 public:
  BackButton(smk::Window& window) : Activity(window) {}
  ~BackButton() override = default;

  void Step() override;
  void Draw() override;
  std::function<void()> on_quit = []{};

 private:
  bool back_button_hover = false;
  glm::vec2 back_button_position;
};

#endif /* end of include guard: ACTIVITY_BACK_BUTTON_HPP */
