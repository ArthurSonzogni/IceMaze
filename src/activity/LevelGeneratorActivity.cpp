#include "activity/LevelGeneratorActivity.hpp"
#include <smk/Color.hpp>
#include <smk/Input.hpp>
#include "resources.hpp"

LevelGeneratorActivity::LevelGeneratorActivity(smk::Window& window)
    : Activity(window), level_activity_(window) {}

void LevelGeneratorActivity::OnEnter() {
  generator = std::make_unique<LevelGenerator>(width, height);
  best_score_timeout_ = 60;
}

void LevelGeneratorActivity::Draw() {
  if (window().input().IsKeyPressed(GLFW_KEY_ESCAPE)) {
    on_escape();
    return;
  }

  if (best_score_timeout_ == 0) {
    level_activity_.on_lose = [&] { best_score_timeout_ = 1; };
    level_activity_.on_win = [&] {
      best_score_timeout_ = 60;
      generator = std::make_unique<LevelGenerator>(width, height);
    };
    level_activity_.Draw();
    return;
  }

  if (best_score_timeout_ > 1)
    generator->Compute();

  int best_score = generator->BestScore();
  if (best_score_ != best_score) {
    best_score_ = best_score;
    best_score_timeout_ = 60;
  }

  best_score_timeout_--;
  level_activity_.level = generator->Best();
  level_activity_.level.Init(window());

  level_activity_.level.Draw(window());
}
