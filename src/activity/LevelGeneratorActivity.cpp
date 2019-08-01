#include "activity/LevelGeneratorActivity.hpp"
#include <smk/Color.hpp>

LevelGeneratorActivity::LevelGeneratorActivity(smk::Screen& screen)
    : Activity(screen), level_activity_(screen) {}

void LevelGeneratorActivity::OnEnter() {
  generator = std::make_unique<LevelGenerator>(width, height);
  best_score_timeout_ = 60;
}

void LevelGeneratorActivity::Draw() {
  if (screen().input().IsKeyPressed(GLFW_KEY_ESCAPE)) {
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
  screen().PoolEvents();

  if (best_score_timeout_ > 1)
    generator->Compute(screen());

  int best_score = generator->BestScore();
  if (best_score_ != best_score) {
    best_score_ = best_score;
    best_score_timeout_ = 60;
  }

  best_score_timeout_--;
  level_activity_.level = generator->Best();
  level_activity_.level.Init(screen());

  screen().Clear(smk::Color::Black);
  level_activity_.level.Draw(screen());
  screen().Display();
}
