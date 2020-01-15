#ifndef LEVEL_GENERATOR_ACTIVITY
#define LEVEL_GENERATOR_ACTIVITY

#include "LevelGenerator.hpp"
#include "activity/LevelActivity.hpp"
#include <memory>

class LevelGeneratorActivity : public Activity {
  public:
   LevelGeneratorActivity(smk::Window& window);
   void OnEnter() override;
   void Draw() override;
   std::function<void()> on_escape = []{};
   int width = 10;
   int height = 10;

  private:
   LevelActivity level_activity_;
   std::unique_ptr<LevelGenerator> generator;
   int best_score_ = 0;
   int best_score_timeout_ = 60;
   void Start();
   bool started_ = false;
};

#endif /* end of include guard: LEVEL_GENERATOR_ACTIVITY */
