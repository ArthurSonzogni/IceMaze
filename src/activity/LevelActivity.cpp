#include <iostream>

#include <cmath>
#include <smk/Color.hpp>
#include "activity/LevelActivity.hpp"
#include "resources.hpp"

namespace {}  // namespace

LevelActivity::LevelActivity(smk::Window& window) : Activity(window) {}
LevelActivity::~LevelActivity() {}

void LevelActivity::Draw() {
  window().PoolEvents();
  auto& input = window().input();

  if (input.IsKeyPressed(GLFW_KEY_ESCAPE)) {
    on_escape();
    return;
  }

  level.Step(window(), on_win, on_lose);
  level.Draw(window());

  window().Display();
  window().LimitFrameRate(60.0);
}
