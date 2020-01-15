#include "activity/Menu.hpp"
#include <smk/Color.hpp>
#include "resources.hpp"
#include <smk/Shape.hpp>

Menu::Menu(smk::Window& window) : Activity(window) {}
Menu::~Menu() {}

void Menu::Draw() {

  // Input.
  window().PoolEvents();
  auto& input = window().input();

  smk::View view;
  view.SetCenter(320, 240);
  view.SetSize(window().width(), window().height());
  window().SetView(view);

  window().Clear(smk::Color::Black);

  // Draw.
  block = smk::Sprite(blockimg, {0, 0, 32, 32});
  for (int x = 0; x <= 640; x += 32) {
    for (int y = 0; y <= 480; y += 32) {
      int dx = x - 320;
      int dy = y - 240;
      int r = 200;
      if (dx * dx + dy * dy < r * r) {
        glass.SetPosition(x, y);
        window().Draw(glass);
      } else {
        block.SetPosition(x, y);
        window().Draw(block);
      }
    }
  }

  float dy = 50.f- entries.size() * 20.f;

  auto cursor = input.cursor();
  bool selected_by_cursor = false;
  int selected_previous = selected;

  // Currect the view:
  cursor -= glm::vec2(window().width(), window().height()) * 0.5f;
  cursor += glm::vec2(640, 480) * 0.5f;

  for (int i = 0; i < (int)entries.size(); ++i) {
    auto& entry = entries[i];
    smk::Text text;
    text.SetFont(font_arial);
    text.SetString(entry);

    auto text_dimension = text.ComputeDimensions();
    float text_width = text_dimension.x;
    float center = 320;

    auto text_position = glm::vec2(center - text_width / 2, 220 + i * 40 + dy);
    if (cursor.x > text_position.x &&
        cursor.x < text_position.x + text_dimension.x &&
        cursor.y > text_position.y &&
        cursor.y < text_position.y + text_dimension.y) {
      selected = i;
      selected_by_cursor = input.IsCursorReleased();
    }

    // Shadow
    text.SetPosition(text_position);
    text.SetColor(glm::vec4(0.f, 0.f, 0.f, 0.1f));
    window().Draw(text);

    // relief
    text.Move(-3, -3);
    text.SetColor(glm::vec4(30, 25, 100, 255) / 255.f);
    window().Draw(text);

    // text
    text.Move(0, -3);
    text.SetColor(glm::vec4(60, 50, 200, 255) / 255.f);
    window().Draw(text);



    if (i != selected)
      continue;
    // circle
    auto circle = smk::Shape::Circle(5);
    circle.SetColor(glm::vec4(100, 100, 100, 255) / 255.f);

    circle.SetPosition(center + (10 * sin(time * 0.6) + text_width * 0.5 + 20),
                       240 + selected * 40 + dy);
    window().Draw(circle);

    circle.SetPosition(center - (10 * sin(time * 0.6) + text_width * 0.5 + 20),
                       240 + selected * 40 + dy);
    window().Draw(circle);
  }

  if (input.IsKeyPressed(GLFW_KEY_ENTER) || selected_by_cursor) {
    PlaySound(sb_menu_select);
    on_enter();
    return;
  }

  if (input.IsKeyPressed(GLFW_KEY_ESCAPE)) {
    on_escape();
    return;
  }

  // Update selected
  {

    if (input.IsKeyPressed(GLFW_KEY_UP))
      selected--;

    if (input.IsKeyPressed(GLFW_KEY_DOWN))
      selected++;

    selected = std::max(0, std::min((int)entries.size() - 1, selected));
    if (selected != selected_previous) {
      PlaySound(sb_menu_change);
      on_change();
    }
  }


  window().Display();
  window().LimitFrameRate(60.f);
  time++;
}
