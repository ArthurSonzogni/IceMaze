#include "activity/Menu.hpp"

#include <fstream>
#include <smk/Color.hpp>
#include <smk/Input.hpp>
#include <smk/Shape.hpp>
#include <smk/Text.hpp>
#include <smk/Vibrate.hpp>
#include <smk/Window.hpp>
#include <sstream>
#include "Shape.hpp"
#include "resources.hpp"

const float item_space = 96.f;
const float item_height = 80.f;
const float button_alpha = 0.3f;
const float button_alpha_hover = 0.6f;
//const float button_alpha_hover = 0.6f;
const float scroll_margin = item_space - item_height;
const glm::vec4 text_color = {0.f, 0.f, 0.f, 1.f};
const glm::vec4 text_color_hover = {0.f, 0.f, 0.f, 1.f};

Menu::Menu(smk::Window& window) : Activity(window), back_button_(window) {
  back_button_.on_quit = [&] {
    back_button_pressed_ = true;
    on_escape();
  };
}

void Menu::Step() {
  if (on_escape) {
    back_button_pressed_ = false;
    back_button_.Step();
    if (back_button_pressed_)
      return;
  }

  total_scrolling_ = scrolling_ + cursor_scrolling_;

  auto& input = window().input();
  auto cursor = input.cursor();
  cursor -= window().dimension() * 0.5f;
  cursor.y += scrolling_;

  int previous_choice = selected;

  bool click = false;
  if (input.IsCursorPressed()) {
    first_cursor_position = cursor.y;
    last_cursor_position = cursor.y;
    has_pressed_ = true;
    focusable = true;
    focus_step = 0;
  } else if (has_pressed_) {
    if (input.IsCursorHold()) {
      auto diff = (last_cursor_position - cursor).y;
      last_cursor_position = cursor.y;
      cursor_scrolling_ = first_cursor_position - last_cursor_position;
      cursor_scrolling_speed_ += (diff - cursor_scrolling_speed_) * 0.5f;

      if (focusable) {
        focus_step++;
        if (glm::length(cursor_scrolling_) > 10.f) {
          focusable = false;
          focus_step = 0;
        }
      }
    } else if (input.IsCursorReleased()) {
      scrolling_ += cursor_scrolling_;
      cursor_scrolling_ -= cursor_scrolling_;
      if (focusable)
        click = true;
      focusable = false;
    } else {
      focus_step = 0;
      scrolling_ += cursor_scrolling_speed_;
      cursor_scrolling_speed_ *= 0.95f;
    }
  }

  if (glm::length(still_cursor - input.cursor()) > 1.f)
    is_using_keyboard_ = false;

  if (input.IsKeyPressed(GLFW_KEY_UP)) {
    is_using_keyboard_ = true;
    still_cursor = input.cursor();
    selected--;
  }
  if (input.IsKeyPressed(GLFW_KEY_DOWN)) {
    is_using_keyboard_ = true;
    still_cursor = input.cursor();
    selected++;
  }

  selected =
      std::max(0, std::min(std::min(save, int(entries.size() - 1)), selected));
  if (selected != previous_choice) {
    PlaySound(sb_menu_change);
    on_change();
  }

  if (input.IsKeyPressed(GLFW_KEY_ESCAPE) && on_escape) {
    on_escape();
  }

  // mise a jour de la vue;
  if (is_using_keyboard_) {
    float yview_target = selected * item_space;
    scrolling_ += (yview_target - scrolling_) * 0.1f;
  }

  bool selected_by_touch = false;

  button_width_ = window().width() * 0.8;
  button_height_ = item_height;

  entry_data_.resize(entries.size());

  // dessin des niveaux
  for (int i = 0; i < (int)entries.size(); i++) {
    entry_data_[i].position = glm::vec2(0.f, item_space * i);

    if (i > std::min(save, int(entries.size() - 1))) {
      entry_data_[i].hover_position = 0.f;
      entry_data_[i].hover_alpha = 0.1f;
      continue;
    }

    bool hover;
    if (is_using_keyboard_) {
      hover = (selected == i);
    } else {
      auto cursor_relative =
          (cursor + cursor_scrolling_ - entry_data_[i].position) * 2.f;
      bool& cursor_hover = entry_data_[i].hover;
      bool hover_previous = cursor_hover;
      cursor_hover = cursor_relative.x > -button_width_ &&
              cursor_relative.x < +button_width_ &&
              cursor_relative.y > -button_height_ &&
              cursor_relative.y < +button_height_;

      hover = cursor_hover;
      if (cursor_hover && !hover_previous) {
        selected = i;
        on_change();
      }
      if (hover && click) {
        smk::Vibrate(5);
        selected_by_touch = true;
        selected = i;
      }
    }

    float f = 8.0 * (focus_step / (4.0 + focus_step));
    entry_data_[i].hover_position +=
        (hover * (4.f - f) - entry_data_[i].hover_position) * 0.3f;
    entry_data_[i].hover_alpha += ((hover ? button_alpha_hover : button_alpha) -
                                   entry_data_[i].hover_alpha) *
                                  0.4f;
  }

  if (input.IsKeyPressed(GLFW_KEY_ENTER) || selected_by_touch) {
    PlaySound(sb_menu_select);
    on_enter();
  }

  // Apply scrolling bounds.
  {
    float min_scrolling =
        -item_height * 0.5f + window().height() * 0.5f - scroll_margin - 32 * 3;
    float max_scrolling = +item_height * 0.5f +
                          (entries.size() - 1) * item_space -
                          window().height() * 0.5f + scroll_margin;
    float scrolling_target =
        std::max(min_scrolling, std::min(max_scrolling, scrolling_));
    scrolling_ += (scrolling_target - scrolling_) * 0.3f;
  }
}

void Menu::Draw() {

  // Background
  {
    smk::View view;
    view.SetCenter(window().width() * 0.5f, window().height() * 0.5f);
    view.SetSize(window().width(), window().height());
    window().SetView(view);
    auto intro_screen = smk::Sprite(texture_intro_screen_);
    intro_screen.SetPosition(window().dimension() * 0.5f);
    intro_screen.SetCenter(texture_intro_screen_.width() * 0.5f,
                           texture_intro_screen_.height() * 0.5f);
    intro_screen.SetScale(
        std::max(float(window().width()) / texture_intro_screen_.width(),
                 float(window().height()) / texture_intro_screen_.height()));
    intro_screen.SetColor({0.2f, 0.2f, 0.2f, 1.f});
    window().Draw(intro_screen);
  }

  // forme du curseur
  auto curseur = smk::Shape::Circle(8);
  curseur.SetColor(glm::vec4(0.25, 0.12, 0.4, 1.0));

  // text/font
  smk::Text text;
  float text_scale = 0.8f;
  text.SetFont(font_arial);
  text.SetBlendMode(smk::BlendMode::Alpha);
  text.SetScale(text_scale);

  auto rectangle =
      RoundedRectangle(button_width_, button_height_, button_height_ * 0.2);

  smk::View view;
  view.SetCenter(0.f, total_scrolling_);
  view.SetSize(window().width(), window().height());
  window().SetView(view);

  // dessin des niveaux
  for (int i = 0; i < (int)entries.size(); i++) {

    // Rectangle background.
    rectangle.SetPosition(entry_data_[i].position);
    rectangle.SetColor({0.f, 0.f, 0.f, entry_data_[i].hover_alpha});
    window().Draw(rectangle);


    // Rectangle foreground.
    rectangle.SetPosition(entry_data_[i].position -
                          glm::vec2(0.f, entry_data_[i].hover_position));
    rectangle.SetColor({1.f, 1.f, 1.f, entry_data_[i].hover_alpha});
    window().Draw(rectangle);

    // Text.
    text.SetString(entries[i]);
    text.SetPosition(entry_data_[i].position -
                     text.ComputeDimensions() * 0.5f * text_scale -
                     glm::vec2(0.f, entry_data_[i].hover_position));
    text.SetColor(entry_data_[i].hover ? text_color_hover : text_color);
    window().Draw(text);
  }

  if (on_escape) {
    back_button_.Draw();
  }
}

void Menu::OnEnter() {
  scrolling_ = selected * item_space + window().height() * 0.5;
  is_using_keyboard_ = true;
}
