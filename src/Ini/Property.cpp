#include "Property.h"

#include "StringUtils.hpp"

#include <string>

Ini::Property::Property(std::vector<Token>& args) : m_args(args)
{
}

bool Ini::Property::GetBoolean() const
{
    if (!CheckArgs())
        return false;

    bool result = false;

    std::string str = m_args.front().Value.str_;

    if (str == "Yes")
        result = true;
    else if (str == "No")
        result = false;
    else
        printf("Can't create Boolean Property: %s\n", str.c_str());

    return result;
}

std::string Ini::Property::GetString() const
{
    if (!CheckArgs())
        return "";

    auto& token = m_args.front();

    if (token.Type != Token::StringLiteral)
    {
        if (token.Type == Token::IntegerLiteral)
            return std::to_string(token.Value.int_);
        else {
            if (token.Value.str_.length())
                return token.Value.str_;

            printf("Cannot create String property!\n");
            return "";
        }
    }
    else
        return token.Value.str_;
}

std::string Ini::Property::GetUnicode() const
{
    if (!CheckArgs())
        return "";

    if (m_args.front().Type != Token::UnicodeLiteral)
    {
        printf("Expected Unicode Literal!\n");
        return "";
    }

    return m_args.front().Value.str_;
}

long long Ini::Property::GetInteger() const
{
    if (!CheckArgs())
        return 0;

    auto& token = m_args.front();

    //cast into an int
    if (token.Type == Token::FloatLiteral)
        return (long long)token.Value.float_;

    return token.Value.int_;
}

double Ini::Property::GetDouble() const
{
    if (!CheckArgs())
        return 0;

    auto& token = m_args.front();

    //cast into a double
    if (token.Type == Token::IntegerLiteral)
        return (double)token.Value.int_;

    return token.Value.float_;
}

bool Ini::Property::CheckArgs() const
{
    if (m_args.empty())
    {
        printf("Missing arguments for property\n");
        return false;
    }

    return true;
}