#pragma once


#include <vector>
#include <map>
#include "Token.h"

namespace Ini {

class PROINI_API Property
{
public:
    Property(std::vector<Token>& args);
    bool GetBoolean() const;
    std::string GetString() const;
    std::string GetUnicode() const;
    long long GetInteger() const;
    double GetDouble() const;

    template<class T> T Get() const;

    bool CheckArgs() const;
private:
    std::vector<Token> m_args;
    std::map<std::string, Token> sub_obj;
};

//template<>
//inline bool Property::Get(const std::string& name)
//{
//    //    if (!CheckArgs() || m_args.size() != 4)
////        return glm::ivec4();
////    glm::ivec4 result = glm::ivec4();
//////Left:0 Top : 0 Right : 1023 Bottom : 768
////
////    for (int i = 0; i < m_args.size(); i++)
////    {
////        std::array<const std::string& , 2> val = SplitAtFirst<':'>(std::get<std::string>(m_args[i].Value));
////
////        if (val[0] == "Left")
////        {
////            result.x = std::stoi(std::string(val[1]));
////        }
////        else if (val[0] == "Top")
////        {
////            result.y = std::stoi(std::string(val[1]));
////        }
////        else if (val[0] == "Right")
////        {
////            result.z = std::stoi(std::string(val[1]));
////        }
////        else if (val[0] == "Bottom")
////        {
////            result.w = std::stoi(std::string(val[1]));
////        }
////    }
////    return result;
//
////    auto it = blocks_["MusicTrack"].find(std::string(name));
////
////    if(it != blocks_["MusicTrack"].end())
////        return std::dynamic_pointer_cast<MusicTrack>(it->second);
//
//    return false;
//}

}