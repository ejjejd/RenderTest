#pragma once
#include <unordered_map>

#include "GLFW/glfw3.h" 
#include "window/input/input-events.h"

using namespace ::input;

namespace input
{
   inline std::unordered_map<uint32_t, InputEvent> GlfwKeyMap =
   {
      { GLFW_KEY_SPACE, InputEvent::Space },
      { GLFW_KEY_APOSTROPHE, InputEvent::Apostrophe },
      { GLFW_KEY_COMMA, InputEvent::Comma },
      { GLFW_KEY_MINUS, InputEvent::Minus },
      { GLFW_KEY_PERIOD, InputEvent::Period },
      { GLFW_KEY_SLASH, InputEvent::Slash },
      { GLFW_KEY_0, InputEvent::_0 },
      { GLFW_KEY_1, InputEvent::_1 },
      { GLFW_KEY_2, InputEvent::_2 },
      { GLFW_KEY_3, InputEvent::_3 },
      { GLFW_KEY_4, InputEvent::_4 },
      { GLFW_KEY_5, InputEvent::_5 },
      { GLFW_KEY_6, InputEvent::_6 },
      { GLFW_KEY_7, InputEvent::_7 },
      { GLFW_KEY_8, InputEvent::_8 },
      { GLFW_KEY_9, InputEvent::_9 },
      { GLFW_KEY_SEMICOLON, InputEvent::Semicolon },
      { GLFW_KEY_EQUAL, InputEvent::Equal },
      { GLFW_KEY_A, InputEvent::A },
      { GLFW_KEY_B, InputEvent::B },
      { GLFW_KEY_C, InputEvent::C },
      { GLFW_KEY_D, InputEvent::D },
      { GLFW_KEY_E, InputEvent::E },
      { GLFW_KEY_F, InputEvent::F },
      { GLFW_KEY_G, InputEvent::G },
      { GLFW_KEY_H, InputEvent::H },
      { GLFW_KEY_I, InputEvent::I },
      { GLFW_KEY_J, InputEvent::J },
      { GLFW_KEY_K, InputEvent::K },
      { GLFW_KEY_L, InputEvent::L },
      { GLFW_KEY_M, InputEvent::M },
      { GLFW_KEY_N, InputEvent::N },
      { GLFW_KEY_O, InputEvent::O },
      { GLFW_KEY_P, InputEvent::P },
      { GLFW_KEY_Q, InputEvent::Q },
      { GLFW_KEY_R, InputEvent::R },
      { GLFW_KEY_S, InputEvent::S },
      { GLFW_KEY_T, InputEvent::T },
      { GLFW_KEY_U, InputEvent::U },
      { GLFW_KEY_V, InputEvent::V },
      { GLFW_KEY_W, InputEvent::W },
      { GLFW_KEY_X, InputEvent::X },
      { GLFW_KEY_Y, InputEvent::Y },
      { GLFW_KEY_Z, InputEvent::Z },
      { GLFW_KEY_LEFT_BRACKET, InputEvent::LeftBracket },
      { GLFW_KEY_BACKSLASH, InputEvent::BackSlash },
      { GLFW_KEY_RIGHT_BRACKET, InputEvent::RightBracket },
      { GLFW_KEY_GRAVE_ACCENT, InputEvent::GraveAccent },
      { GLFW_KEY_ESCAPE, InputEvent::Escape },
      { GLFW_KEY_ENTER, InputEvent::Enter },
      { GLFW_KEY_TAB, InputEvent::Tab },
      { GLFW_KEY_BACKSPACE, InputEvent::Backspace },
      { GLFW_KEY_INSERT, InputEvent::Insert },
      { GLFW_KEY_DELETE, InputEvent::Delete },
      { GLFW_KEY_RIGHT, InputEvent::Right },
      { GLFW_KEY_LEFT, InputEvent::Left },
      { GLFW_KEY_DOWN, InputEvent::Down },
      { GLFW_KEY_UP, InputEvent::Up },
      { GLFW_KEY_PAGE_UP, InputEvent::Page_up },
      { GLFW_KEY_PAGE_DOWN, InputEvent::Page_down },
      { GLFW_KEY_HOME, InputEvent::Home },
      { GLFW_KEY_END, InputEvent::End },
      { GLFW_KEY_CAPS_LOCK, InputEvent::Caps_lock },
      { GLFW_KEY_SCROLL_LOCK, InputEvent::Scroll_lock },
      { GLFW_KEY_NUM_LOCK, InputEvent::Num_lock },
      { GLFW_KEY_PRINT_SCREEN, InputEvent::Print_screen },
      { GLFW_KEY_PAUSE, InputEvent::Pause },
      { GLFW_KEY_F1, InputEvent::F1 },
      { GLFW_KEY_F2, InputEvent::F2 },
      { GLFW_KEY_F3, InputEvent::F3 },
      { GLFW_KEY_F4, InputEvent::F4 },
      { GLFW_KEY_F5, InputEvent::F5 },
      { GLFW_KEY_F6, InputEvent::F6 },
      { GLFW_KEY_F7, InputEvent::F7 },
      { GLFW_KEY_F8, InputEvent::F8 },
      { GLFW_KEY_F9, InputEvent::F9 },
      { GLFW_KEY_F10, InputEvent::F10 },
      { GLFW_KEY_F11, InputEvent::F11 },
      { GLFW_KEY_F12, InputEvent::F12 },
      { GLFW_KEY_F13, InputEvent::F13 },
      { GLFW_KEY_F14, InputEvent::F14 },
      { GLFW_KEY_F15, InputEvent::F15 },
      { GLFW_KEY_F16, InputEvent::F16 },
      { GLFW_KEY_F17, InputEvent::F17 },
      { GLFW_KEY_F18, InputEvent::F18 },
      { GLFW_KEY_F19, InputEvent::F19 },
      { GLFW_KEY_F20, InputEvent::F20 },
      { GLFW_KEY_F21, InputEvent::F21 },
      { GLFW_KEY_F22, InputEvent::F22 },
      { GLFW_KEY_F23, InputEvent::F23 },
      { GLFW_KEY_F24, InputEvent::F24 },
      { GLFW_KEY_F25, InputEvent::F25 },
      { GLFW_KEY_KP_0, InputEvent::KeyPad_0 },
      { GLFW_KEY_KP_1, InputEvent::KeyPad_1 },
      { GLFW_KEY_KP_2, InputEvent::KeyPad_2 },
      { GLFW_KEY_KP_3, InputEvent::KeyPad_3 },
      { GLFW_KEY_KP_4, InputEvent::KeyPad_4 },
      { GLFW_KEY_KP_5, InputEvent::KeyPad_5 },
      { GLFW_KEY_KP_6, InputEvent::KeyPad_6 },
      { GLFW_KEY_KP_7, InputEvent::KeyPad_7 },
      { GLFW_KEY_KP_8, InputEvent::KeyPad_8 },
      { GLFW_KEY_KP_9, InputEvent::KeyPad_9 },
      { GLFW_KEY_KP_DECIMAL, InputEvent::KeyPad_Decimal },
      { GLFW_KEY_KP_DIVIDE, InputEvent::KeyPad_Divide },
      { GLFW_KEY_KP_MULTIPLY, InputEvent::KeyPad_Multiply },
      { GLFW_KEY_KP_SUBTRACT, InputEvent::KeyPad_Subtract },
      { GLFW_KEY_KP_ADD, InputEvent::KeyPad_Add },
      { GLFW_KEY_KP_ENTER, InputEvent::KeyPad_Enter },
      { GLFW_KEY_KP_EQUAL, InputEvent::KeyPad_Equal },
      { GLFW_KEY_LEFT_SHIFT, InputEvent::LeftShift },
      { GLFW_KEY_LEFT_CONTROL, InputEvent::LeftControl },
      { GLFW_KEY_LEFT_ALT, InputEvent::LeftAlt },
      { GLFW_KEY_RIGHT_SHIFT, InputEvent::RightShift },
      { GLFW_KEY_RIGHT_CONTROL, InputEvent::RightControl },
      { GLFW_KEY_RIGHT_ALT, InputEvent::RightAlt },
   };

   inline std::unordered_map<uint32_t, InputEvent> GlfwMouseButtonMap =
   {
      { GLFW_MOUSE_BUTTON_1, InputEvent::M_Left  },
      { GLFW_MOUSE_BUTTON_2, InputEvent::M_Right },
      { GLFW_MOUSE_BUTTON_3, InputEvent::M_Middle },
      { GLFW_MOUSE_BUTTON_4, InputEvent::M_4 },
      { GLFW_MOUSE_BUTTON_5, InputEvent::M_5 },
      { GLFW_MOUSE_BUTTON_6, InputEvent::M_6 },
      { GLFW_MOUSE_BUTTON_7, InputEvent::M_7 },
      { GLFW_MOUSE_BUTTON_8, InputEvent::M_8 }
   };

   inline InputEventState GlfwTranslateState(const uint32_t state)
   {
      if (state == GLFW_PRESS || state == GLFW_REPEAT) //TODO Later make for 'repeat' separate state
         return InputEventState::Pressed;
      else
         return InputEventState::Released;
   }
}