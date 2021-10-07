#pragma once

#include <string>
#include <experimental/string_view>
#include <memory>
#include <functional>
#include "Context.hpp"

namespace Ini {

class IIni;
//class FileSystem;

class PROINI_API Lexer
{
private:
    enum LexerMode
    {
        NORMAL,
        INTEGER,
        FLOAT,
        STRING,
        UNICODE,
        PERCENT,
        SINGLE_CHAR,
    };
public:
    explicit Lexer(IIni* ini);

    ParsingContext* Lex(const std::string& source, const std::string& filename);
private:
    std::vector<Token> Tokenize(const std::string& , ParsingContext* context = nullptr,bool tokenstream=true);
    Token CreateToken(const std::string&  line, int& pos, std::vector<Token>& toks, ParsingContext* context=nullptr,bool tokenstream=true);
    void AddEol(TokenStream* stream,int col);
    bool CheckEol(const std::string&  line, TokenStream* stream);
    void SkipWhitespaces(const std::string&  str, int& pos);
    void Preprocess(const std::string&  str, int& pos, ParsingContext* context);

    std::string ReadQuoted(const std::string&  str, int & pos);
    std::string ReadCmd(const std::string&  str, int& pos);

    void MergeTokens(std::vector<Token>& tokens, int max);

    void TokenFunc(std::function<Token(Token&, Token&)> func, const std::string&  str, int pos, ParsingContext* context);

    Token TokenOperation(const Token & a, const Token & b, std::function<double(const double, const double)> f);

    std::string GetFileName() {return m_path; }
private:
    IIni* m_ini;
//    FileSystem& m_fs;
    std::string m_basepath;
    std::string m_path;
    int m_line;
    TokenStream* tocken_stream_;
};


}
