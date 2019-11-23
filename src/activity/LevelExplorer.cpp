#include "activity/LevelExplorer.hpp"

#include <fstream>
#include <smk/Color.hpp>
#include <smk/Screen.hpp>
#include <smk/Shape.hpp>
#include <smk/Text.hpp>
#include <sstream>
#include "resources.hpp"

namespace {
std::string packStatic = "";

}  // namespace

LevelExplorer::LevelExplorer(smk::Screen& screen) : Activity(screen) {}

void LevelExplorer::Draw() {
  smk::View view;
  view.SetCenter(screen().width() / 2, yview);
  view.SetSize(screen().width(), screen().height());
  screen().SetView(view);

  // forme du curseur
  auto curseur = smk::Shape::Circle(8);
  curseur.SetColor(glm::vec4(0.25, 0.12, 0.4, 1.0));

  // text/font
  smk::Text text;
  text.SetFont(font_arial);
  text.SetBlendMode(smk::BlendMode::Alpha);

  screen().PoolEvents();
  int previous_choice = choice;

  if (screen().input().IsKeyPressed(GLFW_KEY_UP))
    choice--;

  if (screen().input().IsKeyPressed(GLFW_KEY_DOWN))
    choice++;

  choice = std::max(0, std::min(std::min(save, int(entries.size()-1)), choice));
  if (choice != previous_choice)
    PlaySound(sb_menu_change);

  if (screen().input().IsKeyPressed(GLFW_KEY_ENTER)) {
    PlaySound(sb_menu_select);
    on_enter();
  }

  if (screen().input().IsKeyPressed(GLFW_KEY_ESCAPE)) {
    on_escape();
  }

  // mise a jour de la vue;
  float yview_target = choice * 40 + screen().height() * 0.5;
  yview += (yview_target - yview) * 0.05;

  // dessin de l'arrièreplan
  for (int y = yview / 32 - screen().height() / 32;
       y <= yview / 32 + screen().height() / 32; y++) {
    for (int x = 0; x <= screen().width() / 32; x++) {
      glass.SetPosition(x * 32, y * 32);
      screen().Draw(glass);
    }
  }

  // dessin des niveaux
  for (int i = 0; i < (int)entries.size(); i++) {
    text.SetString(entries[i]);

    // ombre
    text.SetPosition(30, 30 + 40 * i + 2);
    text.SetColor(glm::vec4(0.f, 0.f, 0.f, 0.1f));
    screen().Draw(text);

    // relief
    text.Move(-3, -3);
    if (i <= save)
      text.SetColor(glm::vec4(30, 25, 100, 255) / 255.f);
    else
      text.SetColor(glm::vec4(105, 50, 0, 255) / 255.f);
    screen().Draw(text);

    // text
    text.Move(0, -3);
    if (i <= save)
      text.SetColor(glm::vec4(60, 50, 200, 255) / 255.f);
    else
      text.SetColor(glm::vec4(210, 100, 0, 255) / 255.f);

    screen().Draw(text);
  }

  // dessin du curseur;
  curseur.SetPosition(12, 50 + 40 * choice);
  screen().Draw(curseur);
  screen().Display();
  screen().LimitFrameRate(60.0);
}

void LevelExplorer::OnEnter() {
  yview = choice * 40 + screen().height() * 0.5;
}
