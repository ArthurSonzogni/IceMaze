#include <iostream>

#include <smk/Shape.hpp>

#include <cmath>
#include <smk/Color.hpp>
#include <smk/Vibrate.hpp>
#include "activity/LevelActivity.hpp"
#include "resources.hpp"

namespace {}  // namespace

LevelActivity::LevelActivity(smk::Window& window)
    : Activity(window), back_button_(window) {
  back_button_.on_quit = [&] { on_escape(); };
}
LevelActivity::~LevelActivity() {}

void LevelActivity::OnEnter() {
  ready_for_input_ = false;
}

void LevelActivity::Step() {
  auto& input = window().input();

  if (input.IsKeyPressed(GLFW_KEY_ESCAPE)) {
    on_escape();
    return;
  }

  Direction direction = Direction::None;

  // clang-format off
       if (input.IsKeyHold(GLFW_KEY_UP))    direction = Direction::Up;
  else if (input.IsKeyHold(GLFW_KEY_DOWN))  direction = Direction::Down;
  else if (input.IsKeyHold(GLFW_KEY_LEFT))  direction = Direction::Left;
  else if (input.IsKeyHold(GLFW_KEY_RIGHT)) direction = Direction::Right;
  // clang-format on

  if (!ready_for_input_ || level.IsMoving()) {
    ready_for_input_ = true;
    cursor_anchor_ = input.cursor();
  } else {
    if (input.IsCursorPressed()) {
      cursor_anchor_ = input.cursor();
      ready_for_input_ = true;
    } else if (ready_for_input_ && input.IsCursorHold()) {
      float trigger = 32.f;
      glm::vec2 cursor = input.cursor();
      glm::vec2 diff = cursor - cursor_anchor_;
      if (glm::length(diff) > trigger) {
        cursor_anchor_ = cursor;
        diff /= trigger;
        if (std::abs(diff.x) > std::abs(diff.y))
          direction = diff.x < 0.f ? Direction::Left : Direction::Right;
        else
          direction = diff.y < 0.f ? Direction::Up : Direction::Down;
      }
    }
  }

  if (direction != Direction::None)
    smk::Vibrate(10);

  level.Step(direction, on_win, on_lose);
  back_button_.Step();
}

void LevelActivity::Draw() {
  level.Draw(window());
  back_button_.Draw();
}
