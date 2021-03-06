#pragma once
#include <cstdint>

namespace graphics
{
   enum class Feature : uint8_t 
   {
      Blend,
      Depth
   };

   enum class Attachment : uint8_t
   {
      Color,
      Depth
   };

   enum class InternalFormat : uint8_t
   {
      RGB8,
      RGBA8,
      RGB16,
      RGBA16,
      Depth16,
      Depth24,
      Depth32
   };

   enum class Format : uint8_t
   {
      RGB,
      RGBA,
      BGR,
      BGRA,
      Depth
   };

   enum class Type : uint8_t
   {
      Ubyte,
      Uint,
      Float
   };

   enum class BlendFunc : uint8_t
   {
      Add,
      Subtract
   };

   enum class BlendValue : uint8_t
   {
      One,
      SrcAlpha,
      OneMinusSrcAlpha
   };

   enum class Face : uint8_t
   {
      Front,
      Back,
      Both
   };
}
