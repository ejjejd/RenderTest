#pragma once
#include <unordered_map>

#include "Ultralight/Ultralight.h"
#include "window/input/maps/input-events.h"

namespace input
{
   static std::unordered_map<InputEvent, uint32_t> UltralightInputMap =
   {
      { InputEvent::M_Left,   ultralight::MouseEvent::kButton_Left},
      { InputEvent::M_Middle, ultralight::MouseEvent::kButton_Middle},
      { InputEvent::M_Right,  ultralight::MouseEvent::kButton_Right}
   };
}