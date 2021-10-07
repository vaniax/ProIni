#pragma once

#include <string>
#include <map>
#include "Ini/Ini.h"


namespace Ini {
    class Block;
    class ParsingContext;
    class Parser;
    class Lexer;

    using BlockConstruct = std::function<std::shared_ptr<Block>(void)>;
}


namespace GFrost {
    class FileSystem;
}

class PROINI_API INIFile : public Ini::IIni
{

public:
    explicit INIFile();

    virtual ~INIFile();

    inline void Register(const std::string &nspace, const std::string &name,
            std::shared_ptr<Ini::Block> block) override
    {
        blocks_[nspace].emplace(name, block);
    }

    template<class T = Ini::Block>
    std::shared_ptr<T> GetBlock(const std::string& name = "");
//    {
//
//        auto block = blocks_[T::GetBlockName()];
//        auto it = block.find(std::string(name));
//
//        if (it != block.end())
//            return std::dynamic_pointer_cast<T>(it->second);
//
//        return nullptr;
//    }

    inline std::vector<std::string>& GetGlobalIncludes() { return m_globalIncludes; }

    Ini::ParsingContext* GetContext(const std::string& path, bool load = true) override;

    void RegisrerBlock(const std::string& type, const Ini::BlockConstruct &block);

    /// Register element with default constructor
#ifdef GFOST_CONTEXT
    template<class T = GFrost::Object>
    inline void RegisrerBlock(std::string_view type)
    {
        auto ctx = context_;
        this->RegisrerBlock(type, [ctx](void) {
            return std::shared_ptr<Ini::Block>(new T(ctx));
        });
    }
#else
    template<class T>
    inline void RegisrerBlock(std::string_view type)
    {
        this->RegisrerBlock(type, [](void) {
            return std::shared_ptr<Ini::Block>(new T());
        });
    }
#endif

    bool Load(const std::string &filename);

     void SetWorkDirectory(const std::string &path) {
         work_dir_ = path;
     }

    const std::string& GetWorkDirectory() {
        return work_dir_;
    }

private:

    Ini::ParsingContext* loadFile(const std::string & filename);

    std::vector<std::string> m_globalIncludes;
    std::map<const std::string, Ini::ParsingContext*> m_files;
//    std::map<const std::string, std::shared_ptr<Ini::Block>> m_mappedImages;

    std::map<std::string, std::map<std::string, std::shared_ptr<Ini::Block>>> blocks_;

    Ini::Parser* parser_;
    std::string work_dir_ = "./";
};


