
#include "compile.h"
#include "semantic.h"

void Compiler::compileFile(const char *filename)
{
   
    Semantic sem;
    sem.analisar(filename);
}
