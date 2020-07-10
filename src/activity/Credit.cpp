#include "activity/Credit.hpp"
#include <smk/Color.hpp>
#include <smk/Shape.hpp>
#include <smk/Input.hpp>
#include "resources.hpp"
#include "License.hpp"

Credit::Credit(smk::Window& window) : Activity(window), back_button_(window) {
  back_button_.on_quit = [&] { on_quit(); };
}

void Credit::OnEnter() {
  view_ = glm::vec2(0.f, 0.f);
  view_speed_ = glm::vec2(0.f, 0.f);
  last_cursor_position = window().input().cursor();
}

Credit::~Credit() {}

void Credit::Draw() {
  auto& input = window().input();

  if (input.IsKeyPressed(GLFW_KEY_ENTER) ||
      input.IsKeyPressed(GLFW_KEY_ESCAPE) ||
      input.IsKeyPressed(GLFW_KEY_SPACE)) {
    on_quit();
    return;
  }

  if (input.IsKeyHold(GLFW_KEY_LEFT)) view_.x -= 10;
  if (input.IsKeyHold(GLFW_KEY_RIGHT)) view_.x += 10;
  if (input.IsKeyHold(GLFW_KEY_UP)) view_.y -= 10;
  if (input.IsKeyHold(GLFW_KEY_DOWN)) view_.y += 10;

  if (input.IsCursorPressed()) {
    last_cursor_position = input.cursor();
  } else if (input.IsCursorHold() || input.IsCursorReleased()) {
    auto diff = last_cursor_position - input.cursor();
    view_ += diff;
    view_speed_ += (diff - view_speed_) * 0.3f;
    last_cursor_position = input.cursor();
  } else {
    view_ += view_speed_;
    view_speed_ *= 0.9f;
  }

  window().Clear({0.3, 0.3, 0.2, 1.0});
  smk::View view;
  view.SetCenter(window().width() * 0.5 + view_.x, window().height() * 0.5 + view_.y);
  view.SetSize(window().width(), window().height());
  window().SetView(view);

  auto draw = [&](smk::Transformable& text, glm::vec4 color, int style = 0) {
    text.SetColor(smk::Color::Black);
    window().Draw(text);

    text.Move(0, -3);
    if (style == 1)
      text.SetColor(smk::Color::White);
    if (style != 2)
      window().Draw(text);

    text.Move(0, +1.5);
    text.SetColor(color);
    window().Draw(text);
  };

  int y = 0;
  int x = 32;
  for (auto& author : authors) {
    // Name
    {
      auto t = smk::Text();
      t.SetString(author.name);
      t.SetFont(font_arial);
      t.SetPosition(10, y);
      draw(t, {0.5, 0.5, 0.5, 1.0}, 1);
      y += t.ComputeDimensions().y + 10;
    }
    for (auto& resource : author.resources) {
      auto circle = smk::Shape::Circle(5);
      circle.SetPosition(35 + x, y + 12);
      draw(circle, {0.8,0.8,0.8,1.0}, 1);

      int licence_x;
      int licence_y;
      // Description
      {
        auto t = smk::Text();
        t.SetString(resource.description);
        t.SetFont(font_arial_20);
        t.SetPosition(50 + x, y);
        draw(t, {0.8, 0.8, 0.8, 1.0});
        licence_x = 50 + x + t.ComputeDimensions().x + 20;
        licence_y = y;
        y += t.ComputeDimensions().y + 5;
      }
      // License
      {
        auto t = smk::Text();
        t.SetString(resource.license);
        t.SetFont(font_arial_20);
        t.SetPosition(licence_x, licence_y);
        t.Move(10.f, 0.f);
        draw(t, {0.9, 0.6, 0.6, 1.0});
      }
      // Link
      {
        auto t = smk::Text();
        t.SetString(resource.link);
        t.SetFont(font_arial_20);
        t.SetPosition(50 + x, y);
        draw(t, {0.5f, 0.5f, 1.f, 1.0}, 2);
        y += t.ComputeDimensions().y + 10;
      }
    }
    y += 40;
  }
  back_button_.Step();
  back_button_.Draw();
}
