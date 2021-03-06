#pragma once
#include <memory>

#include "platforms/win64/win64-dev.h"

namespace utils
{
   class Timer
   {
   private:
      struct NativeInfo;
      std::unique_ptr<NativeInfo> Native;
   public:
      Timer(const bool reset = false);
      ~Timer();

      void Reset();

      float GetElapsedTime() const;
   };
}