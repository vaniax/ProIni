#include "Token.h"

namespace Ini {

Token::Token(TokenType type)
{
    Type = type;
}

Token::Token(const std::string& str, bool unicode)
{
    Value.str_ = str;

    if (unicode)
        Type = UnicodeLiteral;
    else
        Type = StringLiteral;
}

Token::Token(long long num)
{
    Value.int_ = num;
    Type = IntegerLiteral;
}

Token::Token(double num, bool percent)
{
    Value.float_ = num;

    if (percent)
        Type = PercentLiteral;
    else
        Type = FloatLiteral;
}


}