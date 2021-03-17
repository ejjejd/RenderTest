#pragma once
#include <memory>
#include <functional>

#include "graphics/api/devices/gl-device.h"
#include "window/window.h"
#include "window/input/input-manager.h"
#include "gui/gui-manager.h"
#include "jobs/job-system.h"
#include "utils/timer.h"

namespace app
{
   std::shared_ptr<graphics::GraphicsDevice> g_GraphicsDevice;

   std::shared_ptr<app::Window> g_Window;


   float g_DeltaTime;
   float g_FPS;

   inline void CreateEngineApp()
   {
      //Resolving all subsystems dependencies
      
      g_GraphicsDevice = std::make_shared<graphics::OpenglGraphicsDevice>(); //TODO make the device selectable from the config if the will be one more

      g_Window = std::make_shared<app::Window>(g_GraphicsDevice);

      
      //Independent subsystems setup's
      
      core::JobSystem::Setup();
   }

   inline void RunEngineApp(const std::function<void()>& userTickFunc)
   {
     utils::Timer frameTimer;

     while(!g_Window->ShouldClose())
     {
        frameTimer.Reset();


        g_Window->BeginFrame();


        userTickFunc();


        g_Window->EndFrame();


        g_DeltaTime = 1.0f / frameTimer.GetElapsedTime();
        g_FPS = 1000.0f / frameTimer.GetElapsedTime();
     }
   }
}