#ifndef LEVEL_GENERATOR_SIZE_SELECTOR_HPP
#define LEVEL_GENERATOR_SIZE_SELECTOR_HPP

#include "activity/Activity.hpp"
#include <memory>

class LevelGeneratorSizeSelector : public Activity {
  public:
   LevelGeneratorSizeSelector(smk::Screen& screen);
   void Draw() override;

   std::function<void()> on_enter = [] {};
   std::function<void()> on_escape = [] {};

   int width = 10;
   int height = 10;

  private:
   void UpdateView();
   float view_x_ = 0;
   float view_y_ = 0;
   float view_zoom_ = 0;
};

#endif /* end of include guard: LEVEL_GENERATOR_SIZE_SELECTOR_HPP */
