#include "activity/Menu.hpp"
#include <smk/Color.hpp>
#include "resources.hpp"
#include <smk/Shape.hpp>

Menu::Menu(smk::Screen& screen) : Activity(screen) {}
Menu::~Menu() {}

void Menu::Draw() {

  // Input.
  screen().PoolEvents();
  auto& input = screen().input();

  if (input.IsKeyPressed(GLFW_KEY_ENTER)) {
    on_enter();
    return;
  }

  if (input.IsKeyPressed(GLFW_KEY_ESCAPE)) {
    on_escape();
    return;
  }

  // Update selected
  {
    int selected_previous = selected;

    if (input.IsKeyPressed(GLFW_KEY_UP))
      selected--;

    if (input.IsKeyPressed(GLFW_KEY_DOWN))
      selected++;

    selected = std::max(0, std::min((int)entries.size() - 1, selected));
    if (selected != selected_previous)
      on_change();
  }

  smk::View view;
  view.SetCenter(320, 240);
  view.SetSize(screen().width(), screen().height());
  screen().SetView(view);

  screen().Clear(smk::Color::Black);

  // Draw.
  block.SetTextureRectangle(smk::Rectangle{0, 0, 32, 32});
  for (int x = 0; x <= 640; x += 32) {
    for (int y = 0; y <= 480; y += 32) {
      int dx = x - 320;
      int dy = y - 240;
      int r = 200;
      if (dx * dx + dy * dy < r * r) {
        glass.SetPosition(x, y);
        screen().Draw(glass);
      } else {
        block.SetPosition(x, y);
        screen().Draw(block);
      }
    }
  }

  for (int i = 0; i < entries.size(); ++i) {
    auto& entry = entries[i];
    smk::Text text;
    text.SetFont(font_arial);
    text.SetString(entry);

    float text_width = text.ComputeDimensions().x;
    float center = 320;

    // Shadow
    text.SetPosition(center - text_width / 2, 220 + i * 40);
    text.SetColor(glm::vec4(0.f, 0.f, 0.f, 0.1f));
    screen().Draw(text);

    // relief
    text.Move(-3, -3);
    text.SetColor(glm::vec4(30, 25, 100, 255) / 255.f);
    screen().Draw(text);

    // text
    text.Move(0, -3);
    text.SetColor(glm::vec4(60, 50, 200, 255) / 255.f);
    screen().Draw(text);

    if (i != selected)
      continue;
    // circle
    auto circle = smk::Shape::Circle(5);
    circle.SetColor(glm::vec4(100, 100, 100, 255) / 255.f);

    circle.SetPosition(center + (10 * sin(time * 0.6) + text_width * 0.5 + 20),
                       240 + selected * 40);
    screen().Draw(circle);

    circle.SetPosition(center - (10 * sin(time * 0.6) + text_width * 0.5 + 20),
                       240 + selected * 40);
    screen().Draw(circle);
  }

  screen().Display();
  screen().LimitFrameRate(60.f);
  time++;
}
