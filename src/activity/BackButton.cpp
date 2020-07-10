// Copyright 2019 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#include "activity/BackButton.hpp"
#include <cmath>
#include <smk/Color.hpp>
#include <smk/Shape.hpp>
#include <smk/Text.hpp>
#include <smk/Input.hpp>
#include "resources.hpp"

const glm::vec4 backgroung_color = {1.0, 1.0, 1.0, 0.2};
const glm::vec4 backgroung_color_hover = {1.0, 1.0, 1.0, 1.f};
const glm::vec4 arrow_color = {0.f, 0.f, 0.f, 1.f};
const glm::vec4 arrow_color_hover = {0.f, 0.f, 0.f, 1.f};
const float circle_diameter = 48.f;

void BackButton::Step() {
  float width = window().width();
  float height = window().height();
  glm::vec2 center = glm::vec2(width, height) * 0.5f;
  back_button_position = glm::vec2(-width, height) * 0.5f + glm::vec2(80, -80);
  auto delta =
      back_button_position - (window().input().cursor() - center);
  delta.x /= circle_diameter;
  delta.y /= circle_diameter;
  back_button_hover = glm::length(delta) < 1.f;

  if (window().input().IsCursorReleased() && back_button_hover)
    on_quit();
}

void BackButton::Draw() {
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glStencilFunc(GL_ALWAYS, 0, 0xffffffff);
  glFrontFace(GL_CCW);
  glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set the view.
  float width = window().width();
  float height = window().height();
  smk::View view;
  view.SetCenter(0, 0);
  view.SetSize(width, height);
  window().SetShaderProgram(window().shader_program_2d());
  window().SetView(view);

  auto circle = smk::Shape::Circle(1.f, 80);
  circle.SetPosition(back_button_position);
  circle.SetScale(circle_diameter, circle_diameter);

  auto text = smk::Text(font_arial, L"←");
  float scale = circle_diameter * 0.025f;
  text.SetPosition(back_button_position -
                   text.ComputeDimensions() * 0.5f * scale);
  text.Move(0.f, -17.f);
  text.SetScale(scale, scale);

  circle.SetColor(back_button_hover ? backgroung_color_hover
                                    : backgroung_color);
  text.SetColor(back_button_hover ? arrow_color_hover : arrow_color);

  window().Draw(circle);
  window().Draw(text);
  window().Draw(circle);
  window().Draw(text);
}
