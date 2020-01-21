#include "resources.hpp"
#include <fstream>
#include <iostream>
#include <string>

std::string GetEnvironmentVariable(const char* env) {
  auto value = std::getenv(env);
  if (value)
    return value;
  return std::string();
}

std::string ResourcePath() {
  static bool initialized = false;
  static std::string resource_path;
  if (initialized)
    return resource_path;
  initialized = true;

  auto SNAP = GetEnvironmentVariable("SNAP");

  std::vector<std::string> path_list = {
    // Application installed using snapcraft.
    SNAP + "/usr/local/share/icemaze/resources",

    // Application installed using "sudo make install"
    "/usr/local/share/icemaze/resources",

    // Code build and run inside ${CMAKE_CURRENT_DIRECTORY}/build
    "./resources",

    // Code build and run inside ${CMAKE_CURRENT_DIRECTORY}
    "../resources",

    // Code build for WebAssembly.
    "/resources",
  };

  for (auto& path : path_list) {
    auto file = std::ifstream(path + "/icemaze");
    if (!file)
      continue;

    std::string line;
    if (std::getline(file, line) && line == "icemaze") {
      resource_path = path;
    }
  }

  std::cerr << "Resource path = " << resource_path << std::endl;
  return resource_path;
}

std::string SavePath() {
  static bool initialized = false;
  static std::string save_path;

  if (initialized)
    return save_path;
  initialized = true;

#ifdef __EMSCRIPTEN__
  save_path = "/sav";
  return save_path;
#endif

  auto SNAP_USER_COMMON = GetEnvironmentVariable("SNAP_USER_COMMON");
  auto HOME = GetEnvironmentVariable("HOME");

  if (!SNAP_USER_COMMON.empty()) {
    save_path = SNAP_USER_COMMON;
  } else if (!HOME.empty()) {
    save_path = HOME;
  } else {
    save_path = ".";
  }

  std::cerr << "Save path = " << save_path << std::endl;
  return save_path;
}

std::string skin;

std::string SkinPath() {
  return ResourcePath() + "/skin/" + skin;
}

std::vector<std::string> SkinList() {
  std::vector<std::string> output;
  std::string path = ResourcePath() + "/skin/SkinList";
  std::ifstream file(path);
  if (!file) {
    std::cerr << "Skin list not found in " << path << std::endl;
    return output;
  }

  std::string line;
  while (getline(file, line))
    output.push_back(line);
  return output;
}

bool GetSkin() {
  skin = "default";
  std::string skin_save_path = SavePath() + "/.icemaze.skin.sav";
  std::ifstream file(skin_save_path);
  if (!file) {
    SetSkin(skin);
    return true;
  }

  std::string line;
  if (getline(file, line)) {
    skin = line;
    return true;
  }

  return false;
}

void SetSkin(std::string filename) {
  std::string skin_save_path = SavePath() + "/.icemaze.skin.sav";
  std::ofstream file(skin_save_path);
  if (!file) {
    std::cerr << "Can't save to " << skin_save_path << std::endl;
    return;
  }

  file << filename;
#ifdef __EMSCRIPTEN__
  EM_ASM(FS.syncfs(false, function(err){console.log(err)});, 0);
#endif
  return;
}

// liste des image/sprite;
smk::Font font_arial;
smk::Font font_arial_20;

smk::SoundBuffer boingsb;
smk::SoundBuffer ouverture_clesb;
smk::SoundBuffer plopsb;
smk::SoundBuffer sb_intro;
smk::SoundBuffer sb_press_enter;
smk::SoundBuffer sb_menu_select;
smk::SoundBuffer sb_menu_change;
smk::SoundBuffer sb_success;
smk::SoundBuffer sb_get_key;
smk::SoundBuffer sb_lose;

smk::Sprite angle1;
smk::Texture angleimg;
smk::Texture blockimg;
smk::Texture cleimg;
smk::Texture glassimg;
smk::Texture joueurimg;
smk::Texture serrureimg;
smk::Texture sortieimg;
smk::Texture vorteximg;
smk::Texture texture_keyboard;
smk::Texture texture_intro_screen_;

smk::Sprite angle2;
smk::Sprite angle3;
smk::Sprite angle4;
smk::Sprite block;
smk::Sprite cle;
smk::Sprite glass;
smk::Sprite joueur;
smk::Sprite serrure;
smk::Sprite sortie;
smk::Sprite vortex;
smk::Sprite sprite_key_left;
smk::Sprite sprite_key_right;
smk::Sprite sprite_key_up;
smk::Sprite sprite_key_down;
smk::Sprite sprite_key_enter;
smk::Sprite sprite_key_escape;

void LoadResources() {
  smk::Texture::Option nearest_filter;
  nearest_filter.min_filter = GL_NEAREST;
  nearest_filter.mag_filter = GL_NEAREST;
  texture_intro_screen_ =
      smk::Texture(ResourcePath() + "/intro_screen.png", nearest_filter);

  // Textures
  glassimg = smk::Texture(SkinPath() + "/glass.bmp");
  blockimg = smk::Texture(SkinPath() + "/block.bmp");
  joueurimg = smk::Texture(SkinPath() + "/joueur.bmp");
  sortieimg = smk::Texture(SkinPath() + "/sortie.bmp");
  angleimg = smk::Texture(SkinPath() + "/angle.png");
  cleimg = smk::Texture(SkinPath() + "/cle.png");
  serrureimg = smk::Texture(SkinPath() + "/serrure.bmp");
  vorteximg = smk::Texture(SkinPath() + "/vortex.png");
  texture_keyboard = smk::Texture(ResourcePath() + "/keyboard.png");

  glass = smk::Sprite(glassimg);
  block = smk::Sprite(blockimg);
  joueur = smk::Sprite(joueurimg);
  sortie = smk::Sprite(sortieimg);
  cle = smk::Sprite(cleimg);
  serrure = smk::Sprite(serrureimg);
  vortex = smk::Sprite(vorteximg);
  sprite_key_up = smk::Sprite(texture_keyboard, {0, 0, 32, 32});
  sprite_key_right = smk::Sprite(texture_keyboard, {32, 0, 64, 32});
  sprite_key_left = smk::Sprite(texture_keyboard, {0, 32, 32, 64});
  sprite_key_down = smk::Sprite(texture_keyboard, {32, 32, 64, 64});
  sprite_key_enter = smk::Sprite(texture_keyboard, {0, 64, 64, 96});
  sprite_key_escape = smk::Sprite(texture_keyboard, {0, 96, 64, 128});
  angle1 = smk::Sprite(angleimg);
  angle2 = smk::Sprite(angleimg);
  angle3 = smk::Sprite(angleimg);
  angle4 = smk::Sprite(angleimg);

  angle2.SetCenter(0, 32);
  angle2.SetRotation(270);

  angle3.SetCenter(32, 32);
  angle3.SetRotation(180);

  angle4.SetCenter(32, 0);
  angle4.SetRotation(90);

  vortex.SetCenter(16, 16);


  // sons
  plopsb = smk::SoundBuffer(ResourcePath() + "/plop.ogg");
  boingsb = smk::SoundBuffer(ResourcePath() + "/boing.ogg");
  ouverture_clesb = smk::SoundBuffer(ResourcePath() + "/ouverture_cle.ogg");
  sb_intro = smk::SoundBuffer(ResourcePath() + "/intro.wav");
  sb_press_enter = smk::SoundBuffer(ResourcePath() + "/press_enter.wav");
  sb_menu_select = smk::SoundBuffer(ResourcePath() + "/menu_select.wav");
  sb_menu_change = smk::SoundBuffer(ResourcePath() + "/menu_change.wav");
  sb_success = smk::SoundBuffer(ResourcePath() + "/success.wav");
  sb_get_key = smk::SoundBuffer(ResourcePath() + "/get_key.wav");
  sb_lose = smk::SoundBuffer(ResourcePath() + "/lose.wav");

  // Fonts.
  font_arial = smk::Font(ResourcePath() + "/font_arial.ttf", 60);
  font_arial_20 = smk::Font(ResourcePath() + "/font_arial.ttf", 20);
}

namespace {
std::vector<smk::Sound> sounds;
int sound_index = 0;
}  // namespace

void PlaySound(const smk::SoundBuffer& snd) {
  sounds.resize(10);
  sound_index = (sound_index + 1) % sounds.size();
  sounds[sound_index] = smk::Sound(snd);
  sounds[sound_index].Play();
}
