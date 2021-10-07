#include "INIFile.h"
#include <iostream>


#include "Ini/Parser.h"
#include "Ini/Lexer.h"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

using namespace GFrost;

Ini::ParsingContext* INIFile::loadFile(const std::string & filename) {

    std::ifstream ifs(work_dir_ + filename);
    //std::shared_ptr<File> file(GetSubsystem<ResourceCache>()->GetFile(path.c_str()));
    std::string str((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());

    ifs.close();

//        Ini::Parser* p = new Ini::Parser(this);
    std::shared_ptr<Ini::Lexer> lexer(new Ini::Lexer(this));

    auto* ctx = lexer->Lex(str, work_dir_ + filename);
    parser_->Parse(ctx);
    auto jt = m_files.emplace(work_dir_ + filename, ctx);

    return jt.first->second;
}

INIFile::INIFile()
{
    parser_ = new Ini::Parser(this);
}

INIFile::~INIFile()
{
    delete parser_;
}

Ini::ParsingContext *INIFile::GetContext(const std::string &path, bool load)
{
    auto it = m_files.find(path);

    if (it != m_files.end()) {
        return it->second;
    }
    else if (load) {
        return loadFile(path);
    }
    else
        return nullptr;

}

bool INIFile::Load(const std::string &filename) {
    return loadFile(filename) != nullptr;
}

void INIFile::RegisrerBlock(const std::string& type, const Ini::BlockConstruct &block) {
    parser_->RegisrerBlock(type, block);
}
