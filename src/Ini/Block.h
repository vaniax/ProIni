#pragma once

#include <variant>
#include <memory>
#include <string>
#include <stack>

#include "Property.h"

namespace Ini {
    class IIni;
}

namespace Ini {

class IIni;

class Block : public std::enable_shared_from_this<Block>
{
public:

    virtual ~Block() {}

    virtual void SetProperty(const std::string& name, const Property& value) = 0;

    virtual void Register(IIni* ini, const std::string& name="") = 0;

    template<class T>
    static inline std::shared_ptr<T> Create() {
        return std::make_shared<T>();
    }

    const std::string& GetBlockName() { return name_; }

    bool HasParent() { return parent_ != nullptr; }
    Block *GetParent() { return parent_; }

    std::string GetPath() {
        if (this->HasParent()) {
            return this->GetParent()->GetPath() + "." + name_;
        }

        return name_;
    }

    template<class T>
    T *CreateChild(const std::string& name)
    {
        std::shared_ptr<T> nt(std::make_shared<T>());
        static_cast<T*>(nt.get())->parent_ = this;
        name_ = name;
        sub_blocks_[name] = nt;
        return nt.get();
    }

    std::map<std::string, std::shared_ptr<Block>> sub_blocks_;

private:
    Block *parent_ = nullptr;
    std::string name_;
};

}