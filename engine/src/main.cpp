#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "scene/scene.h"

#include "math/matrices/matrix4x4.h"
#include "scene/renders/forward-render.h"

#include "asset-manager/obj-loader.h"
#include "asset-manager/image-loader.h"
#include "asset-manager/text-loader.h"
#include "asset-manager/asset-manager.h"

#include "math/math_utils.h"

#include <string.h>
#include <Windows.h>

#include "gui.h"
#include "bgl/shader.h"

#include "debug/log/log.h"
#include "bgl/debug/callback.h"

char g_KeyStates[1024];
Camera* g_MainCamera;

float g_DeltaTime = 1.0f / 60.0f;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
   if (action == GLFW_PRESS)
      g_KeyStates[key] = 1;
   if (action == GLFW_RELEASE)
      g_KeyStates[key] = 0;
}

void CursorCallback(GLFWwindow* window, double posX, double posY)
{
   RotateCamera(g_MainCamera, posX, posY, g_DeltaTime);
}

void InputHandle()
{
   if (g_KeyStates[GLFW_KEY_W])
      MoveCamera(g_MainCamera, CameraMoveForward, g_DeltaTime);
   if(g_KeyStates[GLFW_KEY_S])
      MoveCamera(g_MainCamera, CameraMoveBackward, g_DeltaTime);
   if(g_KeyStates[GLFW_KEY_D])
      MoveCamera(g_MainCamera, CameraMoveRight, g_DeltaTime);
   if (g_KeyStates[GLFW_KEY_A])
      MoveCamera(g_MainCamera, CameraMoveLeft, g_DeltaTime);
   if (g_KeyStates[GLFW_KEY_E])
      MoveCamera(g_MainCamera, CameraMoveUp, g_DeltaTime);
   if (g_KeyStates[GLFW_KEY_Q])
      MoveCamera(g_MainCamera, CameraMoveDown, g_DeltaTime);
}

int main()
{
   if (!glfwInit())
      return -1;

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

   GLFWwindow* window = glfwCreateWindow(1280, 720, "RenderTest", NULL, NULL);
   if (!window)
      return -1;

   glfwMakeContextCurrent(window);

   if (glewInit() != GLEW_OK)
      return -1;

   const GLubyte* vendorInfo   = glGetString(GL_VENDOR);
   const GLubyte* rendererInfo = glGetString(GL_RENDERER);
   const GLubyte* versionInfo  = glGetString(GL_VERSION);
   const GLubyte* glslInfo     = glGetString(GL_SHADING_LANGUAGE_VERSION);

   WD_LOG_MESSAGE("%s %s", vendorInfo, rendererInfo);
   WD_LOG_MESSAGE("Opengl %s", versionInfo);
   WD_LOG_MESSAGE("GLSL %s\n", glslInfo);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBlendEquation(GL_FUNC_ADD);

   glEnable(GL_DEPTH_TEST);

   glfwSetKeyCallback(window, KeyCallback);
   glfwSetCursorPosCallback(window, CursorCallback);

   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   glEnable(GL_DEBUG_OUTPUT);
   glDebugMessageCallback(MessageCallbackOGL, 0);
   
   g_MainCamera = CreateCamera(vec3(0.0f, 0.0f, 10.0f), PI / 4, 1.7f, 5.0f);

   Scene* scene = CreateScene(g_MainCamera);

   assets::AssetManager assetManager;
   assets::AssetRef pistolAssetRef = assetManager.RequireAsssetRef("res/meshes/pistol/pistol.obj");
   assets::AssetRef cubeAssetRef = assetManager.RequireAsssetRef("res/meshes/cube.obj");
  
   assetManager.Load();

   uint8_t floorMaterial = AddObjectMaterial(scene, CreateMaterial(vec3(0.0f, 0.2f, 0.2f), vec3(1.0f, 1.0f, 1.0f), 16.0f, svec3(0.0f)));
   uint8_t pistolMaterial = AddObjectMaterial(scene, CreateMaterial(vec3(0.7f, 0.2f, 0.2f), vec3(0.5f, 0.5f, 0.5f), 8.0f, svec3(0.0f)));

   AddRenderObject(scene, CreateRenderObject(CreateMesh(pistolAssetRef), CreateTranslateMatrix(vec3(0.0f, 0.0f, 0.0f)), pistolMaterial));

   Matrix4 cubeTransform = CreateTranslateMatrix(vec3(0.0f, -2.0f, 0.0f ));
   cubeTransform = multiply_matrix(cubeTransform, CreateScaleMatrix(vec3(5.0f, 1.0f, 5.0f)));
   AddRenderObject(scene, CreateRenderObject(CreateMesh(cubeAssetRef), cubeTransform, floorMaterial));
   
   AddPointLight(scene, CreatePointLight(vec3(1.2f, 1.0f, 2.0f), vec3(1.0f, 1.0f, 1.0f), 0.09f, 1.0f, 0.032f));
   AddSpotlight(scene, CreateSpotlight(vec3(0.0f, 10.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), PI / 12, PI / 15));

   InitializeForwardRender();

   LARGE_INTEGER freq;
   QueryPerformanceFrequency(&freq);


   int width = 1280;
   int height = 720;

   InitializeGUI("file:///res/gui/web/main.html", width, height);

   GLuint surfaceTexture;
   glCreateTextures(GL_TEXTURE_2D, 1, &surfaceTexture);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, surfaceTexture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   const char* vertexShaderSrc =
      "#version 330 core\n"

      "layout(location = 0) in vec2 pos;\n"
      "layout(location = 1) in vec2 uv;\n"

      "out vec2 Uv;\n"

      "void main()\n"
      "{\n"
         "Uv = uv;\n"
         "gl_Position = vec4(pos, 0.0f, 1.0f);\n"
      "}";

   const char* fragmentShaderSrc =
      "#version 330 core\n"
      
      "out vec4 Color;\n"
      
      "in vec2 Uv;\n"

      "uniform sampler2D Texture;\n"

      "void main()\n"
      "{\n"
         "Color = texture(Texture, Uv);\n"
      "}";

   GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderSrc);
   GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

   GLuint shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram);

   float vertices[] =
   {
      - 1.0f, -1.0f, 0.0f, 1.0f,
      -1.0f, 1.0f, 0.0f, 0.0f,
      1.0f, 1.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 1.0f, 0.0f,
      1.0f, -1.0f, 1.0f, 1.0f
      - 1.0f, -1.0f, 0.0f, 1.0f,
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

   {
      int maxX, maxY, maxZ, count;

      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &maxX);
      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &maxY);
      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &maxZ);
      glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &count);

      LOG_WARNING("MAX WORK GROUP ITEMS X %d", maxX);
      LOG_WARNING("MAX WORK GROUP ITEMS Y %d", maxY);
      LOG_WARNING("MAX WORK GROUP ITEMS Z %d", maxZ);
      LOG_WARNING("MAX WORK GROUP INVOCATIONS %d", count);
   }

   {
      int maxX, maxY, maxZ, count;

      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &maxX);
      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &maxY);
      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &maxZ);

      LOG_WARNING("MAX WORK GROUP COUNT X %d", maxX);
      LOG_WARNING("MAX WORK GROUP COUNT Y %d", maxY);
      LOG_WARNING("MAX WORK GROUP COUNT Z %d", maxZ);
   }

   while (!glfwWindowShouldClose(window))
   {
      LARGE_INTEGER startFrameTicks;
      QueryPerformanceCounter(&startFrameTicks);


      glfwPollEvents();
      InputHandle();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      UpdateForwardRender(scene, g_DeltaTime);

      glUseProgram(shaderProgram);

      SetShaderInt(shaderProgram, "Texture", 0);

      SetTestCounterJS(1.0f / g_DeltaTime);

      void* pixels;
      GetWebWindowInfo(&width, &height, &pixels);

      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 12);
      glBindVertexArray(0);

      glfwSwapBuffers(window);

      LARGE_INTEGER endFrameTicks;
      QueryPerformanceCounter(&endFrameTicks);

      g_DeltaTime = endFrameTicks.QuadPart - startFrameTicks.QuadPart;
      g_DeltaTime /= freq.QuadPart / 1000.0f;
      g_DeltaTime = 1.0f / g_DeltaTime;
   }

   glfwTerminate();
   return 0;
}