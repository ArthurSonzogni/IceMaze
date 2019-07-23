#ifndef AUTO_RESET_H
#define AUTO_RESET_H

template <class T>
class AutoReset {
 public:
  AutoReset(T& value, T new_value) : value_(value), previous_value_(value) {
    value_ = new_value;
  }

  ~AutoReset() { value_ = previous_value_; }

 private:
  T& value_;
  T previous_value_;
};

#endif /* end of include guard: AUTO_RESET_H */
