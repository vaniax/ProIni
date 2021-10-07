#pragma once

#include <map>
#include <memory>
#include <stack>
#include <vector>
#include <functional>
#include <sstream>

#include "Token.h"

namespace Ini
{
    class Block;
    class ParsingContext;
//    class IStream;
    class FileSystem;
    class IIni;
    class TokenStream;

    using BlockConstruct = std::function<std::shared_ptr<Block>(void)>;

    class PROINI_API Parser
    {
        enum State
        {
            NO_BLOCK,
            IN_BLOCK,
            BLOCK_OPEN,
            PROP_SET,
        };

    public:

        Parser(IIni* ini);
        void Parse(ParsingContext* context);
        void RegisrerBlock(const std::string& type, const BlockConstruct& block) {
            m_constructors[type] = block; }
    private:
        void BlockOpen(TokenStream* t, State& s);
        void InBlock(TokenStream* t, State& s);
        void PropSet(TokenStream* t, State& s);

        const std::string PopString();
        const Token PopToken();

        bool CreateBlock(const std::string& type, const std::string& name);
        void CreateProperty(State& state);

    private:
        unsigned m_lvl = 0;

        std::shared_ptr<Block> m_block;
        std::string m_blockName;
        std::stack<Token> m_arguments;
        std::string sub_name;
        std::map<std::string, std::vector<Token>> m_sub_arguments_;
        std::map<const std::string, BlockConstruct> m_constructors;
        IIni* ini_;

    };
}