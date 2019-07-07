#include "Level.hpp"
#include <smk/Color.hpp>
#include <smk/Screen.hpp>
#include <smk/Shape.hpp>
#include <smk/Text.hpp>
#include "resources.hpp"

using namespace std;

Level::Level() {
  title_ = "";
  author_ = "";
  width_ = 0;
  height_ = 0;
  starting_point_.x = 0;
  starting_point_.y = 0;
  pos.x = starting_point_.x * 32;
  pos.y = starting_point_.y * 32;
  view_x = pos.x + 16.0;
  view_y = pos.y + 16.0;
  view_speed = 1.0;
}

Level::Level(std::string filename) : Level() {
  std::ifstream flux(filename.c_str());
  if (!flux)
    return;
  std::string ligne;
  // INFORMATION CARTE
  std::getline(flux, title_);
  std::getline(flux, author_);
  std::getline(flux, ligne);
  width_ = std::stoi(ligne);
  std::getline(flux, ligne);
  height_ = std::stoi(ligne);
  // LECTURE CARTE
  cases_ = std::vector<int>(width_ * height_);
  for (int y = 0; y < height_; y++) {
    std::getline(flux, ligne);
    int x = 0;
    for (char c : ligne) {
      cases_[x + width_ * y] = c;

      if (c == 'j') {
        starting_point_.x = x;
        starting_point_.y = y;
        cases_[x + width_ * y] = '0';
      }
      ++x;
    }
    std::cerr << ligne << std::endl;
  }
  std::cerr << std::endl;
  pos.x = starting_point_.x * 32;
  pos.y = starting_point_.y * 32;
}

char Level::getCase(int x, int y) {
  if (x < 0 or x >= width_ or y < 0 or y >= height_)
    return ' ';
  return cases_[x + width_ * y];
}

void Level::setCase(int x, int y, int c) {
  cases_[x + width_ * y] = c;
}

void Level::getAutoTileInfo(int& gh, int& dh, int& db, int& gb, int x, int y) {
  // enregistrement des cases_ adjacentes
  int c = 0;
  int pow = 0b100000000;
  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
      if (getCase(x + dx, y + dy) == '1' or getCase(x + dx, y + dy) == ' ') {
        c += pow;
      }
      pow = pow >> 1;
    }
  }
  // clang-format off
  switch (c & 0b110100000) {
    case 0b000000000: gh = 1; break;
    case 0b100000000: gh = 1; break;
    case 0b010000000: gh = 7; break;
    case 0b000100000: gh = 5; break;
    case 0b010100000: gh = 12; break;
    case 0b110000000: gh = 7; break;
    case 0b100100000: gh = 5; break;
    case 0b110100000: gh = 13; break;
  }

  switch (c & 0b011001000) {
    case 0b000000000: dh = 2; break;
    case 0b010000000: dh = 6; break;
    case 0b001000000: dh = 2; break;
    case 0b000001000: dh = 5; break;
    case 0b011000000: dh = 6; break;
    case 0b001001000: dh = 5; break;
    case 0b010001000: dh = 11; break;
    case 0b011001000: dh = 13; break;
  }
  switch (c & 0b000100110) {
    case 0b000000000: gb = 3; break;
    case 0b000100000: gb = 8; break;
    case 0b000000100: gb = 3; break;
    case 0b000000010: gb = 7; break;
    case 0b000100100: gb = 8; break;
    case 0b000000110: gb = 7; break;
    case 0b000100010: gb = 10; break;
    case 0b000100110: gb = 13; break;
  }
  switch (c & 0b000001011) {
    case 0b000000000: db = 4; break;
    case 0b000001000: db = 8; break;
    case 0b000000010: db = 6; break;
    case 0b000000001: db = 4; break;
    case 0b000001010: db = 9; break;
    case 0b000000011: db = 6; break;
    case 0b000001001: db = 8; break;
    case 0b000001011: db = 13; break;
  }
}
// clang-format on

void Level::teleport(position& pos) {
  int xx = pos.x / 32;
  int yy = pos.y / 32;
  int i = 0;
  bool continuer = true;
  while (continuer) {
    if (cases_[i] == 't') {
      if (xx + width_ * yy != i) {
        continuer = false;
      } else
        i++;
    } else
      i++;
    if (i > height_ * width_) {
      continuer = false;
    }
  }
  pos.x = (i % width_) * 32;
  pos.y = (i / width_) * 32;
}

Level::Direction Level::Bounce(Direction direction, char bouncer) {
  boing.Play();
  switch (bouncer) {
    case 'o':  // coin gh
      if (direction == Direction::Up)
        return Direction::Right;
      else if (direction == Direction::Left)
        return Direction::Down;

    case 'p':  // coin dh
      if (direction == Direction::Up)
        return Direction::Left;
      else if (direction == Direction::Right)
        return Direction::Down;

    case 'm':  // coin bd
      if (direction == Direction::Down)
        return Direction::Left;
      else if (direction == Direction::Right)
        return Direction::Up;

    case 'l':  // coin gb
      if (direction == Direction::Down)
        return Direction::Right;
      else if (direction == Direction::Left)
        return Direction::Up;
  }
}

// clang-format off
bool Level::CanBounce(Direction dir, char bouncer) {
  switch (bouncer) {
    case 'o': return dir == Direction::Up || dir == Direction::Left;
    case 'p': return dir == Direction::Up || dir == Direction::Right;
    case 'm': return dir == Direction::Down || dir == Direction::Right;
    case 'l': return dir == Direction::Down || dir == Direction::Left;
  }
}
// clang-format on

void Level::Stop() {
  mouvement = false;
  if (ismoving)
    plop.Play();
}

bool Level::GetNewDirectionFromInput(smk::Screen& screen) {
  auto& input = screen.input();
  if (input.IsKeyHold(GLFW_KEY_UP))
    direction = Direction::Up;
  else if (input.IsKeyHold(GLFW_KEY_DOWN))
    direction = Direction::Down;
  else if (input.IsKeyHold(GLFW_KEY_LEFT))
    direction = Direction::Left;
  else if (input.IsKeyHold(GLFW_KEY_RIGHT))
    direction = Direction::Right;
  else
    return false;
  return true;
}

void Level::Step(smk::Screen& screen,
                 std::function<void()> on_win,
                 std::function<void()> on_lose) {
  if (mouvement_compteur > 0)
    return ContinueMovement();

  if (!mouvement) {
    ismoving = false;
    if (!GetNewDirectionFromInput(screen))
      return;
    mouvement = true;
  }

  NextStep(screen, on_win, on_lose);

  if (mouvement_compteur > 0)
    return ContinueMovement();
}

void Level::ContinueMovement() {
  const int step = 8;
  // clang-format off
  switch (direction) {
    case Direction::Up:    pos.y -= step; break;
    case Direction::Down:  pos.y += step; break;
    case Direction::Left:  pos.x -= step; break;
    case Direction::Right: pos.x += step; break;
  }
  // clang-format on
  mouvement_compteur -= step;
  ismoving = true;
}

void Level::NextStep(smk::Screen& screen,
                 std::function<void()> on_win,
                 std::function<void()> on_lose) {
  int X = pos.x / 32;
  int Y = pos.y / 32;
  switch (char CurrentCase = getCase(X,Y)) {
    case '0':  // vide
      break;
    case 's':  // sortie
      on_win();
      return;
    case ' ':  // hors jeu
      on_lose();
      return;
    case 'c':  // cle
      setCase(pos.x / 32, pos.y / 32, '0');
      nb_cle++;
      break;

    case 'o':  // coin gh
    case 'p':  // coin dh
    case 'm':  // coin bd
    case 'l':  // coin gb
      direction = Bounce(direction, CurrentCase);
      break;
    case 't':  // teleporteur
      teleport(pos);
      break;
  }

  char NextCase;
  // clang-format off
  switch (direction) {
    case Direction::Up:    NextCase = getCase(X,     Y - 1); break;
    case Direction::Down:  NextCase = getCase(X,     Y + 1); break;
    case Direction::Left:  NextCase = getCase(X - 1, Y    ); break;
    case Direction::Right: NextCase = getCase(X + 1, Y    ); break;
  }
  // clang-format on

  switch (NextCase) {
    case '0':  // vide
    case 's':  // sortie
    case 'c':  // cle
    case ' ':  // hors jeu
    case 't':  // teleporteur
      mouvement_compteur = 32;
      break;

    case 'd':
      if (nb_cle > 0) {
        ouverture_cle.Play();
        nb_cle--;
        switch (direction) {
          case Direction::Up:
            setCase(pos.x / 32, pos.y / 32 - 1, '0');
            break;
          case Direction::Down:
            setCase(pos.x / 32, pos.y / 32 + 1, '0');
            break;
          case Direction::Right:
            setCase(pos.x / 32 + 1, pos.y / 32, '0');
            break;
          case Direction::Left:
            setCase(pos.x / 32 - 1, pos.y / 32, '0');
            break;
        }
        break;
      }

    case '1':  // mur
    case 'i':  // invisible
      Stop();
      return;

    case 'o':  // coin gh
    case 'p':  // coin dh
    case 'm':  // coin bd
    case 'l':  // coin gb
      if (CanBounce(direction, NextCase)) {
        mouvement_compteur = 32;
      } else {
        Stop();
      }
      break;
  }
}

void Level::Draw(smk::Screen& screen) {
  screen.Clear(smk::Color::Black);

  float target_view_x = width_ * 32.f / 2.f;
  if (width_ * 32 > screen.width()) {
    target_view_x = pos.x + 16.0;
    target_view_x =
        std::min(width_ * 32.f - screen.width() / 2.f, target_view_x);
    target_view_x = std::max(screen.width() / 2.f, target_view_x);
  }

  float target_view_y = height_ * 32.f / 2.f;
  if (height_ * 32 > screen.height()) {
    target_view_y = pos.y + 16.0;
    target_view_y =
        std::min(height_ * 32.f - screen.height() / 2.f, target_view_y);
    target_view_y = std::max(screen.height() / 2.f, target_view_y);
  }

  view_x += (target_view_x - view_x) * view_speed;
  view_y += (target_view_y - view_y) * view_speed;
  view_speed += (0.05 - view_speed) * 0.01;

  smk::View view;
  view.SetSize(int(screen.width()),  //
               int(screen.height()));
  view.SetCenter(int(width_ * 32 * 0.5),  //
                 int(height_ * 32 * 0.5));
  view.SetCenter(view_x, view_y);
  screen.SetView(view);

  tempo++;

  for (int x = 0; x < width_; x++) {
    for (int y = 0; y < height_; y++) {
      char tile = getCase(x, y);
      // Rect<int> rectangle;
      switch (tile) {
        case '0':
          glass.SetPosition(x * 32, y * 32);
          screen.Draw(glass);
          break;
        case '1':
          int gh, dh, gb, db;
          getAutoTileInfo(gh, dh, db, gb, x, y);

          smk::Rectangle rectangle;
          rectangle.left = ((gh + 1) % 2) * 16;
          rectangle.top = int((gh - 1) / 2) * 16;
          rectangle.right = 16 + rectangle.left;
          rectangle.bottom = 16 + rectangle.top;
          block.SetTextureRectangle(rectangle);
          block.SetPosition(x * 32, y * 32);
          screen.Draw(block);

          rectangle.left = ((dh + 1) % 2) * 16;
          rectangle.top = int((dh - 1) / 2) * 16;
          rectangle.right = 16 + rectangle.left;
          rectangle.bottom = 16 + rectangle.top;
          block.SetTextureRectangle(rectangle);
          block.SetPosition(x * 32 + 16, y * 32);
          screen.Draw(block);

          rectangle.left = ((gb + 1) % 2) * 16;
          rectangle.top = int((gb - 1) / 2) * 16;
          rectangle.right = 16 + rectangle.left;
          rectangle.bottom = 16 + rectangle.top;
          block.SetTextureRectangle(rectangle);
          block.SetPosition(x * 32, y * 32 + 16);
          screen.Draw(block);

          rectangle.left = ((db + 1) % 2) * 16;
          rectangle.top = int((db - 1) / 2) * 16;
          rectangle.right = 16 + rectangle.left;
          rectangle.bottom = 16 + rectangle.top;
          block.SetTextureRectangle(rectangle);
          block.SetPosition(x * 32 + 16, y * 32 + 16);
          screen.Draw(block);

          break;
        case 'i': {
          smk::Rectangle rectangle;
          glass.SetPosition(x * 32, y * 32);
          screen.Draw(glass);
          block.SetPosition(x * 32, y * 32);
          rectangle.left = 0;
          rectangle.right = 32;
          rectangle.top = 0;
          rectangle.bottom = 32;
          block.SetTextureRectangle(rectangle);
          float dx = x * 32 - pos.x;
          float dy = y * 32 - pos.y;
          float distance = sqrt(dx * dx + dy * dy);
          block.SetColor(glm::vec4(
              1.0, 1.0, 1.0, 1.0 - std::min(1.0, (distance - 32) / 64.0)));
          screen.Draw(block);
          block.SetColor(smk::Color::White);
        } break;
        case 's':
          sortie.SetPosition(x * 32, y * 32);
          screen.Draw(sortie);
          break;
        case 'l':
          glass.SetPosition(x * 32, y * 32);
          screen.Draw(glass);
          angle1.SetPosition(x * 32, y * 32);
          screen.Draw(angle1);
          break;
        case 'o':
          glass.SetPosition(x * 32, y * 32);
          screen.Draw(glass);
          angle2.SetPosition(x * 32, y * 32);
          screen.Draw(angle2);
          break;
        case 'p':
          glass.SetPosition(x * 32, y * 32);
          screen.Draw(glass);
          angle3.SetPosition(x * 32, y * 32);
          screen.Draw(angle3);
          break;
        case 'm':
          glass.SetPosition(x * 32, y * 32);
          screen.Draw(glass);
          angle4.SetPosition(x * 32, y * 32);
          screen.Draw(angle4);
          break;
        case 'c':
          glass.SetPosition(x * 32, y * 32);
          screen.Draw(glass);
          cle.SetPosition(x * 32, y * 32);
          screen.Draw(cle);
          break;
        case 'd':
          serrure.SetPosition(x * 32, y * 32);
          screen.Draw(serrure);
          break;

        case 't':
          glass.SetPosition(x * 32, y * 32);
          screen.Draw(glass);
          vortex.SetPosition(x * 32 + 16, y * 32 + 16);
          vortex.SetRotation(-tempo * 23);
          screen.Draw(vortex);
          break;
      }
    }
  }

  // affichage joueur;
  joueur.SetPosition(pos.x, pos.y);
  screen.Draw(joueur);
}
