#include "resources.hpp"
#include <fstream>
#include <iostream>
#include <string>

std::string skin;
std::string skin_path;

std::vector<std::string> SkinList() {
  std::vector<std::string> output;
  for (std::string path : {"./skin/SkinList", "../skin/SkinList", "/etc/level/skin/SkinList"}) {
    std::ifstream file(path);
    if (!file)
      continue;
    std::string line;
    while (getline(file, line))
      output.push_back(line);
    return output;
  }
  return output;
}

bool GetSkin() {
  skin = "default";
  skin_path = "./skin/default/";
#ifdef __EMSCRIPTEN__
  for (std::string path : {"/sav/"}) {
#else
  for (std::string path : {"./skin/", "../skin/", "/etc/level/skin/"}) {
#endif
    std::ifstream file(path + "Skin");
    if (!file)
      continue;
    std::string line;
    getline(file, line);
    skin = line;
#ifdef __EMSCRIPTEN__
    skin_path = "./skin/" + skin + "/";
#else
    skin_path = path + skin + "/";
#endif
    return true;
  }
  return false;
}
void SetSkin(std::string filename) {
#ifdef __EMSCRIPTEN__
  for (std::string path : {"/sav/Skin"}) {
#else
  for (std::string path : {"./skin/Skin", "../skin/Skin", "/etc/level/skin/Skin"}) {
#endif
    {
      std::ofstream file(path);
      if (!file)
        continue;
      file << filename;
    }
#ifdef __EMSCRIPTEN__
    EM_ASM(FS.syncfs(false, function(err){console.log(err)});, 0);
#endif
    return;
  }
  std::cerr << "Error: Not able to set skin" << std::endl;
}

// liste des image/sprite;
smk::Font font_arial;
smk::Font font_arial_20;
smk::Sound boing;
smk::Sound ouverture_cle;
smk::Sound plop;
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
smk::Texture angleimg;
smk::Texture blockimg;
smk::Texture cleimg;
smk::Texture glassimg;
smk::Texture joueurimg;
smk::Texture serrureimg;
smk::Texture sortieimg;
smk::Texture vorteximg;

smk::Texture texture_keyboard;
smk::Sprite sprite_key_left;
smk::Sprite sprite_key_right;
smk::Sprite sprite_key_up;
smk::Sprite sprite_key_down;
smk::Sprite sprite_key_enter;
smk::Sprite sprite_key_escape;
smk::Texture texture_intro_screen_;

void LoadResources() {
  smk::Texture::Option nearest_filter;
  nearest_filter.min_filter = GL_NEAREST;
  nearest_filter.mag_filter = GL_NEAREST;
  texture_intro_screen_ =
      smk::Texture(skin_path + "../intro_screen.png", nearest_filter);

  // images
  glassimg = smk::Texture(skin_path + "glass.bmp");
  glass.SetTexture(glassimg);

  blockimg = smk::Texture(skin_path + "block.bmp");
  block.SetTexture(blockimg);

  joueurimg = smk::Texture(skin_path + "joueur.bmp");
  joueur.SetTexture(joueurimg);

  sortieimg = smk::Texture(skin_path + "sortie.bmp");
  sortie.SetTexture(sortieimg);

  angleimg = smk::Texture(skin_path + "angle.png");
  angle1.SetTexture(angleimg);

  angle2.SetTexture(angleimg);
  angle2.SetCenter(0, 32);
  angle2.SetRotation(270);

  angle3.SetTexture(angleimg);
  angle3.SetCenter(32, 32);
  angle3.SetRotation(180);

  angle4.SetTexture(angleimg);
  angle4.SetCenter(32, 0);
  angle4.SetRotation(90);

  cleimg = smk::Texture(skin_path + "cle.png");
  cle.SetTexture(cleimg);

  serrureimg = smk::Texture(skin_path + "serrure.bmp");
  serrure.SetTexture(serrureimg);

  vorteximg = smk::Texture(skin_path + "vortex.png");
  vortex.SetTexture(vorteximg);
  vortex.SetCenter(16, 16);

  texture_keyboard = smk::Texture(skin_path + "../keyboard.png");

  sprite_key_up.SetTexture(texture_keyboard);
  sprite_key_up.SetTextureRectangle({0, 0, 32, 32});

  sprite_key_right.SetTexture(texture_keyboard);
  sprite_key_right.SetTextureRectangle({32, 0, 64, 32});

  sprite_key_left.SetTexture(texture_keyboard);
  sprite_key_left.SetTextureRectangle({0, 32, 32, 64});

  sprite_key_down.SetTexture(texture_keyboard);
  sprite_key_down.SetTextureRectangle({32, 32, 64, 64});

  sprite_key_enter.SetTexture(texture_keyboard);
  sprite_key_enter.SetTextureRectangle({0, 64, 64, 96});

  sprite_key_escape.SetTexture(texture_keyboard);
  sprite_key_escape.SetTextureRectangle({0, 96, 64, 128});

  // sons
  plopsb = smk::SoundBuffer(skin_path + "../plop.ogg");
  plop.SetBuffer(plopsb);

  boingsb = smk::SoundBuffer(skin_path + "../boing.ogg");
  boing.SetBuffer(boingsb);

  ouverture_clesb = smk::SoundBuffer(skin_path + "../ouverture_cle.ogg");
  ouverture_cle.SetBuffer(ouverture_clesb);

  sb_intro = smk::SoundBuffer(skin_path + "../intro.wav");
  sb_press_enter = smk::SoundBuffer(skin_path + "../press_enter.wav");
  sb_menu_select = smk::SoundBuffer(skin_path + "../menu_select.wav");
  sb_menu_change = smk::SoundBuffer(skin_path + "../menu_change.wav");
  sb_success = smk::SoundBuffer(skin_path + "../success.wav");
  sb_get_key = smk::SoundBuffer(skin_path + "../get_key.wav");
  sb_lose = smk::SoundBuffer(skin_path + "../lose.wav");

  // Fonts.
  font_arial = smk::Font(skin_path + "../font_arial.ttf", 40);
  font_arial_20 = smk::Font(skin_path + "../font_arial.ttf", 20);
}

namespace {
std::vector<smk::Sound> sounds;
int sound_index = 0;
}  // namespace

void PlaySound(const smk::SoundBuffer& snd) {
  sounds.resize(10);
  sound_index = (sound_index + 1) % sounds.size();
  sounds[sound_index].SetBuffer(snd);
  sounds[sound_index].Play();
}
