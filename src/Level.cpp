#include "Level.hpp"
#include <queue>
#include <random>
#include <set>
#include <smk/Color.hpp>
#include <smk/RenderTarget.hpp>
#include <smk/Shape.hpp>
#include <smk/Text.hpp>
#include "resources.hpp"
#include "util/auto_reset.hpp"

void NOTREACHED() {}

using namespace std;

Level::Level() {
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
  current_position = starting_point_;
  next_position = current_position;
}

char Level::getCase(Position pos) {
  if (pos.x < 0 || pos.x >= width_ || pos.y < 0 || pos.y >= height_)
    return ' ';
  return cases_[pos.x + width_ * pos.y];
}

void Level::setCase(Position pos, char c) {
  cases_[pos.x + width_ * pos.y] = c;
}

void Level::getAutoTileInfo(int& gh, int& dh, int& db, int& gb, int x, int y) {
  // enregistrement des cases_ adjacentes
  int c = 0;
  int pow = 0b100000000;
  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
      if (getCase({x + dx, y + dy}) == '1' ||
          getCase({x + dx, y + dy}) == ' ') {
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

void Level::teleport() {
  size_t j = current_position.x + width_ * current_position.y;
  for (size_t i = 0; i < cases_.size(); ++i) {
    if (cases_[i] != 't')
      continue;
    if (i == j)
      continue;

    current_position.x = i % width_;
    current_position.y = i / width_;
    next_position.x = i % width_;
    next_position.y = i / width_;
  }
}

Direction Level::Bounce(Direction direction_, char bouncer) {
  PlaySoundInternal(boingsb);
  switch (bouncer) {
    case 'o':  // coin gh
      if (direction_ == Direction::Up)
        return Direction::Right;
      else if (direction_ == Direction::Left)
        return Direction::Down;
      break;
    case 'p':  // coin dh
      if (direction_ == Direction::Up)
        return Direction::Left;
      else if (direction_ == Direction::Right)
        return Direction::Down;
      break;
    case 'm':  // coin bd
      if (direction_ == Direction::Down)
        return Direction::Left;
      else if (direction_ == Direction::Right)
        return Direction::Up;
      break;
    case 'l':  // coin gb
      if (direction_ == Direction::Down)
        return Direction::Right;
      else if (direction_ == Direction::Left)
        return Direction::Up;
      break;
  }
  return direction_;
}

// clang-format off
bool Level::CanBounce(Direction dir, char bouncer) {
  switch (bouncer) {
    case 'o': return dir == Direction::Up || dir == Direction::Left;
    case 'p': return dir == Direction::Up || dir == Direction::Right;
    case 'm': return dir == Direction::Down || dir == Direction::Right;
    case 'l': return dir == Direction::Down || dir == Direction::Left;
  }
  NOTREACHED();
  return false;
}
// clang-format on

void Level::Stop() {
  next_position = current_position;
  direction_ = Direction::None;
  if (is_moving_)
    PlaySoundInternal(plopsb);
}

void Level::Step(Direction input,
                 std::function<void()> on_win,
                 std::function<void()> on_lose) {
  if (anim > 0)
    return AnimationStep();

  if (direction_ == Direction::None) {
    is_moving_ = false;
    direction_ = input;
  }

  current_position = next_position;
  NextStep(on_win, on_lose);

  if (anim > 0)
    return AnimationStep();
}

void Level::AnimationStep() {
  const int step = 8;
  anim -= step;
  is_moving_ = true;
  pos.x = anim * current_position.x + (32 - anim) * next_position.x;
  pos.y = anim * current_position.y + (32 - anim) * next_position.y;
}

// clang-format off
Position NextPosition(Position current, Direction direction_) {
  switch (direction_) {
    case Direction::Up:    current.y -= 1; break;
    case Direction::Down:  current.y += 1; break;
    case Direction::Left:  current.x -= 1; break;
    case Direction::Right: current.x += 1; break;
    case Direction::None: NOTREACHED(); break;
  }
  return current;
}
// clang-format on

void Level::NextStep(std::function<void()> on_win,
                     std::function<void()> on_lose) {
  switch (char CurrentCase = getCase(current_position)) {
    case '0':  // vide
      break;
    case 's':  // sortie
      PlaySoundInternal(sb_success);
      on_win();
      return;
    case ' ':  // hors jeu
      PlaySoundInternal(sb_lose);
      on_lose();
      return;
    case 'c':  // cle
      setCase(current_position, '0');
      PlaySoundInternal(sb_get_key);
      nb_cle++;
      break;

    case 'o':  // coin gh
    case 'p':  // coin dh
    case 'm':  // coin bd
    case 'l':  // coin gb
      if (CanBounce(direction_, CurrentCase)) {
        direction_ = Bounce(direction_, CurrentCase);
      } else {
        Stop();
        return;
      }
      break;
    case 't':  // teleporteur
      teleport();
      break;
  }

  if (direction_ == Direction::None)
    return;

  next_position = NextPosition(current_position, direction_);
  char NextCase = getCase(next_position);

  switch (NextCase) {
    case '0':  // vide
    case 's':  // sortie
    case 'c':  // cle
    case ' ':  // hors jeu
    case 't':  // teleporteur
      anim = 32;
      break;

    case 'd':
      if (nb_cle > 0) {
        nb_cle--;
        PlaySoundInternal(ouverture_clesb);
        setCase(next_position, '0');
        anim = 32;
        break;
      }
      [[fallthrough]];
    case '1':  // mur
    case 'i':  // invisible
      Stop();
      return;

    case 'o':  // coin gh
    case 'p':  // coin dh
    case 'm':  // coin bd
    case 'l':  // coin gb
      if (CanBounce(direction_, NextCase))
        anim = 32;
      else
        Stop();
      break;
  }
}

void Level::UpdateView(smk::RenderTarget& surface) {
  float target_view_x = width_ * 32.f / 2.f;
  if (width_ * 32 > surface.width()) {
    target_view_x = pos.x + 16.0;
    target_view_x =
        std::min(width_ * 32.f - surface.width() / 2.f, target_view_x);
    target_view_x = std::max(surface.width() / 2.f, target_view_x);
  }

  float target_view_y = height_ * 32.f / 2.f;
  if (height_ * 32 > surface.height()) {
    target_view_y = pos.y + 16.0;
    target_view_y =
        std::min(height_ * 32.f - surface.height() / 2.f, target_view_y);
    target_view_y = std::max(surface.height() / 2.f, target_view_y);
  }

  view_x += (target_view_x - view_x) * view_speed;
  view_y += (target_view_y - view_y) * view_speed;
  view_speed += (0.05 - view_speed) * 0.01;

  smk::View view;
  view.SetSize(int(surface.width()),  //
               int(surface.height()));
  view.SetCenter(int(width_ * 32 * 0.5),  //
                 int(height_ * 32 * 0.5));
  view.SetCenter(view_x, view_y);
  surface.SetView(view);
}

void Level::Draw(smk::RenderTarget& surface) {
  surface.Clear(smk::Color::Black);
  UpdateView(surface);

  tempo++;

  for (int x = 0; x < width_; x++) {
    for (int y = 0; y < height_; y++) {
      char tile = getCase({x, y});
      // Rect<int> rectangle;
      switch (tile) {
        case '0':
          glass.SetPosition(x * 32, y * 32);
          surface.Draw(glass);
          break;
        case '1':
          int gh, dh, gb, db;
          getAutoTileInfo(gh, dh, db, gb, x, y);

          smk::Rectangle rectangle;
          rectangle.left = ((gh + 1) % 2) * 16;
          rectangle.top = int((gh - 1) / 2) * 16;
          rectangle.right = 16 + rectangle.left;
          rectangle.bottom = 16 + rectangle.top;
          block = smk::Sprite(blockimg, rectangle);
          block.SetPosition(x * 32, y * 32);
          surface.Draw(block);

          rectangle.left = ((dh + 1) % 2) * 16;
          rectangle.top = int((dh - 1) / 2) * 16;
          rectangle.right = 16 + rectangle.left;
          rectangle.bottom = 16 + rectangle.top;
          block = smk::Sprite(blockimg, rectangle);
          block.SetPosition(x * 32 + 16, y * 32);
          surface.Draw(block);

          rectangle.left = ((gb + 1) % 2) * 16;
          rectangle.top = int((gb - 1) / 2) * 16;
          rectangle.right = 16 + rectangle.left;
          rectangle.bottom = 16 + rectangle.top;
          block = smk::Sprite(blockimg, rectangle);
          block.SetPosition(x * 32, y * 32 + 16);
          surface.Draw(block);

          rectangle.left = ((db + 1) % 2) * 16;
          rectangle.top = int((db - 1) / 2) * 16;
          rectangle.right = 16 + rectangle.left;
          rectangle.bottom = 16 + rectangle.top;
          block = smk::Sprite(blockimg, rectangle);
          block.SetPosition(x * 32 + 16, y * 32 + 16);
          surface.Draw(block);

          break;
        case 'i': {
          smk::Rectangle rectangle;
          glass.SetPosition(x * 32, y * 32);
          surface.Draw(glass);
          rectangle.left = 0;
          rectangle.right = 32;
          rectangle.top = 0;
          rectangle.bottom = 32;
          block = smk::Sprite(blockimg, rectangle);
          block.SetPosition(x * 32, y * 32);
          float dx = x * 32 - pos.x;
          float dy = y * 32 - pos.y;
          float distance = sqrt(dx * dx + dy * dy);
          block.SetColor(glm::vec4(
              1.0, 1.0, 1.0, 1.0 - std::min(1.0, (distance - 32) / 64.0)));
          surface.Draw(block);
          block.SetColor(smk::Color::White);
        } break;
        case 's':
          sortie.SetPosition(x * 32, y * 32);
          surface.Draw(sortie);
          break;
        case 'l':
          glass.SetPosition(x * 32, y * 32);
          surface.Draw(glass);
          angle1.SetPosition(x * 32, y * 32);
          surface.Draw(angle1);
          break;
        case 'o':
          glass.SetPosition(x * 32, y * 32);
          surface.Draw(glass);
          angle2.SetPosition(x * 32, y * 32);
          surface.Draw(angle2);
          break;
        case 'p':
          glass.SetPosition(x * 32, y * 32);
          surface.Draw(glass);
          angle3.SetPosition(x * 32, y * 32);
          surface.Draw(angle3);
          break;
        case 'm':
          glass.SetPosition(x * 32, y * 32);
          surface.Draw(glass);
          angle4.SetPosition(x * 32, y * 32);
          surface.Draw(angle4);
          break;
        case 'c':
          glass.SetPosition(x * 32, y * 32);
          surface.Draw(glass);
          cle.SetPosition(x * 32, y * 32);
          surface.Draw(cle);
          break;
        case 'd':
          serrure.SetPosition(x * 32, y * 32);
          surface.Draw(serrure);
          break;

        case 't':
          glass.SetPosition(x * 32, y * 32);
          surface.Draw(glass);
          vortex.SetPosition(x * 32 + 16, y * 32 + 16);
          vortex.SetRotation(-tempo * 23);
          surface.Draw(vortex);
          break;
        case 'j':
          joueur.SetPosition(x * 32, y*32);
          surface.Draw(joueur);
          break;
      }
    }
  }

  // affichage joueur;
  joueur.SetPosition(pos.x, pos.y);
  surface.Draw(joueur);
}

int Level::Evaluate() {
  AutoReset<bool>(in_evaluate_, true);

  bool initial_position_found = false;
  for (int i = 0; i < (int)cases_.size(); ++i) {
    if (cases_[i] == 'j') {
      if (initial_position_found)
        return -1;
      initial_position_found = true;
      starting_point_ = {i % width_, i / width_};
      cases_[i] = '0';
    }
  }
  if (!initial_position_found)
    return -1;

  in_evaluate_ = true;

  struct State {
    Position position;
    int iteration;
    bool operator<(const State state) const {
      return position < state.position;
    }
  };

  State initial_state = {starting_point_, 0};

  std::set<State> handled;
  std::queue<State> to_be_handled;
  handled.insert(initial_state);
  to_be_handled.push(initial_state);

  bool win = false;
  bool lose = false;
  std::function<void()> on_win = [&win]() { win = true; };
  std::function<void()> on_lose = [&lose]() { lose = true; };

  int number_of_walls = 0;
  for(auto& it : cases_)
    number_of_walls += (it == '1');

  int max_width = 0;
  while(!to_be_handled.empty()) {
    max_width = std::max(max_width, int(to_be_handled.size()));
    State state = to_be_handled.front();
    to_be_handled.pop();

    for (Direction dir : {
             Direction::Up,
             Direction::Down,
             Direction::Left,
             Direction::Right,
         }) {
      next_position = state.position;
      direction_ = dir;
      win = false;
      lose = false;
      int travel = 0;
      const int max_travel = (width_ * height_);
      while (direction_ != Direction::None && !lose && !win && travel < max_travel) {
        current_position = next_position;
        NextStep(on_win, on_lose);
        travel++;
      }
      if (lose || travel >= max_travel)
        continue;
      if (win)
        return state.iteration + travel + number_of_walls * 0.5;
      State new_state;
      new_state.position = current_position;
      new_state.iteration = state.iteration + travel;
      if (handled.count(new_state))
        continue;
      handled.insert(new_state);
      to_be_handled.push(new_state);
    }
  }
  return -1;
}

// static
Level Level::Random(int width, int height) {
  Level level;
  level.width_ = width;
  level.height_ = height;
  level.starting_point_.x = width / 2;
  level.starting_point_.y = height / 2;
  level.next_position = level.starting_point_;
  level.current_position = level.starting_point_;
  level.cases_ = std::vector<int>(width * height, '1');
  for (int y = 1; y < height - 1; ++y) {
    for (int x = 1; x < width - 1; ++x) {
      level.setCase({x, y}, '0');
    }
  }
  level.setCase({1, 1}, 's');
  level.setCase({width/2, height/2}, 'j');
  return level;
}

void Level::Mutate(std::mt19937& random) {
  // Swap two case.
  int a = std::uniform_int_distribution<int>(0, cases_.size() - 1)(random);
  int b = std::uniform_int_distribution<int>(0, cases_.size() - 1)(random);
  std::swap(cases_[a], cases_[b]);

  // Remove a random wall.
  {
    int p = std::uniform_int_distribution<int>(0, cases_.size() - 1)(random);
    if (cases_[p] == '1') {
      cases_[p] = '0';
      return;
    }
  }

  // Add a random block.
  {
    int p = std::uniform_int_distribution<int>(0, 12)(random);
    switch(p) {
      case 0: cases_[p] = '0'; break;
      case 1: cases_[p] = 'l'; break;
      case 2: cases_[p] = 'o'; break;
      case 3: cases_[p] = 'p'; break;
      case 4: cases_[p] = 'm'; break;
      //case 5: cases_[p] = 'i'; break;
      //case 6: cases_[p] = 't'; break;
      default:  cases_[p] = '1'; break;
    }
  }
}

void Level::Init(smk::RenderTarget& surface) {
  for (int i = 0; i < (int)cases_.size(); ++i) {
    if (cases_[i] == 'j') {
      starting_point_ = {i % width_, i / width_};
      cases_[i] = '0';
    }
  }
  current_position = starting_point_;
  next_position = starting_point_;
  anim = 0;
  pos.x = current_position.x * 32;
  pos.y = current_position.y * 32;
  (void)surface;
  //UpdateView(surface);
}

void Level::PlaySoundInternal(const smk::SoundBuffer& snd) {
  if (in_evaluate_)
    return;
  PlaySound(snd);
}
