#pragma once

#include "Context.hpp"
#include <functional>

namespace Ini {

class Block;

class IIni {
public:
    virtual void Register(const std::string &nspace, const  std::string &name, std::shared_ptr<Block> block) = 0;

    virtual Ini::ParsingContext *GetContext(const std::string &path, bool load = true) = 0;
};


}