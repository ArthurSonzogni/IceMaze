#include "LevelGenerator.hpp"

#include <random>
#include <algorithm>
#include <ctime>
#include <iostream>

LevelGenerator::LevelGenerator(int width, int height)
    : width_(width),
      height_(height),
      random(static_cast<std::mt19937::result_type>(std::time(nullptr))) {
  candidates_.resize(std::max(10, 100));
  for (auto& it : candidates_) {
    it.level = Level::Random(width_, height_);
    it.score = 2;
  }
}

void LevelGenerator::Compute() {
  int n = candidates_.size();
  for (int i = 0; i < n; ++i) {
    Level level = candidates_[i].level;
    level.Mutate(random);

    candidates_.emplace_back();
    candidates_.back().level = level;
    candidates_.back().score = level.Evaluate();
  }

  std::shuffle(candidates_.begin(), candidates_.end(), random);
  std::sort(candidates_.begin(), candidates_.end());
  candidates_.resize(n);
}

Level LevelGenerator::Best() {
  return candidates_.front().level;
}

int LevelGenerator::BestScore() {
  return candidates_.front().score;
}
