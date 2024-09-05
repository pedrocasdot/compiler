#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "parser.h"
#include <vector>
#include <string>

class Semantic
{
public:
    Semantic();
    void analisar(const char* filename);
private:

    unordered_map<std::string, Simbolo> declarations;
    bool verificarTipoDeRetorno();
    bool verificarCompatibilidadeEmAtribuicao();
};

#endif // SEMANTIC_H
