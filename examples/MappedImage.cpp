#include "MappedImage.h"

#include "INIFileProperty.h"

#include <cxxabi.h>

MappedImage MappedImage::DEFAULT;

MappedImage::MappedImage() { *this = DEFAULT; }

void MappedImage::SetProperty(const std::string& name, const Ini::Property& value)
{
    if (name == "Texture")
        m_texture = value.GetString().c_str();
    else if (name == "TextureWidth")
        m_texWidth = value.GetInteger();
    else if (name == "TextureHeight")
        m_texHeight = value.GetInteger();
    else if (name == "Coords")
        m_coords = value.Get<GFrost::Rect>();
    else if (name == "Status")
        m_status = value.GetString();
    else if (name == "TextureHeight")
        m_texHeight = value.GetInteger();
    else if (name == "TextureWidth")
        m_texWidth = value.GetInteger();

}

void MappedImage::Register(Ini::IIni* ini, const std::string& name)
{
    ini->Register("MappedImage", name, shared_from_this());
}