#pragma once

#include "Types.hpp"

#include <Ini/Block.h>
#include <INIFile.h>

class MappedImage : public Ini::Block
{
public:
    MappedImage();

    const std::string& GetTextureName() { return m_texture; }
//        inline GFrost::Vector4 GetTextureSize() { return glm::ivec2(m_texWidth, m_texHeight); }
    inline GFrost::Rect GetCoords() { return m_coords; }

    inline GFrost::Vector2 GetPos() { return pos_; }
    inline int GetTextureWidth() { return m_texWidth; }
    inline int GetTextureHeight() { return m_texHeight; }

    // Inherited via Block
    void SetProperty(const std::string& name, const Ini::Property& value) override;

    void Register(Ini::IIni* ini, const  std::string& name) override;

    bool operator == (MappedImage& rvl) {
        return m_coords == rvl.m_coords
            && m_status == rvl.m_status
            && m_texHeight == rvl.m_texHeight
            && m_texWidth == m_texWidth
            && m_texture == rvl.m_texture
            && pos_ == rvl.pos_;
    }

    static MappedImage DEFAULT;
private:
    std::string m_texture;
    int m_texWidth{};
    int m_texHeight{};
    GFrost::Rect m_coords;
    std::string m_status;

    GFrost::Vector2 pos_;
};

template<>
inline std::shared_ptr<MappedImage> INIFile::GetBlock(std::string_view name)
{
    auto blocks = blocks_["MappedImage"];

    auto it = blocks.find(std::string(name));

    if (it != blocks.end())
        return std::dynamic_pointer_cast<MappedImage>(it->second);

    printf("WARNING: Can't find object MappedImage: %s", name);
    return std::make_shared<MappedImage>(MappedImage::DEFAULT);
}

//template<>
//inline MappedImage* Ini::Property::Get() const
//{
//    auto block = new MappedImage();
//
//
//    for (auto& it : m_args) {
//        printf("MappedImage::property = %s\n", it.Value.str_.c_str());
//    }
//
////    auto it = blocks_["GameObject"].find(std::string(name));
////
////    if(it != blocks_["GameObject"].end())
////        return std::dynamic_pointer_cast<Block>(it->second);
////
//    return block;
//}

