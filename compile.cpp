
#include "compile.h"
#include "lex.h"

void Compiler::compileFile(const char *filename)
{
    std::string src;
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        std::cerr << "FICHEIRO NÃO EXISTE" << std::endl;
        return;
    }
    while (!feof(f))
    {
        char c = (char)fgetc(f);
        if (c != EOF && c)
            src += c;
    }
    Lexer lex(filename, src);
    lex.lerTokens();
}
