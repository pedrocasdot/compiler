# Mini Compiler

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()

A lightweight compiler implementation in C++ that performs lexical analysis, syntax parsing, and semantic analysis for a custom programming language.

## 📋 Features

- **Lexical Analysis**: Tokenizes source code into meaningful tokens
- **Syntax Analysis**: Implements a recursive descent parser to build a parse tree
- **Semantic Analysis**: Performs type checking and scope resolution
- **Error Handling**: Provides detailed error messages with line numbers
- **Symbol Table**: Maintains information about variables and functions

## 🚀 Getting Started

### Prerequisites

- C++17 or later
- GNU Make
- GCC or Clang compiler

### Building the Compiler

```bash
# Clone the repository
git clone <repository-url>
cd compiler

# Build the project
make
```

### Usage

```bash
# Compile a source file
./main input.txt
```

## 🏗️ Project Structure

```
compiler/
├── bin/                 # Compiled object files
├── include/             # Header files
│   ├── lex.h           # Lexical analyzer
│   ├── parser.h        # Parser
│   ├── semantic.h      # Semantic analyzer
│   └── compile.h       # Main compiler interface
├── src/                # Source files
│   ├── lex.cpp
│   ├── parser.cpp
│   ├── semantic.cpp
│   └── compile.cpp
├── main.cpp            # Entry point
├── makefile            # Build configuration
└── README.md           # This file
```

## 📝 Example

Input (`input.txt`):
```c
void main() {
    int x = 123;
    int y = 456;
    int sum = x + y;
}
```

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

# Mini Compilador

[![Licença: MIT](https://img.shields.io/badge/Licença-MIT-amarelo.svg)](https://opensource.org/licenses/MIT)
[![Status de Compilação](https://img.shields.io/badge/compilação-sucesso-verde)]()

Uma implementação leve de um compilador em C++ que realiza análise léxica, análise sintática e análise semântica para uma linguagem de programação personalizada.

## 📋 Funcionalidades

- **Análise Léxica**: Transforma o código-fonte em tokens significativos
- **Análise Sintática**: Implementa um parser descendente recursivo para construir a árvore de análise
- **Análise Semântica**: Realiza verificação de tipos e resolução de escopo
- **Tratamento de Erros**: Fornece mensagens de erro detalhadas com números de linha
- **Tabela de Símbolos**: Mantém informações sobre variáveis e funções

## 🚀 Começando

### Pré-requisitos

- C++17 ou superior
- GNU Make
- Compilador GCC ou Clang

### Compilando o Projeto

```bash
# Clonar o repositório
git clone <url-do-repositório>
cd compiler

# Compilar o projeto
make
```

### Uso

```bash
# Compilar um arquivo de origem
./main arquivo_entrada.txt
```

## 🤝 Contribuindo

Contribuições são bem-vindas! Sinta-se à vontade para enviar um Pull Request.

## 📄 Licença

Este projeto está licenciado sob a licença MIT - consulte o arquivo [LICENSE](LICENSE) para obter detalhes.
