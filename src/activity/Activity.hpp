#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <smk/Screen.hpp>

class Activity {
 public:
  Activity(smk::Screen& screen) : screen_(screen) {}
  virtual ~Activity() = default;

  virtual void Draw() = 0;

  virtual void OnEnter() {}
  virtual void OnQuit() {}

 protected:
  smk::Screen& screen() { return screen_; }

 private:
  smk::Screen& screen_;
};

#endif /* end of include guard: ACTIVITY_H */
