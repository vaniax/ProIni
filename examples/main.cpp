
#include <INIFile.h>
#include "INIFileProperty.h"
#include "MappedImage.h"

#include <assert.h>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif

using namespace GFrost;

#ifdef _MSC_VER
int WinMain() {
#else
int main(int argc, char** argv) {
#endif

    INIFile *ini_f = new INIFile();
    ini_f->SetWorkDirectory("./Ini/");

    ini_f->RegisrerBlock("MappedImage", Ini::Block::Create<MappedImage>);
//    ini_f->RegisrerBlock("GameObject", Ini::Block::Create<GameObject>);
//    ini_f->RegisrerBlock("MusicTrack", Ini::Block::Create<MusicTrack>);

    Ini::Block* bl0 = Ini::Block::Create<MappedImage>().get();

    std::cout << bl0->GetBlockName() << std::endl;

    MappedImage mi0;
    assert(mi0 == MappedImage::DEFAULT);

    auto position = mi0.GetPos();
    std::cout << "mi0 pos: {x:" << position.x_ << ", y:" << position.y_ << "}" << std::endl;

    ini_f->Load("MappedImageTest.ini");

    auto img0 = ini_f->GetBlock<MappedImage>("LoadPageHuge");
    std::cout << "LoadPageHuge " << img0->GetTextureName() << std::endl;

    auto ing1 = ini_f->GetBlock<MappedImage>("SCCAttack2");
//    std::cout <<"SCCAttack " << ing1->GetTextureName()<< std::endl;
//
//    Rect ns = ing1->GetCoords();
//    std::cout << "Coords: " << ns.ToString()<< std::endl;
//    std::cout << "TextureHeight: " << std::to_string(ing1->GetTextureHeight())<< std::endl;


    return 0;
}
