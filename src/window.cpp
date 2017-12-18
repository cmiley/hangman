#include <window.hpp>

Window::Window()
{
  gWindow = nullptr;
  show_another_window = false;
  show_test_window = false;
  clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  imgui_vars.ambient = glm::vec3(0.5);
  imgui_vars.specular = glm::vec3(0.6);
  imgui_vars.spot_size = 2.0;
  imgui_vars.spot_brightness = 0.5;
  imgui_vars.shininess = 0.2;
  imgui_vars.zoom = 1.0;
  imgui_vars.meat_mode = false;
  imgui_vars.s_selector = 2;
  imgui_vars.tableAngle = 0.0;
}

Window::~Window()
{
  ImGui_ImplSdlGL3_Shutdown();
  SDL_StopTextInput();
  SDL_DestroyWindow(gWindow);
  gWindow = nullptr;
  SDL_Quit();
}

bool Window::Initialize(const string &name, int* width, int* height)
{
    // Start SDL
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL failed to initialize: %s\n", SDL_GetError());
    return false;
  }

  // Start OpenGL for SDL
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  // Create window
  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);
  
  //use for fullscreen
  if (*height == 0 && *width == 0)
  {
    *height = current.h;
    *width = current.w;
  }

  gWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, *width, *height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
  if(gWindow == nullptr)
  {
    printf("Widow failed to create: %s\n", SDL_GetError());
    return false;
  }

  ImGui_ImplSdlGL3_Init(gWindow);

  // Create context
  gContext = SDL_GL_CreateContext(gWindow);
  if(gContext == nullptr)
  {
    printf("OpenGL context not created: %s\n", SDL_GetError());
    return false;
  }

  // Use VSync
  if(SDL_GL_SetSwapInterval(1) < 0)
  {
    printf("Unable to use VSync: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

void Window::RunMenu(string text, int num_wrong, string gameOver)
{

  ImGui_ImplSdlGL3_NewFrame(gWindow);
  bool value = true;

  string len = to_string(text.length());
  string guess_left = to_string(6-num_wrong);

  {
    ImGui::Begin("Hangman Menu", &value);
    ImGui::Text("Your word is "); ImGui::SameLine(); ImGui::Text(len.c_str()); ImGui::SameLine(); ImGui::Text(" letters long...");
    ImGui::Text("Your current word is: "); ImGui::SameLine(); ImGui::Text(text.c_str());
    ImGui::Text("You have "); ImGui::SameLine(); ImGui::Text(guess_left.c_str()); ImGui::SameLine(); ImGui::Text(" guesses left. "); ImGui::SameLine(); ImGui::Text(gameOver.c_str());
//    ImGui::Text("Edit the variables below to see changes in the lighting of the program.");
//    ImGui::SliderFloat("Zoom", &imgui_vars.zoom, 0.0f, 2.0f);
//    ImGui::SliderFloat("Spotlight Size", &imgui_vars.spot_size, 0.0f, 5.0f);
//    ImGui::SliderFloat("Spotlight Brightness", &imgui_vars.spot_brightness, 0.0f, 1.0f);
    ImGui::ColorEdit3("Ambient Brightness", (float*)&imgui_vars.ambient);
    ImGui::ColorEdit3("Specular Brightness", (float*)&imgui_vars.specular);
    ImGui::ColorEdit3("Diffuse Brightness", (float*)&imgui_vars.diffuse);
    ImGui::SliderFloat("Shininess coefficient", &imgui_vars.shininess, 0.0f, 1.0f);
//    ImGui::SliderFloat3("Light Position", imgui_vars.light_pos, 0, 10);
    ImGui::SliderFloat("Gravity Angle", &imgui_vars.tableAngle, -30., 30.);
//    ImGui::Checkbox("MEAT MODE", &imgui_vars.meat_mode);
    ImGui::RadioButton("Passthrough", &imgui_vars.s_selector, 0); ImGui::SameLine();
    ImGui::RadioButton("Per Vertex", &imgui_vars.s_selector, 1); ImGui::SameLine();
    ImGui::RadioButton("Per Fragment", &imgui_vars.s_selector, 2);
//    ImGui::ColorEdit3("Spotlight Intensities", (float*)&imgui_vars.spotlight.intensities);
//    ImGui::SliderFloat("Attenuation", &imgui_vars.spotlight.attenuation, 0.0f, 1.0f);
//    ImGui::SliderFloat("Ambient Coefficient", &imgui_vars.spotlight.ambientCoefficient, 0.0f, 1.0f);
//    ImGui::SliderAngle("Cone Angle", &imgui_vars.spotlight.coneAngle);
//    ImGui::SliderFloat3("Spotlight Position", imgui_vars.spot_pos, 0, 10);
//    ImGui::SliderFloat3("Cone Direction", imgui_vars.cone_dir, 0, 20);
//    ImGui::SliderFloat("Gravity", imgui_vars.gravity, 0, 20);
//    if (ImGui::Button("Test Window")) show_test_window ^= 1;
//    if (ImGui::Button("Another Window")) show_another_window ^= 1;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }

  // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name the window.
  if (show_another_window)
  {
    ImGui::Begin("Another Window", &show_another_window);
    ImGui::Text("Hello from another window!");
    ImGui::End();
  }

  // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow().
  if (show_test_window)
  {
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
    ImGui::ShowTestWindow(&show_test_window);
  }

  // Rendering
  glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
  ImGui::Render();
}

void Window::Swap()
{
  SDL_GL_SwapWindow(gWindow);
}

MenuVars* Window::getVars()
{
  return &imgui_vars;
}

void Window::SetZoom(float new_zoom)
{
  imgui_vars.zoom = new_zoom;
}

void Window::setSelector(int num)
{
  switch (num)
  {
    case 0:
      imgui_vars.s_selector = 0;
      break;
    case 1:
      imgui_vars.s_selector = 1;
      break;
    case 2:
      imgui_vars.s_selector = 2;
      break;
    default:
      imgui_vars.s_selector = 2;
      break;
  }
}

float Window::getAngle()
{
  return imgui_vars.tableAngle;
}