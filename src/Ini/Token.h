#pragma once
#include <string>

#include <ProIni.h>

namespace Ini {

struct PROINI_API Token
{
    enum TokenType
    {
        Unknown,
        Identifier,
        Equals,
        IntegerLiteral,
        FloatLiteral,
        PercentLiteral,
        StringLiteral,
        UnicodeLiteral,
        Hashtag,
        Colon,
        Comma,
        Plus,
        Minus,
        EndOfLine,
        EndOfFile,
    };

    struct TokenPosition
    {
        int Line;
        int Column;
    };

    Token(TokenType type=Unknown);
    Token(const std::string& str, bool unicode = false);
    Token(long long num);
    Token(double num, bool percent = false);

    TokenPosition Position;
    TokenType Type;

    struct value {
        std::string str_;
        long long int_;
        double float_;
    }Value;

    //std::tuple<std::string,long long, double> ;
};

}