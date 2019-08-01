#include "activity/IntroScreen.hpp"
#include <smk/Color.hpp>
#include "resources.hpp"
#include <smk/Shape.hpp>

namespace {
float width = 128;
float height = 128;
}  // namespace

IntroScreen::IntroScreen(smk::Screen& screen) : Activity(screen) {}
void IntroScreen::OnEnter() {
  // clang-format off
  zoom_ = 
    std::min(
      screen().width() / width,
      screen().height() / height
    ) * 10.0;
  // clang-format on
}

IntroScreen::~IntroScreen() {}

void IntroScreen::Draw() {

  // Input.
  screen().PoolEvents();
  auto& input = screen().input();

  if (input.IsKeyPressed(GLFW_KEY_ENTER)) {
    on_enter();
    return;
  }

  smk::View view;
  view.SetCenter(width * 0.5, height * 0.5);
  // clang-format off
  float zoom_target = 
    std::min(
      screen().width() / width,
      screen().height() / height
    );
  // clang-format on
  zoom_ *= std::pow(zoom_target / zoom_, 0.02);
  std::cerr << "zoom_ = " << zoom_ << std::endl;
  view.SetSize(screen().width() / zoom_, screen().height() / zoom_);
  screen().SetView(view);

  screen().Clear(smk::Color::Black);

  smk::Sprite intro_screen;
  intro_screen.SetTexture(texture_intro_screen_);
  intro_screen.SetPosition(0,0);
  screen().Draw(intro_screen);

  view.SetCenter(0, 0);
  view.SetSize(screen().width(), screen().height());
  screen().SetView(view);

  float ratio = std::log(zoom_ / zoom_target);
  float alpha = std::max(0.0, 1.0 - ratio * ratio * 30);
  float alpha2 = std::max(0.0, 1.0 - ratio * ratio * 90);
  glm::vec4 color = {1.f, 1.f, 1.f, alpha};

  sprite_key_up.SetColor(color);
  sprite_key_down.SetColor(color);
  sprite_key_left.SetColor(color);
  sprite_key_right.SetColor(color);
  sprite_key_enter.SetColor(color);
  sprite_key_escape.SetColor(color);

  float dx = 32;
  float dy = 32;
  float ox = (4 - 5) * dx;
  float oy = (1 - 2.5 + 2) * dy + 100 * (1.0 - alpha);

  auto rectangle = smk::Shape::Square();
  int i = 10; {
    float z = 1.0;
    rectangle.SetColor({1.f, 1.f, 1.f, 0.2 * alpha});
    rectangle.SetScale(dx * 11 * z, dy * 5 * z);

    float dzx = dx * 11 * (1 - z) * 0.5;
    float dzy = dy * 5 * (1 - z) * 0.5;

    rectangle.SetPosition(ox - 4 * dx + dzx, oy - 1 * dx + dzy);
    screen().Draw(rectangle);

    rectangle.SetPosition(ox - 4 * dx + dzx, oy - 7 * dx + dzy);
    screen().Draw(rectangle);
  }

  sprite_key_up.SetPosition(ox + 1 * dx, oy + 0 * dy);
  sprite_key_down.SetPosition(ox + 1 * dx, oy + 2 * dy);
  sprite_key_left.SetPosition(ox + 0 * dx, oy + 1 * dy);
  sprite_key_right.SetPosition(ox + 2 * dx, oy + 1 * dy);
  sprite_key_enter.SetPosition(ox + 4 * dx, oy + 0 * dy);
  sprite_key_escape.SetPosition(ox - 3 * dx, oy + 0 * dy);

  {
    auto text = smk::Text();
    text.SetFont(font_arial);
    text.SetString("IceMaze");
    text.SetColor({0.f,0.f,0.f,alpha});
    auto dimension = text.ComputeDimensions();
    text.SetPosition(ox + 1 * dx, oy - 4.5 * dy);
    text.Move(-dimension * 0.5f);
    screen().Draw(text);
  }

  screen().Draw(sprite_key_up);
  screen().Draw(sprite_key_down);
  screen().Draw(sprite_key_left);
  screen().Draw(sprite_key_right);
  screen().Draw(sprite_key_enter);
  screen().Draw(sprite_key_escape);

  {
    auto text = smk::Text();
    text.SetFont(font_arial);
    text.SetString("Press enter to continue...");
    text.SetColor({0.f, 0.f, 0.f, alpha2 * (0.5 + 0.5 * sin(time * 0.1))});
    auto dimension = text.ComputeDimensions();
    float x = screen().width() * 0.5 - 10;
    float y = screen().height() * 0.5 - 10;
    x = std::min(x, 0.f+ 64 * zoom_ - 10);
    y = std::min(y, 0.f + 64 * zoom_ - 10);
    text.SetPosition(x,y);
    text.Move(-dimension);
    text.SetBlendMode(smk::BlendMode::Invert);
    for(int i = 0; i <8; ++i) {
      auto text2 = text;
      text2.Move(sin(i * 6.28 / 8), cos(i * 6.28 / 8));
      text.SetColor({0.f, 0.f, 0.f, alpha2 * (0.5 + 0.5 * sin(time * 0.1))});
      screen().Draw(text2);
    }
    text.SetColor({1.f, 1.f, 1.f, alpha2 * (0.5 + 0.5 * sin(time * 0.1))});
    screen().Draw(text);
  }

  // Dark background.
  {
    auto rectangle = smk::Shape::Square();
    rectangle.SetPosition(-screen().width() * 0.5f, -screen().height() * 0.5f);
    rectangle.SetScale(screen().width(), screen().height());
    rectangle.SetColor({0.f, 0.f, 0.f, std::exp(-time*0.02)});
    screen().Draw(rectangle);
  }

  screen().Display();
  screen().LimitFrameRate(60.f);
  time++;
}
