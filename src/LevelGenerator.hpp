#ifndef ICEMAZE_LEVEL_GENERATOR
#define ICEMAZE_LEVEL_GENERATOR

#include <random>
#include "Level.hpp"

class LevelGenerator {
 public:
  LevelGenerator(int width, int height);
  void Compute();
  int BestScore();
  Level Best();

 private:
  int width_;
  int height_;
  struct Candidate {
    int score;
    Level level;
    bool operator<(const Candidate& other) const { return score > other.score; }
  };
  std::vector<Candidate> candidates_;
  std::mt19937 random;
};

#endif /* end of include guard: ICEMAZE_LEVEL_GENERATOR */
