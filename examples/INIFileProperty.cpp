
#include "INIFileProperty.h"

#include <Ini/StringUtils.hpp>

//#include <GFrost/Core/StringUtils.h>

#ifdef GFROST_CONTEXT
#  include <GFrost/Math/Vector2.h>
#  include <GFrost/Math/Vector3.h>
#  include <GFrost/Math/Vector4.h>
#  include <GFrost/Math/Rect.h>
#  include <GFrost/Math/Quaternion.h>
#  include <GFrost/Math/Color.h>
#else
#  include "Types.hpp"
#endif

#include <iostream>

using namespace GFrost;

template<>
Vector2 Ini::Property::Get() const
{
    if (!CheckArgs() || m_args.size() != 2)
        return Vector2();
    Vector2 result = Vector2();

    for (auto &it : m_args)
    {
        auto val = SplitAtFirst<':'>(it.Value.str_);
        if (val[0] == "X")
            result.x_ = std::stof(val[1]);
        else if (val[0] == "Y")
            result.y_ = std::stof(val[1]);
    }
    return result;
}

//template<>
//IntVector2 Ini::Property::Get() const
//{
//    IntVector2 result;
//
//    if (!CheckArgs() || m_args.size() != 2)
//        return result;
//
//    for (auto &it : m_args)
//    {
//        auto val = SplitAtFirst<':'>(it.Value.str_);
//        if (val[0] == "X")
//            result.x_ = std::stoi(val[1]);
//        else if (val[0] == "Y")
//            result.y_ = std::stoi(val[1]);
//    }
//    return result;
//}

template<>
Vector3 Ini::Property::Get() const
{
    if (!CheckArgs() || m_args.size() != 3)
        return Vector3();
    Vector3 result = Vector3();

    for (auto &it : m_args)
    {
        auto val = SplitAtFirst<':'>(it.Value.str_);
        if (val[0] == "X")
            result.x_ = std::stof(val[1]);
        else if (val[0] == "Y")
            result.y_ = std::stof(val[1]);
        else if (val[0] == "Z")
            result.z_ = std::stof(val[1]);
    }
    return result;
}

template<>
Vector4 Ini::Property::Get() const
{
    if (!CheckArgs() || m_args.size() != 4)
        return GFrost::Vector4();
    GFrost::Vector4 result = GFrost::Vector4();

    for (auto &it : m_args)
    {
        auto val = SplitAtFirst<':'>(it.Value.str_);

        if (val[0] == "X")
            result.x_ = std::stof(val[1]);
        else if (val[0] == "Y")
            result.y_ = std::stof(val[1]);
        else if (val[0] == "Z")
            result.z_ = std::stof(val[1]);
        else if (val[0] == "W")
            result.w_ = std::stof(val[1]);
    }
    return result;
}

template<>
Quaternion Ini::Property::Get() const
{
    if (!CheckArgs() || m_args.size() != 4)
        return GFrost::Quaternion();
    GFrost::Quaternion result = GFrost::Quaternion();
    // "X:10, Y:5, Z:10, W:11"
    for (auto &it : m_args)
    {
        auto val = SplitAtFirst<':'>(it.Value.str_);

        if (val[0] == "X")
            result.x_ = std::stof(val[1]);
        else if (val[0] == "Y")
            result.y_ = std::stof(val[1]);
        else if (val[0] == "Z")
            result.z_ = std::stof(val[1]);
        else if (val[0] == "W")
            result.w_ = std::stof(val[1]);
    }
    return result;
}

template<>
Rect Ini::Property::Get() const
{
    if (m_args.size() != 4)
        return Rect();
    Vector4 result = Vector4();

//    Initialize<Rect>();

    for (auto &it : m_args)
    {
        printf("-- %s\n", +it.Value.str_.c_str());
        auto val = SplitAtFirst<':'>(it.Value.str_);

        if (val[0] == "Left")
            result.x_ = std::stof(val[1]);
        else if (val[0] == "Top")
            result.y_ = std::stof(val[1]);
        else if (val[0] == "Right")
            result.z_ = std::stof(val[1]);
        else if (val[0] == "Bottom")
            result.w_ = std::stof(val[1]);

//        auto hash = ResctCont[String(val[0].c_str())];
//        switch (hash) {
//            case RectStr::SLeft: {
//                result.x_ = std::stof(val[1]);
//                break;
//            }
//            case RectStr::STop: {
//                result.y_ = std::stof(val[1]);
//                break;
//            }
//            case RectStr::SRight: {
//                result.z_ = std::stof(val[1]);
//                break;
//            }
//            case RectStr::SBottom: {
//                result.w_ = std::stof(val[1]);
//                break;
//            }
//
//            default: break;
//        }
    }
    return result;
}

template<>
Color Ini::Property::Get() const
{
    if (!CheckArgs() || m_args.size() != 4)
        return Color();
    Color result = Color();

    for (auto &it : m_args)
    {
        auto val = SplitAtFirst<':'>(it.Value.str_);

        if (val[0] == "R")
            result.r_ = std::stof(val[1]);
        else if (val[0] == "G")
            result.g_ = std::stof(val[1]);
        else if (val[0] == "B")
            result.b_ = std::stof(val[1]);
        else if (val[0] == "A")
            result.a_ = std::stof(val[1]);
    }
    return result;
}

template<>
std::vector<std::string> Ini::Property::Get() const
{
    std::vector<std::string> result;

    for (auto &it : m_args)
        result.push_back(it.Value.str_);
    return result;
}