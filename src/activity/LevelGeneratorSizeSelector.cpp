#include "activity/LevelGeneratorSizeSelector.hpp"
#include "resources.hpp"
#include "smk/Color.hpp"
#include "smk/Shape.hpp"
#include "smk/Text.hpp"

LevelGeneratorSizeSelector::LevelGeneratorSizeSelector(smk::Window& window):
  Activity(window) {}

void LevelGeneratorSizeSelector::Draw() {
  auto& input = window().input();
  int previous_width = width;
  int previous_height = height;

  if (input.IsKeyPressed(GLFW_KEY_LEFT)) --width;
  if (input.IsKeyPressed(GLFW_KEY_RIGHT)) ++width;
  if (input.IsKeyPressed(GLFW_KEY_UP)) --height;
  if (input.IsKeyPressed(GLFW_KEY_DOWN)) ++height;

  if (previous_width != width || previous_height != height) 
    PlaySound(sb_menu_change);

  width = std::max(4, width);
  height = std::max(4, height);
  if (input.IsKeyPressed(GLFW_KEY_ENTER) ||
      input.IsKeyPressed(GLFW_KEY_SPACE)) {
    PlaySound(sb_menu_select);
    on_enter();
    return;
  }
  if (input.IsKeyPressed(GLFW_KEY_ESCAPE)) {
    PlaySound(sb_menu_select);
    on_escape();
    return;
  }

  UpdateView();

  // Background
  auto view = window().GetView();
  float x0 = int(view.Left()) / 32 * 32 - 32;
  float x1 = int(view.Right()) / 32 * 32 + 32;
  float y0 = int(view.Top()) / 32 * 32 - 32;
  float y1 = int(view.Bottom()) / 32 * 32 + 32;

  block = smk::Sprite(blockimg, {0, 0, 32, 32});
  for (float x = x0; x <= x1; x += 32) {
    for (float y = y0; y <= y1; y += 32) {
      block.SetPosition(x, y);
      window().Draw(block);
    }
  }

  // Level size.
  auto inside = glass;
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      inside.SetPosition(x * 32, y * 32);
      window().Draw(inside);
    }
  }

  smk::Text text;
  text.SetFont(font_arial);
  text.SetString(std::to_string(width) + " × " + std::to_string(height));
  auto dimension = text.ComputeDimensions();
  float scale = (view.Right() - view.Left()) / window().width();
  text.SetPosition(width * 16.f - dimension.x * 0.5 * scale,
                   height * 16.f - dimension.y * 0.5 * scale);
  text.SetColor(smk::Color::Black);
  text.SetScale(scale, scale);
  window().Draw(text);

  {
    smk::View view;
    view.SetSize(window().width(), window().height());
    view.SetCenter(window().width() * 0.5, window().height() * 0.5);
    window().SetView(view);
  }


  smk::Text level_size_text;
  level_size_text.SetFont(font_arial);
  level_size_text.SetPosition(10, 10);
  level_size_text.SetScale(1.0, 1.0);
  level_size_text.SetString("Level size");
  level_size_text.SetColor(smk::Color::Black);
  auto dimension2 = level_size_text.ComputeDimensions();

  auto rectangle = smk::Shape::Square();
  rectangle.SetScale(window().width(), dimension2.y + 20);
  rectangle.SetColor({1.0, 1.0, 1.0, 0.5});

  float space_between_key = 32.f;
  sprite_key_left.SetPosition(dimension2.x + 20, 10 + dimension2.y - 32);
  sprite_key_right.SetPosition(dimension2.x + 20 + 1 * space_between_key,
                               10 + dimension2.y - 32);
  sprite_key_up.SetPosition(dimension2.x + 20 + 3 * space_between_key, 10 + dimension2.y - 32);
  sprite_key_down.SetPosition(dimension2.x + 20 + 4 * space_between_key,
                              10 + dimension2.y - 32);
  sprite_key_enter.SetPosition(dimension2.x + 20 + 6 * space_between_key,
                               10 + dimension2.y - 32);

  for (auto* it : {
           &sprite_key_left,
           &sprite_key_right,
           &sprite_key_up,
           &sprite_key_down,
           &sprite_key_enter,
       }) {
    it->SetColor(smk::Color::White);
  }

  window().Draw(rectangle);
  window().Draw(level_size_text);
  window().Draw(sprite_key_left);
  window().Draw(sprite_key_right);
  window().Draw(sprite_key_up);
  window().Draw(sprite_key_down);
  window().Draw(sprite_key_enter);

  window().Display();
}

void LevelGeneratorSizeSelector::UpdateView() {
  float target_view_x = width * 0.5 * 32.f;
  float target_view_y = height * 0.5f * 32.f;
  float target_zoom =
      std::max(1.f, std::max(width * 40.f / window().width(),
                             height * 40.f / window().height()));

  const float view_speed = 0.05;
  view_x_ += (target_view_x - view_x_) * view_speed;
  view_y_ += (target_view_y - view_y_) * view_speed;
  view_zoom_ += (target_zoom - view_zoom_) * view_speed;

  smk::View view;
  view.SetSize(int(window().width()) * view_zoom_,
               int(window().height()) * view_zoom_);
  view.SetCenter(view_x_, view_y_);
  window().SetView(view);
}
