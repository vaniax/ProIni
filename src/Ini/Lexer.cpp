#include "Lexer.h"

#include <sstream>
#include <algorithm>
#include <codecvt>
#include <locale>
#include <chrono>

#include <iostream>

#include "StringUtils.hpp"
#include "Ini.h"
#include "Token.h"

using namespace std::placeholders;

std::map<std::string, std::string> Ini::ParsingContext::m_globalMacros;

Ini::ParsingContext*
        Ini::Lexer::Lex(const std::string &source, const std::string& path)
{
    auto* context = new ParsingContext();
    m_path = path;
    m_basepath = path.substr(0, path.find_last_of('/') + 1);
    m_line = 0;

    tocken_stream_ = new TokenStream(path); //TODO need cleanup
    context->SetTokens(tocken_stream_);

    std::string line;
    std::string line_view;
    size_t pos;
    std::stringstream sourcestream;
    sourcestream << source;

    auto start = std::chrono::high_resolution_clock::now();
    auto end = start;

    while (std::getline(sourcestream, line))
    {
        //remove all \r
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

        //replace tabs with spaces
        std::replace(line.begin(), line.end(), '\t', ' ');

        line_view = line;

        if (CheckEol(line_view, tocken_stream_))
            continue;

        //remove comments
        pos = line_view.find(';');
        if (pos != std::string::npos)
        {
            line_view = line_view.substr(0, pos);
        }

        if (CheckEol(line_view, tocken_stream_))
            continue;

        pos = line_view.find("//");
        if (pos != std::string::npos)
        {
            line_view = line_view.substr(0, pos);
        }

        if (CheckEol(line_view, tocken_stream_))
            continue;

        //remove trailing whitespaces
        line_view = Trim(line_view);

        if (CheckEol(line_view, tocken_stream_))
            continue;


        tocken_stream_->InsertTokens(Tokenize(line_view, context));

        AddEol(tocken_stream_,pos);
    }

    end = std::chrono::high_resolution_clock::now();
    tocken_stream_->AddToken(Token(Token::EndOfFile));

    return context;
}

Ini::Lexer::Lexer(IIni* ini) : m_ini(ini)
{
//    pctx_ = new ParsingContext();
}

std::vector<Ini::Token> Ini::Lexer::Tokenize(const std::string&  line, ParsingContext* context,bool tokenstream)
{
    std::vector<Token> tokens;
    int pos = 0;
    Skip<' '>(line, pos);

    while (pos < line.size())
    {
        Token tok = CreateToken(line, pos,tokens, context, tokenstream);
        if(tok.Type!=Token::Unknown)
            tokens.push_back(tok);

        Skip<' '>(line, pos);
    }

    return tokens;
}

Ini::Token Ini::Lexer::CreateToken(const std::string&  line, int & pos, std::vector<Token>& toks,
        ParsingContext* context, bool tokenstream)
{
    bool parse = true;
    Token t(Token::Unknown);
    LexerMode mode = LexerMode::NORMAL;
    Token::TokenType type = Token::Unknown;
    std::string content;
    int col = pos;
    char c = 0;

    while (parse)
    {
        c = line[pos];

        if (mode == LexerMode::UNICODE)
        {
            if (c == '"')
                parse = false;
            else {
                content += c;
            }
        }

        if (mode == LexerMode::NORMAL)
        {
            if (isdigit(c))
                mode = LexerMode::INTEGER;
            else if (isalpha(c))
                mode = LexerMode::STRING;
            else if (c == '"')
                mode = LexerMode::UNICODE;
            else
                mode = LexerMode::SINGLE_CHAR;
        }

        if (   mode == LexerMode::INTEGER
            || mode == LexerMode::STRING
            || mode == LexerMode::FLOAT)
        {
            if ((mode == LexerMode::INTEGER || mode == LexerMode::FLOAT) && c == '%') {
                mode = LexerMode::PERCENT;
                parse = false;
            }

            //we started with an int, that now turns out as a float
            if (mode == INTEGER && c == '.') {
                mode = FLOAT;
            }

            //we started with an int, that now turns out as a string
            if (mode == LexerMode::INTEGER && (isalpha(c) || c == '_'))
            {
                mode = LexerMode::STRING;
            }

            if (isspace(c))
                parse = false;
            else
                content += c;
        }

        else if (mode == LexerMode::SINGLE_CHAR)
        {
            switch (c)
            {
                case '=':
                    type = Token::Equals;
                    break;
                case '+':
                    type = Token::Plus;
                    break;
                case '-':
                    type = Token::Minus;
                    break;
                case '%':
                    break;
                case '#':
                    //PREPROCESSOR FUN
                    Preprocess(line, pos,context);
                    break;
                default:
                {
                    int a = 0;
                }
                    break;
            }

            parse = false;
        }

        ++pos;

        if (pos == line.size())
            parse = false;
    }

    switch (mode)
    {
        case STRING:
            if (context && context->CheckMacro(content))
                if (tokenstream)
                {
                    auto macro_toks = Tokenize(content, context);
                    toks.insert(toks.end(), macro_toks.begin(), macro_toks.end());
                }
                else
                {
                    auto toks = Tokenize(content, context, false);
                    MergeTokens(toks, 1);
                    t = toks.front();
                }

            else
                t = Token(content);
            break;
        case INTEGER:
            t = Token(std::stoll(content));
            break;
        case FLOAT:
            t = Token(std::stof(content));
            break;
        case PERCENT:
            t = Token(std::stod(content),true);
            break;
        case UNICODE:
            t = Token(content,true);
            break;
        case SINGLE_CHAR:
            t = Token(type);
            break;
        default:
            break;
    }

    t.Position.Column = col;
    t.Position.Line = m_line;
    return t;
}

void Ini::Lexer::AddEol(TokenStream* stream,int col)
{
    bool add = (stream->GetTokens().size()==0) ? true :  stream->GetBack().Type != Token::EndOfLine;

    if (add)
    {
        Token t(Token::EndOfLine);
        t.Position.Line = m_line;
        t.Position.Column = col;
        stream->AddToken(t);
    }
    ++m_line;
}

bool Ini::Lexer::CheckEol(const std::string&  line, TokenStream* stream)
{
    if (line.size() == 0)
    {
        AddEol(stream, 0);
        return true;
    }

    return false;
}

void Ini::Lexer::SkipWhitespaces(const std::string&  str, int& pos)
{
    while (isspace(str[pos]))
        ++pos;
}

void Ini::Lexer::Preprocess(const std::string&  str, int& pos, ParsingContext* context)
{
    Skip<' '>(str, pos);

    auto cmd = std::string(ReadCmd(str, pos));

    Skip<' '>(str, pos);

    if (cmd == "#define")
    {
        const std::string&  name = ReadTill<' '>(str, pos);
        if (name == "TUTORIAL_WOTR_VOLUME")
        {
            int a = 0;
        }

        Skip<' '>(str, pos);
        const std::string&  value = str.substr(pos, str.size());
        pos = str.size();
        context->AddMacro(std::string(name), std::string(value));
    }
    else if (cmd=="#include")
    {
        std::string include = ReadQuoted(str, pos);
        std::string inc_path = m_basepath + std::string(include);
//        inc_path = fs::weakly_canonical(fs::path(inc_path)).string();
        std::replace(inc_path.begin(), inc_path.end(), '\\', '/');
//        std::transform(inc_path.begin(), inc_path.end(), inc_path.begin(), ::tolower);
        context->Include(m_ini->GetContext(inc_path));
    }
    else
    {
        std::function<Token(Token&, Token&)> op;
        bool known = true;

        if (cmd == "#MULTIPLY")
            op = std::bind(&Lexer::TokenOperation, this, _1, _2, [](const double a, const double b) {return a*b; });
        else if (cmd == "#ADD")
            op = std::bind(&Lexer::TokenOperation, this, _1, _2, [](const double a, const double b) {return a+b; });
        else if (cmd == "#DIVIDE")
            op = std::bind(&Lexer::TokenOperation, this, _1, _2, [](const double a, const double b) {return a/b; });
        else if (cmd == "#SUBTRACT")
            op = std::bind(&Lexer::TokenOperation, this, _1, _2, [](const double a, const double b) {return a-b; });
        else
        {
            known = false;
            printf("INI:%s[%ul] UNKNOWN PREPROCESSOR FUNCTION: %s",
                    m_path.c_str(), m_line+1, cmd.c_str());
        }

        if(known)
            TokenFunc(op, str, pos, context);
    }
}

void Ini::Lexer::MergeTokens(std::vector<Token>& tokens, int max)
{
    if (tokens.size() > max)
    {
        auto it = std::begin(tokens);
        while (it != std::end(tokens)) {
            // Do some stuff
            if (it->Type == Token::Minus)
            {
                auto after = std::next(it);
                if (after->Type == Token::IntegerLiteral)
                    it->Value.int_ -= after->Value.int_;
                else if (after->Type == Token::FloatLiteral)
                    it->Value.float_ -= after->Value.float_;

                it = tokens.erase(it);
            }
            else if (it->Type == Token::Plus)
            {
                auto after = std::next(it);
                if (after->Type == Token::IntegerLiteral)
                    it->Value.int_ += after->Value.int_;
                else if (after->Type == Token::FloatLiteral)
                    it->Value.float_ += after->Value.float_;

                it = tokens.erase(it);
            }

            std::advance(it, 1);
        }
    }
}

void Ini::Lexer::TokenFunc(std::function<Token(Token&, Token&)> func, const std::string&  str, int pos, ParsingContext* context)
{
    Token tok1, tok2;
    const std::string&  content = str.substr(pos + 1, str.find_first_of(')')-(pos+1));
    std::vector<Token> toks = Tokenize(content, context, false);

    MergeTokens(toks, 2);

    context->GetTokenStream()->AddToken(func(toks[0], toks[1]));
}

Ini::Token Ini::Lexer::TokenOperation(const Token & a, const Token & b, std::function<double(double, double)> f)
{
    bool integerTok = ((a.Type == Token::IntegerLiteral) && (b.Type == Token::IntegerLiteral)) ? true : false;

    double first = (a.Type == Token::IntegerLiteral)
                    ? a.Value.int_
                    : a.Value.float_;

    double second = (b.Type == Token::IntegerLiteral)
                    ? b.Value.int_
                    : b.Value.float_;

    double result = f(first, second);
    return (integerTok) ? static_cast<long long>(result) : result;
}

std::string  Ini::Lexer::ReadQuoted(const std::string&  str, int & pos)
{
    while (str[pos] != '\"' && pos < str.size()) ++pos;
    ++pos;

    int first = pos;

    while (str[pos] != '\"' && pos < str.size())
        ++pos;

    int last = pos;

    return str.substr(first, last - first);
}

std::string Ini::Lexer::ReadCmd(const std::string& str, int &pos)
{
    char c = str[pos];
    int first = pos;

    while ((c != ' ') && (c !='(') && (pos < str.size()))
        c = str[++pos];

    int last = pos;
    return str.substr(first,last-first);
}