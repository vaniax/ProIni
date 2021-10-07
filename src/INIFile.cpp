#include "INIFile.h"
#include <iostream>


#include "Ini/Parser.h"
#include "Ini/Lexer.h"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

using namespace GFrost;

Ini::ParsingContext* INIFile::loadFile(const std::string & filename) {

    parser_ = new Ini::Parser(this);
    lexer_ = new Ini::Lexer(this);

    std::ifstream ifs(filename.c_str());
    //std::shared_ptr<File> file(GetSubsystem<ResourceCache>()->GetFile(path.c_str()));
    std::string str((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());

    Ini::Lexer* l = new Ini::Lexer(this);

    auto* ctx = l->Lex(str, filename);
    parser_->Parse(ctx);
    auto jt = m_files.emplace(filename, ctx);

    return jt.first->second;
}

INIFile::INIFile()
{
    parser_ = new Ini::Parser(this);
    lexer_ = new Ini::Lexer(this);
//    fs_ = context_->GetSubsystem<FileSystem>();
//
//    ResourceCache *cache = context_->GetSubsystem<ResourceCache>();
//
//    int num = 0;
//
//    m_globalIncludes = { "data/ini/gamedata.ini" };

//    auto& macros = Ini::ParsingContext::GetGlobalMacros();
//    Ini::Parser* p = new Ini::Parser(this);
//    Ini::Lexer* l = new Ini::Lexer(this);

//    for (const auto& path : m_globalIncludes)
//    {
//        auto& context = l->Lex(fs.GetStream(path), path); // read & parse
//        p->Parse(context);
//
//        auto& m = context->GetMacros();
//        macros.insert(m.begin(), m.end());
//
//        m_files.emplace(path, context);
//    }
//
//    //clear macro cache
//    for (const auto& f : m_files)
//    {
//        auto& context = f.second;
//
//        context->GetMacros().clear();
//        context->GetTokenStream()->Clear();
//    }
//
//    macros.clear();
//
//    std::cout << num << std::endl;
}

INIFile::~INIFile()
{
}

Ini::ParsingContext *INIFile::GetContext(const std::string &path, bool load)
{
    auto it = m_files.find(path);

    if (it != m_files.end())
    {
        return it->second;
    }
    else if (load)
    {
        std::ifstream ifs(work_dir_ + path);
        //std::shared_ptr<File> file(GetSubsystem<ResourceCache>()->GetFile(path.c_str()));
        std::string str((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());

        ifs.close();

//        Ini::Parser* p = new Ini::Parser(this);
        Ini::Lexer* l = new Ini::Lexer(this);

        auto* ctx = l->Lex(str, work_dir_ + path);
        parser_->Parse(ctx);
        auto jt = m_files.emplace(work_dir_ + path, ctx);

        return jt.first->second;
    }
    else
        return nullptr;

}

bool INIFile::Load(const std::string &filename) {

    std::ifstream ifs(work_dir_ + filename);
    //std::shared_ptr<File> file(GetSubsystem<ResourceCache>()->GetFile(path.c_str()));
    std::string str((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());

    auto ctx = lexer_->Lex(str, work_dir_ + filename);
    parser_->Parse(ctx);

    return false;
}

void INIFile::RegisrerBlock(const std::string& type, const Ini::BlockConstruct &block) {
    parser_->RegisrerBlock(type, block);
}
