#ifndef GRAPHICS_HEADERS_H
#define GRAPHICS_HEADERS_H

#define NUM_LIGHTS 4

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
#endif

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>
#include <assimp/vector3.h>

#include <Magick++.h>

#include <SDL2/SDL.h>

#include <btBulletDynamicsCommon.h>

// ImGui for menus
#include "imgui.hpp"
#include "imgui_impl_sdl_gl3.h"

#define INVALID_UNIFORM_LOCATION 0x7fffffff

#define BIT(x) (1<<(x))

struct Vertex
{
  glm::vec3 vertex;
  glm::vec2 uv;
  glm::vec3 normal;

  Vertex(glm::vec3 v, glm::vec2 u, glm::vec3 n): vertex(v), uv(u), normal(n) {}
};

static void error_callback(int error, const char* description)
{
  fprintf(stderr, "Error %d: %s\n", error, description);
}

struct Light {
  glm::vec4 position;
  glm::vec3 intensities;
  float attenuation;
  float ambientCoefficient;
  float coneAngle;
  glm::vec3 coneDirection;
};

enum ShaderSelector
{
  PASSTHROUGH,
  PER_FRAGMENT,
  PER_VERTEX
};

struct MenuVars
{
  float zoom;
  glm::vec3 ambient;
  glm::vec3 specular;
  glm::vec3 diffuse;
  float spot_size;
  float spot_brightness;
  float shininess;
  float light_pos[3];
  float spot_pos[3];
  float cone_dir[3];
  Light spotlight;
  bool meat_mode;
  int s_selector;
  float tableAngle;
};

struct GL_Light
{
  GLint m_type;
  GLint m_position;
  GLint m_attenuation;
  GLint m_direction;
  GLint m_color;
};

#endif /* GRAPHICS_HEADERS_H */
