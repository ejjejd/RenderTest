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

#include "gui.h"

#include "jobs/job-system.h"

#include "application/window/window-wrapper.h"

#include "input/input-manager.h"

#include "events/subject.h"

#include "application/application.h"

#include "graphics/shaders/compute-shader-adapter.h"

#include "utils/read-from-file.h"

class MainApp : public app::BaseApplication
{
public:
   graphics::Camera MainCamera;
   assets::AssetManager AssetManager;
   core::Scene Scene;

   std::unique_ptr<graphics::ComputeShader> testCS;
   std::unique_ptr<graphics::ShaderPipeline> testSP;

   gl::VertexArray  Vao;
   gl::VertexBuffer Vbo;

   gl::Texture2D csTexture;

   std::unique_ptr<graphics::ShaderPipeline> sphereSP;
   gl::VertexArray sphereVao;

   struct Tile
   {
      float x;
      float y;

      gl::VertexArray Vao;
   };
   std::vector<Tile> Tiles;

   std::unique_ptr<graphics::ShaderPipeline> TileShader;

   void OnStartup() override
   {
      uintptr_t callbackArgs = reinterpret_cast<uintptr_t>(this);

      InputManager->AddAxisMapping("MoveForward", { { input::InputEvent::W, 1.0f },
                                                   { input::InputEvent::S, -1.0f } });
      InputManager->AddAxisMapping("MoveRight", { { input::InputEvent::D, 1.0f },
                                                 { input::InputEvent::A, -1.0f} });
      InputManager->AddAxisMapping("MoveUp", { { input::InputEvent::E, 1.0f },
                                              { input::InputEvent::Q, -1.0f } });

      InputManager->BindAxis("MoveForward", [](const float value, const uintptr_t args)
         {
            MainApp* app = reinterpret_cast<MainApp*>(args);
            app->MainCamera.Move(graphics::CameraMoveType::MoveForward, value, app->DeltaTime);
         }, callbackArgs);

      InputManager->BindAxis("MoveRight", [](const float value, const uintptr_t args)
         {
            MainApp* app = reinterpret_cast<MainApp*>(args);
            app->MainCamera.Move(graphics::CameraMoveType::MoveRight, value, app->DeltaTime);
         }, callbackArgs);
      InputManager->BindAxis("MoveUp", [](const float value, const uintptr_t args)
         {
            MainApp* app = reinterpret_cast<MainApp*>(args);
            app->MainCamera.Move(graphics::CameraMoveType::MoveUp, value, app->DeltaTime);
         }, callbackArgs);

      InputManager->GetInputWrapper()->MouseCursorPosEventSubj.AddObserver([](event::BaseEvent& e, uintptr_t args)
         {
            event::MouseCursorPosEvent mouseE = event::CastEvent<event::MouseCursorPosEvent>(e);
            MainApp* app = reinterpret_cast<MainApp*>(args);
            app->MainCamera.Rotate(mouseE.PosX, mouseE.PosY, app->DeltaTime);
         }, callbackArgs);

      assets::AssetRef pistolAssetRef = AssetManager.RequireAsssetRef("res/meshes/pistol/pistol.obj");
      assets::AssetRef cubeAssetRef = AssetManager.RequireAsssetRef("res/meshes/cube.obj");

      assets::AssetRef pistolTextureRef = AssetManager.RequireAsssetRef("res/meshes/pistol/textures/handgun_C.jpg");

      //for(size_t i = 0; i < 15; ++i)
      //  AssetManager.RequireAsssetId("res/meshes/pistol/pistol.obj");

      {
         utils::Timer assetTimer(true);
         AssetManager.Load();
         LOG_WARNING("Assets loading time: %f ms", assetTimer.GetElaspedTime());
      }

      MainCamera = graphics::Camera(math::Vector3(0.0f, 0.0f, 10.0f), math::Pi / 4, 1.7f, 5.0f);

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

      Scene.SetCamera(&MainCamera);

      size_t floorMaterial = Scene.AddMaterial(floorM);
      size_t pistolMaterial = Scene.AddMaterial(pistolM);

      Scene.AddRenderObject({ graphics::Mesh(pistolAssetRef), CreateTranslateMatrix(math::Vector3(0.0f, 0.0f, 0.0f)), pistolMaterial });

      math::Matrix4 cubeTransform = CreateTranslateMatrix(math::Vector3(0.0f, -2.0f, 0.0f));
      cubeTransform = cubeTransform * CreateScaleMatrix(math::Vector3(5.0f, 1.0f, 5.0f));
      Scene.AddRenderObject({ graphics::Mesh(cubeAssetRef), cubeTransform, floorMaterial });

      Scene.AddLight(graphics::PointLight(math::Vector3(1.2f, 1.0f, 2.0f), math::Vector3(1.0f, 1.0f, 1.0f), 0.09f, 1.0f, 0.032f));

      for (int i = 0; i < 1; ++i)
         Scene.AddLight(graphics::Spotlight(math::Vector3(0, 10.0f, 0), math::Vector3(0.0f, -1.0f, 0.0f), math::Vector3(1.0f, 1.0f, 1.0f), math::Pi / 12, math::Pi / 15));

      graphics::ForwardRender::Initialize();


      gl::TextureParams params;
      params.MagFilter = GL_NEAREST;
      params.MinFilter = GL_NEAREST;
      params.WrapS = GL_CLAMP_TO_EDGE;
      params.WrapT = GL_CLAMP_TO_EDGE;

      csTexture = gl::CreateTexture(Window->GetWidth(), Window->GetHeight(),
                                                    GL_RGBA32F, GL_RGBA, GL_FLOAT,
                                                    params);

      float vertices[] =
      {
         -1.0f, -1.0f, 0.0f, 1.0f,
         -1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 1.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 1.0f,
         -1.0f, -1.0f, 0.0f, 1.0f,
      };

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

      testSP = std::make_unique<graphics::ShaderPipeline>();

      testSP->Add(graphics::ShaderType::Vertex, vertexShaderSrc);
      testSP->Add(graphics::ShaderType::Fragment, fragmentShaderSrc);

      testSP->Compile();

      Vbo = gl::CreateVertexBuffer(sizeof(vertices), vertices);

      Vao = gl::CreateVertexArray();

      GLuint b = gl::AddBufferVertexArray(Vao, Vbo, 4 * sizeof(float));

      gl::AddAttribFormatVertexArray(Vao, 0, b, 2, GL_FLOAT, GL_FALSE, 0);
      gl::AddAttribFormatVertexArray(Vao, 1, b, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));

      glBindImageTexture(0, csTexture.BindId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

      testCS = std::make_unique<graphics::ComputeShader>(utils::ReadFromFile("res/shaders/compute/ray-tracing.cs"));

      testCS->Dispatch(Window->GetWidth(), Window->GetHeight());
      glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

      //Calculate tiles

      constexpr uint8_t tilesX = 8;
      constexpr uint8_t tilesY = 8;

      float ar = (float)Window->GetHeight() / Window->GetWidth();
      const float tileSizeX = 2.0f / tilesX; //Calculated in NDC
      const float tileSizeY = 2.0f / tilesY;

      {
         const char* vertexShaderSrc = R"(
            #version 460 core
      
            layout(location = 0) in vec2 pos;

            flat out vec2 Position;
      
            void main()
            {
               Position = pos;
               gl_Position = vec4(pos, 0.0f, 1.0f);
            })";

         const char* fragmentShaderSrc = R"(
            #version 460 core
         
            out vec4 Color;

            flat in vec2 Position;
      
            void main()
            {
               Color = vec4((Position.xy + 0.5) / 2, 0.2f, 1.0f);
            })";

            TileShader = std::make_unique<graphics::ShaderPipeline>();

            TileShader->Add(graphics::ShaderType::Vertex, vertexShaderSrc);
            TileShader->Add(graphics::ShaderType::Fragment, fragmentShaderSrc);

            TileShader->Compile();
      }


      for (float y = 1.0f; y > -1.0f; y -= tileSizeY)
      {
         for (float x = -1.0f; x < 1.0f; x += tileSizeX)
         {
            float sX = tileSizeX;
            float sY = tileSizeY * (1.0f / ar);

            float fY = y * (1.0f / ar);

            float res[]
            {
               x, fY,
               x, fY - sY,
               x + sX, fY,
               x + sX, fY,
               x + sX, fY - sY,
               x, fY - sY,
            };

            gl::VertexBuffer vbo = gl::CreateVertexBuffer(sizeof(res), res);

            Tile newTile;

            newTile.x = x;
            newTile.y = y;

            newTile.Vao = gl::CreateVertexArray();

            GLuint b = gl::AddBufferVertexArray(newTile.Vao, vbo, 2 * sizeof(float));
            gl::AddAttribFormatVertexArray(newTile.Vao, 0, b, 2, GL_FLOAT, GL_FALSE, 0);

            Tiles.push_back(newTile);
         }


         {
            const char* vertexShaderSrc = R"(
            #version 460 core
      
            layout(location = 0) in vec3 pos;

            uniform mat4 view = mat4(1.0f);
            uniform mat4 model = mat4(1.0f);
            uniform mat4 projection = mat4(1.0f);

            void main()
            {
               gl_Position = projection * model * view * vec4(pos, 1.0f);
            })";

            const char* fragmentShaderSrc = R"(
            #version 460 core
         
            out vec4 Color;

            void main()
            {
               Color = vec4(1.0f, 0.2f, 0.2f, 1.0f);
            })";


            sphereSP = std::make_unique<graphics::ShaderPipeline>();

            sphereSP->Add(graphics::ShaderType::Vertex, vertexShaderSrc);
            sphereSP->Add(graphics::ShaderType::Fragment, fragmentShaderSrc);

            sphereSP->Compile();
         }

         math::Vector3 center(0.0f, 1.0f, 0.0f);
         float radius = 5.0f;
         const size_t segments = 64;

         float pos[segments * 3 + 6];
         
         pos[0] = 0.0f;
         pos[1] = 0.0f;
         pos[2] = 0.0f;

         for (size_t i = 3; i <= segments * 3 + 3; i += 3)
         {
            pos[i] = radius * cos((math::Pi2 / segments) * (i / 3));
            pos[i + 1] = radius * sin((math::Pi2 / segments) * (i / 3));
            pos[i + 2] = 0.0f;
         }

         //pos[49] = cos((math::Pi2 / segments) * 1);
         //pos[50] = 0.0f;
         //pos[49] = cos((math::Pi2 / segments) * 1);

         gl::VertexBuffer vbo = gl::CreateVertexBuffer(sizeof(pos), pos);
         GLuint b = gl::AddBufferVertexArray(sphereVao, vbo, 3 * sizeof(float));
         gl::AddAttribFormatVertexArray(sphereVao, 0, b, 3, GL_FLOAT, GL_FALSE, 0);
      }

   }

   void OnTick() override
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      graphics::ForwardRender::Update(Scene, DeltaTime);


      TileShader->Use();

      //for (auto t : Tiles)
      //{
      //   glBindVertexArray(t.Vao.BindId);
      //   glDrawArrays(GL_TRIANGLES, 0, 12);
      //}

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      sphereSP->Use();
      sphereSP->SetFloats("projection", MainCamera.GetCameraProjection());
      sphereSP->SetFloats("view", MainCamera.GetCameraViewMatrix());

      glBindVertexArray(sphereVao.BindId);
      glDrawArrays(GL_TRIANGLE_FAN, 0, 256);
   }
};

app::BaseApplication* app::CreateApplication()
{
   return new MainApp();
}