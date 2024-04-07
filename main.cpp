#include "compile.h"

int main(int argc, char **argv)
{
        Compiler compiler;
        compiler.compileFile(argv[1]);
        return 0;
}