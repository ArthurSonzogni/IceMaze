#include "resources.hpp"
#include <iostream>
#include <string>
#include <fstream>

std::string skin;
std::string skin_path;

std::vector<std::string> SkinList() {
  std::vector<std::string> output;
  for (std::string path : {"./skin/SkinList", "../skin/SkinList"}) {
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
  for (std::string path : {"./skin/", "../skin/"}) {
#endif
    std::ifstream file(path + "Skin");
    if (!file)
      continue;
    std::cerr << "Reading from" << path + "Skin" << std::endl;
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
  for (std::string path : {"./skin/", "../skin/"}) {
#endif
    {
      std::ofstream file(path);
      if (!file)
        continue;
      file << filename;
      std::cerr << "Writting to" << path << std::endl;
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
smk::Sound boing;
smk::Sound ouverture_cle;
smk::Sound plop;
smk::SoundBuffer boingsb;
smk::SoundBuffer ouverture_clesb;
smk::SoundBuffer plopsb;
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

void LoadResources() {
  // images
  glassimg = smk::Texture(skin_path + "img/glass.bmp");
  glass.SetTexture(glassimg);

  smk::Texture::Option nearest_filter;
  nearest_filter.min_filter = GL_NEAREST;
  nearest_filter.mag_filter = GL_NEAREST;
  blockimg = smk::Texture(skin_path + "img/block.bmp", nearest_filter);
  block.SetTexture(blockimg);

  joueurimg = smk::Texture(skin_path + "img/joueur.bmp");
  joueur.SetTexture(joueurimg);

  sortieimg = smk::Texture(skin_path + "img/sortie.bmp");
  sortie.SetTexture(sortieimg);

  angleimg = smk::Texture(skin_path + "img/angle.png");
  angle1.SetTexture(angleimg);
  angle2.SetTexture(angleimg);
  angle3.SetTexture(angleimg);
  angle4.SetTexture(angleimg);

  angle2.SetCenter(0, 32);
  angle2.SetRotation(270);

  angle3.SetCenter(32, 32);
  angle3.SetRotation(180);

  angle4.SetCenter(32, 0);
  angle4.SetRotation(90);

  cleimg = smk::Texture(skin_path + "img/cle.png");
  cle.SetTexture(cleimg);

  serrureimg = smk::Texture(skin_path + "img/serrure.bmp");
  serrure.SetTexture(serrureimg);

  vorteximg = smk::Texture(skin_path + "img/vortex.png");
  vortex.SetTexture(vorteximg);
  vortex.SetCenter(16, 16);

  // sons
  plopsb = smk::SoundBuffer(skin_path + "snd/plop.ogg");
  plop.SetBuffer(plopsb);

  boingsb = smk::SoundBuffer(skin_path + "snd/boing.ogg");
  boing.SetBuffer(boingsb);

  ouverture_clesb = smk::SoundBuffer(skin_path + "snd/ouverture_cle.ogg");
  ouverture_cle.SetBuffer(ouverture_clesb);
  ouverture_cle.SetVolume(200);

  font_arial = smk::Font(skin_path + "../font_arial.ttf", 40);
}
