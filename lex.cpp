
#include "lex.h"

Token::Token(Type t, const std::string le, const std::string  to, int l, int c)
{
    type = t;
    linha = l;
    coluna = c;
    lexema = le;
    token = to;
    if (le.empty())
    {
        throw std::runtime_error("TOKEN VAZIO");
    }
}

char Lexer::at(int idx)
{
    if (idx >= source.size())
        return 0;
    else
        return source.at(idx);
}

void Lexer::consumir()
{
    if (cur() == '\n')
    {
        linha++;
        coluna = 1;
    }
    else
    {
        if (cur() == '\t')
            coluna += 4;
        else
            coluna++;
    }
    pos++;
    if (pos > source.length() + 10)
        throw std::runtime_error("parser boom!");
}

char Lexer::cur()
{
    return at(pos);
}

char Lexer::peek()
{
    return at(pos + 1);
}

char Lexer::peek2()
{
    return at(pos + 2);
}

bool isSpace(char c)
{
    return c == ' ' || c == '\r' || c == '\t' || c == '\n';
}

void Lexer::pularEspaco()
{
    pularComentario();
    while (isSpace(cur()))
    {
        consumir();
        pularComentario();
    }
}

void Lexer::pularComentario()
{
    int i = verificarComentario();
    if (!i)
        return;
    if (i == 1 || i == 2)
    {
        while (cur() != '\n')
            consumir();
    }
    else if (i == 3)
    {
        while (!(cur() == '*' && peek() == '/'))
            consumir();
        consumir();
        consumir();
    }
}

int Lexer::verificarComentario()
{
    if (cur() == '#') // skip preprocessor instructions
        return 1;
    if (cur() == '/' && peek() == '/')
    {
        return 2;
    }
    if (cur() == '/' && peek() == '*')
    {
        return 3;
    }
    return 0;
}

Lexer::Lexer(const char *_filename, const std::string &s)
{
    ficheiro = _filename;
    pos = 0;
    source = s;
    linha = 1;
    coluna = 1;
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool isIden(char c)
{
    return c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static std::set<char> opChar =
    {'+', '-', '*', '/', '%', '^', '>', '<', '!',
     '=', '(', ')', '[', ']', '{', '}', '.', ':', ',', '\\', '&', '|', '~',
     '?'};

Token Lexer::proximoChar()
{
    if (cur() == ';')
    {
        consumir();
        return makeToken(Token::Type::TERMINADOR, ";", "TOKEN_PONTOVIRG", linha, coluna);
    }
    else if (isdigit(cur()))
    {
        return numero();
    }
    else if (isIden(cur()))
    {
        return identificador();
    }
    else if (opChar.find(cur()) != opChar.end())
    {
        return punctuator();
    }
    else if (cur() == '\"' || cur() == '\'')
    {
        return string();
    }
    throw std::runtime_error(std::string("IMPOSSIVEL FAZER O PARSE") + cur());
}

void Lexer::lerTokens()
{
    try
    {
        pularEspaco();
        while (pos < source.length())
        {
            auto tok = proximoChar();
            if (tokenStream.size() > 0)
            {
                auto last = tokenStream.back();
                if (last.type == tok.type && tok.type == Token::Type::STRING)
                {
                    auto s = last.lexema;
                    s.pop_back();
                    auto s2 = tok.lexema;
                    for (auto iter = s2.begin() + 1; iter != s2.end(); iter++)
                    {
                        s += *iter;
                    }
                    last.lexema = s;
                    tokenStream.pop_back();
                    tokenStream.push_back(last);
                    pularEspaco();
                    continue;
                }
            }
            tokenStream.push_back(tok);
            pularEspaco();
        }
    }
    catch (std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

bool Lexer::verificarPalavraChave(const std::string &s)
{
    return keywords.find(s) != keywords.end();
}

Token Lexer::identificador()
{
    std::string iden;
    while (isIden(cur()) || isdigit(cur()))
    {
        iden += cur();
        consumir();
    }
    if (verificarPalavraChave(iden))
        return makeToken(Token::Type::PALAVRA_CHAVE, iden,keywords.find(iden)->second, linha, coluna);
    else
        return makeToken(Token::Type::IDENTIFICADOR, iden,"TOKEN_IDEN", linha, coluna);
}

Token Lexer::numero()
{
    std::string numero;
    Token::Type ty = Token::Type::INT;
    if (cur() == '0' && peek() == 'x')
    {
        numero += cur();
        numero += peek();
        consumir();
        consumir();
        while (isdigit(cur()) || (cur() >= 'A' && cur() <= 'F') || (cur() >= 'a' && cur() <= 'f'))
        {
            numero += cur();
            consumir();
        }
    }
    else if (cur() == '0' && peek() != 'x' && peek() != '.')
    {
        while (cur() >= '0' && cur() <= '7')
        {
            numero += cur();
            consumir();
        }
    }
    else
    {
        while (isdigit(cur()))
        {
            numero += cur();
            consumir();
        }
        if (cur() == '.')
        {
            ty = Token::Type::DOUBLE;
            numero += cur();
            consumir();
            while (isdigit(cur()))
            {
                numero += cur();
                consumir();
            }
        }
        if (cur() == 'e')
        {
            ty = Token::Type::DOUBLE;
            numero += cur();
            if (peek() == '-')
            {
                consumir();
                numero += cur();
            }
            consumir();
            while (isdigit(cur()))
            {
                numero += cur();
                consumir();
            }
        }
        if (cur() == 'f')
        {
            ty = Token::Type::DOUBLE;
            numero += cur();
            consumir();
        }
    }
    // std::cerr << numero << " "<< (Token::Type::DOUBLE == Token::Type::DOUBLE) <<std::endl;
    return makeToken(ty, numero, ty == Token::Type::DOUBLE ? "TOKEN_DOUBLE" : "TOKEN_INT" , linha, coluna);
}

Token Lexer::punctuator()
{
    char p2 = peek2();
    char p = peek();
    std::string s1, s2, s3;
    s1 = cur();
    s2 = s1 + p;
    s3 = s2 + p2;

    if (operators1.find(s2) != operators1.end())
    {
        auto t = makeToken(Token::Type::PONTUACAO, s2, operators1.find(s2)->second, linha, coluna);
        consumir();
        consumir();
        return t;
    }
    else if (operators2.find(s1) != operators2.end())
    {
        auto t = makeToken(Token::Type::PONTUACAO, s1, operators2.find(s1)->second, linha, coluna);
        consumir();
        return t;
    }
    else
    {
        throw std::runtime_error(std::string("Erro: ") + cur());
    }
}

Token Lexer::string()
{
    std::string s;
    char c = cur();
    consumir();
    while (cur() != c)
    {
        if (cur() == '\\')
        {
            consumir();
            if (cur() == '\\')
            {
                s += "\\";
            }
            else if (cur() == 'n')
            {
                s += "\n";
            }
            else if (cur() == '"')
            {
                s += "\"";
            }
            else if (cur() == '\'')
            {
                s += "\'";
            }
        }
        else
            s += cur();
        consumir();
    }
    consumir();
    if (c == '\'')
    {
        if (s.length() > 1)
            throw std::runtime_error(std::string(""));
        return makeToken(Token::Type::INT, s.length() == 1 ? s : "0", "TOKEN_INT", linha, coluna);
    }

    return makeToken(Token::Type::STRING, s, "TOKEN_STR", linha, coluna);
}

std::vector<Token> &Lexer::todosTokens()
{
    // for(auto x : tokenStream){
    //     std::cout <<  x.toString() <<endl;
    // }
    return tokenStream;
}
