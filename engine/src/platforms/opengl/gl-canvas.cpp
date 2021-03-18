#include "gl-canvas.h"

#include "debug/gassert.h"
#include "glfw/glfw-input-map.h"
#include "events/input-events.h"

namespace graphics
{
   namespace gl
   { 
      namespace input
      {
         static event::Subject KeySubject;
         static event::Subject MouseButtonSubject;
         static event::Subject CursorSubject;
         static event::Subject MouseScrollSubject;


         inline void KeyCallback(GLFWwindow* w, int key, int sc, int action, int mods)
         {
            KeySubject.Invoke(event::KeyEvent(GlfwInputMap.at(key), GlfwTranslateState(action)));
         }

         inline void MouseButtonCallback(GLFWwindow* w, int button, int action, int mods)
         {
            MouseButtonSubject.Invoke(event::MouseButtonEvent(GlfwInputMap.at(button), GlfwTranslateState(action)));
         }

         inline void CursorCallback(GLFWwindow* w, double x, double y)
         {
            CursorSubject.Invoke(event::MouseCursorPosEvent(x, y));
         }

         inline void ScrollCallback(GLFWwindow* w, double x, double y)
         {
            MouseScrollSubject.Invoke(event::MouseScrollEvent(y));
         }
      }

         CanvasGL::CanvasGL(const uint16_t width, const uint16_t height, const std::string_view& title)
            : Width(width), Height(height)
         {
            GASSERT(glfwInit(), "Error in window creation");

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

            GlfwWindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

            GASSERT(GlfwWindow, "Error in window creation");

            glfwMakeContextCurrent(GlfwWindow);

            GASSERT(glewInit() == GLEW_OK, "Error in window creation");


            //Setup input

            glfwSetKeyCallback(GlfwWindow, input::KeyCallback);

            glfwSetMouseButtonCallback(GlfwWindow, input::MouseButtonCallback);

            glfwSetCursorPosCallback(GlfwWindow, input::CursorCallback);

            glfwSetScrollCallback(GlfwWindow, input::ScrollCallback);


            //
            //This should be move out from here
            //

            const GLubyte* vendorInfo = glGetString(GL_VENDOR);
            const GLubyte* rendererInfo = glGetString(GL_RENDERER);
            const GLubyte* versionInfo = glGetString(GL_VERSION);
            const GLubyte* glslInfo = glGetString(GL_SHADING_LANGUAGE_VERSION);

            WD_LOG_MESSAGE("%s %s", vendorInfo, rendererInfo);
            WD_LOG_MESSAGE("Opengl %s", versionInfo);
            WD_LOG_MESSAGE("GLSL %s\n", glslInfo);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);

            glEnable(GL_DEPTH_TEST);

            //glEnable(GL_DEBUG_OUTPUT);
            //glDebugMessageCallback(MessageCallbackOGL, 0);

            glfwSetInputMode(GlfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
         }
   }
}
