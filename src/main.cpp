#include <iostream>
#include <memory>
#include <smk/Audio.hpp>
#include <smk/Color.hpp>
#include <smk/Sound.hpp>
#include <smk/SoundBuffer.hpp>
#include <smk/Sprite.hpp>
#include <smk/Texture.hpp>
#include <string>
#include "Level.hpp"
#include "LevelGenerator.hpp"
#include "activity/IntroScreen.hpp"
#include "activity/Activity.hpp"
#include "activity/LevelActivity.hpp"
#include "activity/Menu.hpp"
#include "activity/LevelGeneratorActivity.hpp"
#include "activity/LevelGeneratorSizeSelector.hpp"
#include "activity/Credit.hpp"
#include "resources.hpp"

namespace {
bool quit = false;
}  // namespace

std::vector<std::string> GetList(std::string file) {
  std::ifstream flux(file.c_str(), std::ios::in);
  if (!flux)
    std::cout << "erreur d'ouverture du fichier: " << file << std::endl;
  std::vector<std::string> list;
  std::string line;
  while (true) {
    if (!getline(flux, line))
      break;
    list.push_back(line);
  }
  flux.close();
  return list;
}

int GetSave(std::string pack) {
  std::string path = SavePath() + "/.icemaze." + pack + ".sav";
  std::ifstream file(path);
  std::string line;
  if (getline(file, line))
    return std::stoi(line);

  return 0;
}

void SetSave(std::string pack, int value) {
  std::string path = SavePath() + "/.icemaze." + pack + ".sav";
  std::ofstream(path) << value;

#ifdef __EMSCRIPTEN__
  EM_ASM(FS.syncfs(false, function(err){console.log(err)});, 0);
#endif
}

class ActivityManager {
 public:
  ActivityManager() {
    window_ = smk::Window(640, 480, "InTheCube");
    skin_loaded = GetSkin();
    LoadResources();

    intro_screen_ = std::make_unique<IntroScreen>(window_);
    intro_screen_->on_enter = [&] { Display(main_menu_.get()); };

    // First the main menu.
    main_menu_ = std::make_unique<Menu>(window_);
    main_menu_->entries = {
        "Play",
        "Skin",
        "Credit",
        "Quit",
    };
    main_menu_->on_enter = [&]() {
      // clang-format off
      switch (main_menu_->selected) {
        case 0: Display(play_menu_.get()); break;
        case 1: Display(skin_menu_.get()); break;
        case 2: Display(credit_.get()); break;
        case 3: quit = true; break;
      }
      // clang-format on
    };

    play_menu_ = std::make_unique<Menu>(window_);
    play_menu_->entries = {
        "Human levels",
        "Machine levels",
    };
    play_menu_->on_enter = [&] {
      // clang-format off
      switch (play_menu_->selected) {
        case 0: Display(pack_explorer_.get()); break;
        case 1: Display(level_generator_size_selector_.get()); break;
      }
      // clang-format on
    };
    play_menu_->on_escape = [&] { Display(main_menu_.get()); };

    // Skin menu
    skin_menu_ = std::make_unique<Menu>(window_);
    skin_menu_->entries = SkinList();
    for (int i = 0; i < (int)skin_menu_->entries.size(); i++) {
      if (skin == skin_menu_->entries[i])
        skin_menu_->selected = i;
    }
    skin_menu_->on_change = [&] { OnUpdateSkin(); };
    skin_menu_->on_escape = [&] { Display(main_menu_.get()); };
    skin_menu_->on_enter = [&] { OnUpdateSkin(); Display(main_menu_.get()); };

    // Level generator size selector
    level_generator_size_selector_ =
        std::make_unique<LevelGeneratorSizeSelector>(window_);
    level_generator_size_selector_->on_enter = [&]() {
      level_generator_activity_->width = level_generator_size_selector_->width;
      level_generator_activity_->height =
          level_generator_size_selector_->height;
      Display(level_generator_activity_.get());
    };
    level_generator_size_selector_->on_escape = [&] {
      Display(play_menu_.get());
    };

    // Level generator
    level_generator_activity_ =
        std::make_unique<LevelGeneratorActivity>(window_);
    level_generator_activity_->on_escape = [&] {
      Display(level_generator_size_selector_.get());
    };

    // Level pack activity
    pack_explorer_ = std::make_unique<Menu>(window_);
    pack_explorer_->entries = GetList(ResourcePath() + "/level/PackFile");
    pack_explorer_->save = 999;
    pack_explorer_->on_enter = [&] {
      std::string pack = pack_explorer_->entries[pack_explorer_->selected];
      level_explorer_->entries = GetList(ResourcePath() + "/level/" + pack + "/LevelList");
      level_explorer_->save = GetSave(pack);
      Display(level_explorer_.get());
    };
    pack_explorer_->on_escape = [&] { Display(play_menu_.get()); };

    // Level selection activity
    level_explorer_ = std::make_unique<Menu>(window_);
    level_explorer_->on_enter = [&] { PlayLevel(); };
    level_explorer_->on_escape = [&] { Display(pack_explorer_.get()); };

    // Level activity
    level_activity_ = std::make_unique<LevelActivity>(window_);
    level_activity_->on_lose = [&] { PlayLevel(); };
    level_activity_->on_escape = [&] { Display(level_explorer_.get()); };
    level_activity_->on_win = [&] { LevelWin(); };

    // Credit
    credit_ = std::make_unique<Credit>(window_);
    credit_->on_quit = [&] { Display(main_menu_.get()); };

    Display(intro_screen_.get());
    Loop();
  }

  void Display(Activity* activity) {
    if (activity_)
      activity_->OnQuit();
    if (activity)
      activity->OnEnter();
    std::swap(activity_, activity);
  }

  void Loop() {
    if (!skin_loaded) {
      skin_loaded = GetSkin();
      if (skin_loaded)
        LoadResources();
    }

    window_.PoolEvents();
    activity_->Step();
    window_.Clear(smk::Color::Black);
    activity_->Draw();
#ifndef __EMSCRIPTEN__
    window_.LimitFrameRate(60.f);
#endif
    window_.Display();
  }

  void PlayLevel() {
    std::string pack = pack_explorer_->entries[pack_explorer_->selected];
    std::string level = level_explorer_->entries[level_explorer_->selected];
    auto lvl = Level(ResourcePath() + "/level/" + pack + "/" + level);
    level_activity_->level = Level(ResourcePath() + "/level/" + pack + "/" + level);
    level_activity_->level.Init(window_);

    Display(level_activity_.get());
  };

  void LevelWin() {
    std::string pack = pack_explorer_->entries[pack_explorer_->selected];
    int& selected = level_explorer_->selected;
    int& save = level_explorer_->save;
    if (selected == save) {
      save++;
      SetSave(pack, save);
    }

    Display(level_explorer_.get());
  }

  void OnUpdateSkin() {
    std::string skin = skin_menu_->entries[skin_menu_->selected];
    SetSkin(skin);
    GetSkin();
    LoadResources();
  }

 private:
  bool skin_loaded = false;

  smk::Window window_;
  Activity* activity_ = nullptr;
  std::unique_ptr<IntroScreen> intro_screen_;
  std::unique_ptr<Menu> main_menu_;
  std::unique_ptr<Menu> play_menu_;
  std::unique_ptr<Menu> skin_menu_;
  std::unique_ptr<Menu> pack_explorer_;
  std::unique_ptr<Menu> level_explorer_;
  std::unique_ptr<LevelActivity> level_activity_;
  std::unique_ptr<LevelGeneratorActivity> level_generator_activity_;
  std::unique_ptr<LevelGeneratorSizeSelector> level_generator_size_selector_;
  std::unique_ptr<Credit> credit_;
};

std::unique_ptr<ActivityManager> activity_manager;
void MainLoop() {
  activity_manager->Loop();
}

int main() {
  smk::Audio audio;
  std::locale::global(std::locale("C.UTF-8"));

#ifdef __EMSCRIPTEN__
  // clang-format off
  EM_ASM(
      FS.mkdir('/sav');
      FS.mount(IDBFS, {}, '/sav');
      FS.syncfs(true, function(err){console.log("IndexDB synced", err)});
  , 0);
  // clang-format on
#endif

  activity_manager = std::make_unique<ActivityManager>();
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(&MainLoop, 0, 1);
#else
  while(!quit)
    MainLoop();
#endif
  activity_manager.reset();
  return EXIT_SUCCESS;
}
