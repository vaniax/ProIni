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

template<> [[nodiscard]]
GFrost::Vector2 Ini::Property::Get() const;

template<> [[nodiscard]]
GFrost::Vector3 Ini::Property::Get() const;

template<> [[nodiscard]]
GFrost::Vector4 Ini::Property::Get() const;

template<> [[nodiscard]]
GFrost::Rect Ini::Property::Get() const;

template<> [[nodiscard]]
GFrost::Quaternion Ini::Property::Get() const;

template<> [[nodiscard]]
GFrost::Color Ini::Property::Get() const;


