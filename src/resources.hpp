#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <iostream>
#include <memory>
#include <smk/Font.hpp>
#include <smk/Sound.hpp>
#include <smk/SoundBuffer.hpp>
#include <smk/Sprite.hpp>
#include <smk/Texture.hpp>
#include <vector>

std::vector<std::string> SkinList();
bool GetSkin();
void SetSkin(std::string filename);
void PlaySound(const smk::SoundBuffer& snd);

std::string ResourcePath();
std::string SavePath();

extern std::string skin;
extern smk::Font font_arial;
extern smk::Font font_arial_20;
extern smk::Sound boing;
extern smk::Sound ouverture_cle;
extern smk::Sound plop;
extern smk::SoundBuffer boingsb;
extern smk::SoundBuffer ouverture_clesb;
extern smk::SoundBuffer plopsb;
extern smk::SoundBuffer sb_intro;
extern smk::SoundBuffer sb_press_enter;
extern smk::SoundBuffer sb_menu_select;
extern smk::SoundBuffer sb_menu_change;
extern smk::SoundBuffer sb_success;
extern smk::SoundBuffer sb_get_key;
extern smk::SoundBuffer sb_lose;

extern smk::Sprite angle1;
extern smk::Sprite angle2;
extern smk::Sprite angle3;
extern smk::Sprite angle4;
extern smk::Sprite block;
extern smk::Sprite cle;
extern smk::Sprite glass;
extern smk::Sprite joueur;
extern smk::Sprite serrure;
extern smk::Sprite sortie;
extern smk::Sprite vortex;
extern smk::Texture angleimg;
extern smk::Texture blockimg;
extern smk::Texture cleimg;
extern smk::Texture glassimg;
extern smk::Texture joueurimg;
extern smk::Texture serrureimg;
extern smk::Texture sortieimg;
extern smk::Texture vorteximg;

extern smk::Texture texture_keyboard;
extern smk::Sprite sprite_key_left;
extern smk::Sprite sprite_key_right;
extern smk::Sprite sprite_key_up;
extern smk::Sprite sprite_key_down;
extern smk::Sprite sprite_key_enter;
extern smk::Sprite sprite_key_escape;
extern smk::Texture texture_intro_screen_;

void LoadResources();

#endif
