#include "semantic.h"

Semantic::Semantic()
{

}


void Semantic::analisar(const char* filename){
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
    Parser parser(lex.todosTokens());
    parser.parse();
    if(parser.erros == 0){
        cout << "Código foi executado sem nenhum erro\n";
    }
}