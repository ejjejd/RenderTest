#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "scene/scene.h"

#include "math/matrices/matrix4x4.h"
#include "scene/renders/forward-render.h"
#include "graphics/shaders/shader-pipeline.h"

#include "asset-manager/obj-loader.h"
#include "asset-manager/image-loader.h"
#include "asset-manager/text-loader.h"
#include "asset-manager/asset-manager.h"

#include "math/math_utils.h"
#include "utils/timer.h"
#include "utils/sync/spin-lock.h"

#include <cstring>
#include <Windows.h>

#include "gui.h"

#include "debug/log/log.h"
#include "bgl/debug/callback.h"

#include "jobs/job-system.h"

#include "bgl/window/window-wrapper.h"

#include "input/input-manager.h"

char g_KeyStates[1024];
graphics::Camera g_MainCamera;

float g_DeltaTime = 1.0f / 60.0f;

gui::GuiController g_GC;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
   if (action == GLFW_PRESS)
      g_KeyStates[key] = 1;
   if (action == GLFW_RELEASE)
      g_KeyStates[key] = 0;
}

void CursorCallback(GLFWwindow* window, double posX, double posY)
{
   g_MainCamera.Rotate(posX, posY, g_DeltaTime);

   g_GC.OnMouseEvent(posX, posY);
}

void ScrollCallback(GLFWwindow* window, double x, double y)
{
   g_GC.OnScrollEvent(y);
}

int main()
{
   bgl::WindowGL window;
   window.Instantiate();

   glfwSetKeyCallback(window.NativeHandle.get(), KeyCallback);
   glfwSetCursorPosCallback(window.NativeHandle.get(), CursorCallback);
   glfwSetScrollCallback(window.NativeHandle.get(), ScrollCallback);

   core::JobSystem::Setup();

   assets::AssetManager assetManager;

   assets::AssetRef pistolAssetRef = assetManager.RequireAsssetRef("res/meshes/pistol/pistol.obj");
   assets::AssetRef cubeAssetRef = assetManager.RequireAsssetRef("res/meshes/cube.obj");

   assets::AssetRef pistolTextureRef = assetManager.RequireAsssetRef("res/meshes/pistol/textures/handgun_C.jpg");
  
   //for(size_t i = 0; i < 15; ++i)
   //  assetManager.RequireAsssetId("res/meshes/pistol/pistol.obj");

   {
      utils::Timer assetTimer(true);
      assetManager.Load();
      LOG_WARNING("Assets loading time: %f ms", assetTimer.GetElaspedTime());
   }

   g_MainCamera = graphics::Camera(math::Vector3(0.0f, 0.0f, 10.0f), PI / 4, 1.7f, 5.0f);

   graphics::Material floorM;
   floorM.Color = math::Vector3(0.0f, 0.2f, 0.2f);
   floorM.Specular = math::Vector3(0.5f, 0.5f, 0.5f);
   floorM.ShineExponent = 10.0f;
   floorM.Emissive = math::Vector3(0.0f);

   graphics::Material pistolM;
   pistolM.Color = math::Vector3(0.7f, 0.2f, 0.2f);
   pistolM.DiffuseTexture = pistolTextureRef;
   pistolM.Specular = math::Vector3(1.0f);
   pistolM.ShineExponent = 10.0f;
   pistolM.Emissive = math::Vector3(0.0f);

   core::Scene scene;
   scene.SetCamera(&g_MainCamera);

   size_t floorMaterial  = scene.AddMaterial(floorM);
   size_t pistolMaterial = scene.AddMaterial(pistolM);

   scene.AddRenderObject({ graphics::Mesh(pistolAssetRef), CreateTranslateMatrix(math::Vector3(0.0f, 0.0f, 0.0f)), pistolMaterial });

   math::Matrix4 cubeTransform = CreateTranslateMatrix(math::Vector3(0.0f, -2.0f, 0.0f ));
   cubeTransform = cubeTransform * CreateScaleMatrix(math::Vector3(5.0f, 1.0f, 5.0f));
   scene.AddRenderObject({ graphics::Mesh(cubeAssetRef), cubeTransform, floorMaterial });
   
   scene.AddLight(graphics::PointLight(math::Vector3(1.2f, 1.0f, 2.0f), math::Vector3(1.0f, 1.0f, 1.0f), 0.09f, 1.0f, 0.032f));

   for(int i = 0; i < 1; ++i)
      scene.AddLight(graphics::Spotlight(math::Vector3(0, 10.0f, 0), math::Vector3(0.0f, -1.0f, 0.0f), math::Vector3(1.0f, 1.0f, 1.0f), PI / 12, PI / 15));

   graphics::ForwardRender::Initialize();

   LARGE_INTEGER freq;
   QueryPerformanceFrequency(&freq);

   utils::Timer deltaTimer;


   //
   // GUI start
   //

   g_GC.Setup(window.GetWidth(), window.GetHeight());

   const char* vertexShaderSrc = R"(
      #version 330 core

      layout(location = 0) in vec2 pos;
      layout(location = 1) in vec2 uv;

      out vec2 Uv;
      
      void main()
      {
         Uv = uv;
         gl_Position = vec4(pos, 0.0f, 1.0f);
      })";

   const char* fragmentShaderSrc = R"(
      #version 330 core
      
      out vec4 Color;
      
      in vec2 Uv;

      uniform sampler2D Texture;

      void main()
      {
         Color = texture(Texture, Uv);
      })";


   graphics::ShaderPipeline guiShader;
   guiShader.Create();

   guiShader.Add(graphics::ShaderType::Vertex, vertexShaderSrc);
   guiShader.Add(graphics::ShaderType::Fragment, fragmentShaderSrc);

   guiShader.Compile();

   float vertices[] =
   {
      -1.0f, -1.0f, 0.0f, 1.0f,
      -1.0f, 1.0f, 0.0f, 0.0f,
      1.0f, 1.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 1.0f, 0.0f,
      1.0f, -1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, 1.0f,
   };

   GLuint vao;
   GLuint vbo;

   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);

   glBindVertexArray(vao);

   glBindBuffer(GL_ARRAY_BUFFER, vbo);

   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid*)(2 * sizeof(float)));

   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindVertexArray(0);

   GLuint surfaceTexture;
   glCreateTextures(GL_TEXTURE_2D, 1, &surfaceTexture);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, surfaceTexture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window.Width, window.Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   //
   //GUI end
   //

   input::SetWindowFocus(window);

   input::InputManager::BindAxis(input::Key::W, [](const float value)
                                                { g_MainCamera.Move(graphics::CameraMoveType::MoveForward, value, g_DeltaTime); });
   input::InputManager::BindAxis(input::Key::S, [](const float value)
                                                { g_MainCamera.Move(graphics::CameraMoveType::MoveBackward, value, g_DeltaTime); });
   input::InputManager::BindAxis(input::Key::D, [](const float value)
                                                { g_MainCamera.Move(graphics::CameraMoveType::MoveRight, value, g_DeltaTime); });
   input::InputManager::BindAxis(input::Key::A, [](const float value)
                                                { g_MainCamera.Move(graphics::CameraMoveType::MoveLeft, value, g_DeltaTime); });
   input::InputManager::BindAxis(input::Key::E, [](const float value)
                                                { g_MainCamera.Move(graphics::CameraMoveType::MoveUp, value, g_DeltaTime); });
   input::InputManager::BindAxis(input::Key::Q, [](const float value)
                                                { g_MainCamera.Move(graphics::CameraMoveType::MoveDown, value, g_DeltaTime); });

   while (!window.ShouldClose())
   {
      deltaTimer.Reset();

      window.BeginFrame();

      input::InputManager::Poll();
      
      graphics::ForwardRender::Update(scene, g_DeltaTime);


      //GUI render

      guiShader.Use();

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, surfaceTexture);

      uint32_t w, h;
      void* pixels;
      g_GC.GetRenderingInfo(w, h, pixels);

      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

      guiShader.SetInt("Texture", 0);

      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 12);
      glBindVertexArray(0);

      window.EndFrame();

      g_DeltaTime = 1.0f / deltaTimer.GetElaspedTime();
   }

   glfwTerminate();
   return 0;
}