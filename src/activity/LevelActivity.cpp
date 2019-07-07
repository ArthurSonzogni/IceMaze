#include <iostream>

#include <cmath>
#include <smk/Color.hpp>
#include "activity/LevelActivity.hpp"
#include "resources.hpp"

namespace {}  // namespace

LevelActivity::LevelActivity(smk::Screen& screen) : Activity(screen) {}
LevelActivity::~LevelActivity() {}

void LevelActivity::Draw() {
  screen().PoolEvents();
  auto& input = screen().input();

  if (input.IsKeyPressed(GLFW_KEY_ESCAPE)) {
    on_escape();
    return;
  }

  level.Step(screen(), on_win, on_lose);
  level.Draw(screen());

  screen().Display();
  screen().LimitFrameRate(60.0);
}
