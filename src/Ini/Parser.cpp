#include "Parser.h"
//#include "../../core/exception.hpp"

#include "Ini.h"
#include "Token.h"
#include "Block.h"
#include "Context.hpp"

#include <string>
#include <algorithm>
#include <iostream>
#include <chrono>

Ini::Parser::Parser(IIni* ini) : ini_(ini)
{
}

void Ini::Parser::Parse(ParsingContext* context)
{
    auto stream = context->GetTokenStream();
    stream->Start();

    State s = NO_BLOCK;



    for (Token t = stream->GetToken();
            t.Type != Token::EndOfFile;
            t = stream->GetToken())
    {
        switch (s)
        {
            case BLOCK_OPEN:
            case NO_BLOCK:

                BlockOpen(stream, s);
                break;
            case IN_BLOCK:
                InBlock(stream, s);
                break;
            case PROP_SET:
                PropSet(stream, s);
                break;
        }
    }
}

void Ini::Parser::BlockOpen(TokenStream* stream, State& s)
{
    Token t = stream->Current();

    switch (s)
    {
        case NO_BLOCK:
            if (t.Type == Token::StringLiteral)
            {
                m_arguments.push(t);
                s = BLOCK_OPEN;
                m_lvl++;
            }
            break;
        case BLOCK_OPEN:
            if (t.Type == Token::StringLiteral) // Block name
            {
                m_arguments.push(t);
            }

            if (t.Type == Token::EndOfLine)
            {
                std::string type, name;
                if (m_arguments.size() == 2)
                    name = PopString();

                type = PopString();

                if (!name.empty() && !CreateBlock(type, name)) {
                    printf("ini error [%u]: Unknown block: %s\n",
                            t.Position.Line, sub_name.c_str());
                }
                //todo Create SubBlock

                s = IN_BLOCK;
            }
            break;

            default:
                int hh = 0;
                break;
    }
}

void Ini::Parser::InBlock(TokenStream* stream, State &s)
{
    Token t = stream->Current();
    std::string str;

    switch (t.Type)
    {
        case Token::TokenType::StringLiteral:
            str = t.Value.str_;
            if (str == "End")
            {
                if (m_block)
                {
                    //TODO: Add block to library
                    int TT = 0;
                    //clear
                    s = NO_BLOCK;
                    m_block = nullptr;
                }

                m_lvl--;
            }
            else {
                //PROPERTY
                if (stream->Peek().Type == Token::Equals)
                {
                    s = PROP_SET;
                    stream->GetToken();
                    m_arguments.push(t);
                }
                //NUGGET or undefined Block
                else {
                    m_lvl++;
                    sub_name = t.Value.str_;
                }
            }
            break;
    }

}

void Ini::Parser::PropSet(TokenStream* stream, State & s)
{
    Token t = stream->Current();

    if (t.Type == Token::EndOfLine)
        CreateProperty(s);
    else
        m_arguments.push(t);
}

const std::string Ini::Parser::PopString()
{
    std::string result = m_arguments.top().Value.str_;
    m_arguments.pop();
    return result;
}

const Ini::Token Ini::Parser::PopToken()
{
    Token result = m_arguments.top();
    m_arguments.pop();
    return result;
}

bool Ini::Parser::CreateBlock(const std::string &type, const std::string &name)
{
    auto it = m_constructors.find(type);

    if (it == m_constructors.end())
        return false;

    m_block = it->second();
    m_block->Register(ini_, name);

    return true;
}

void Ini::Parser::CreateProperty(State & state)
{
    std::vector<Token> arguments;
    std::string name;

    while (m_arguments.size() > 1)
        arguments.push_back(PopToken());
    name = PopString();

    if (m_lvl == 2) {
        if (m_block->sub_blocks_[sub_name] == nullptr) {
            printf("Error load sub block: %s\n",
                    m_block->GetPath().c_str()); //sub_name.c_str());
        }

        m_block->sub_blocks_[sub_name]->SetProperty(name, arguments);
        state = IN_BLOCK;
        return;
    }

    if (m_block) {
        m_block->SetProperty(name, arguments);
        state = IN_BLOCK;
    } else {
        state = IN_BLOCK;
    }
}