#pragma once

#include <Ini/Property.h>

namespace GFrost {
    class Vector2;
    class Vector3;
    class Vector4;
    class Rect;
    class Quaternion;
    class Color;
}

template<>
PROINI_API GFrost::Vector2 Ini::Property::Get() const;

template<>
PROINI_API GFrost::Vector3 Ini::Property::Get() const;

template<>
PROINI_API GFrost::Vector4 Ini::Property::Get() const;

template<>
PROINI_API GFrost::Rect Ini::Property::Get() const;

template<>
PROINI_API GFrost::Quaternion Ini::Property::Get() const;

template<>
PROINI_API GFrost::Color Ini::Property::Get() const;


