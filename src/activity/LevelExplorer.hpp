#ifndef ACTIVITY_LEVEL_EXPLORER_HPP
#define ACTIVITY_LEVEL_EXPLORER_HPP

#include <smk/Text.hpp>
#include <string>
#include <vector>
#include "activity/Activity.hpp"

class LevelExplorer : public Activity {
 public:
  LevelExplorer(smk::Screen&);
  ~LevelExplorer() override = default;
  void Draw() override;
  void OnEnter() override;

  std::vector<std::string> entries;
  int choice = 0;
  int save = 0;

  std::function<void()> on_enter = []{};
  std::function<void()> on_escape = []{};

 private:
  float yview = 480 * 0.5;
};

#endif /* end of include guard: ACTIVITY_LEVEL_EXPLORER_HPP */
