#include "parser.h"
#include <string>
#include <iostream>

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens), indiceToken(0), erros(0)
{
    nextToken();
}

void Parser::nextToken()
{
    if (indiceToken < tokens.size())
    {
        tokenAtual = tokens[indiceToken++];
    }
    else
    {
        tokenAtual = Token();
    }
}

void Parser::erro(const std::string &mensagem)
{
    std::cerr << "Erro: " << mensagem << " na linha " << tokenAtual.linha << std::endl;
    erros++;
    while (indiceToken < tokens.size() && tokenAtual.lexema != ";" && tokenAtual.lexema != "}" && tokenAtual.lexema != ")" && tokenAtual.type != Token::Type::END)
    {
        nextToken();
    }
    if (indiceToken < tokens.size())
    {
        nextToken();
    }
}

void Parser::erroSemantico(const std::string &mensagem)
{
    std::cerr << "Erro: " << mensagem << " na linha " << tokenAtual.linha << std::endl;
    erros++;
}

void Parser::parse()
{
    abrirEscopo();
    programa();
    fecharEscopo();
}

void Parser::programa()
{
    decl_list();
}

void Parser::decl_list()
{
    while (tokenAtual.type != Token::Type::END)
    {
        decl();
    }
}

void Parser::decl()
{
    if (tokenAtual.type == Token::Type::END)
        return;

    std::string tipo = tokenAtual.lexema;
    type_spec();
    if (tokenAtual.type == Token::Type::END)
        return;
    if (tokenAtual.type != Token::Type::IDENTIFICADOR)
    {
        erro("Esperava receber um identificador");
    }

    std::string nome = tokenAtual.lexema;
    nextToken();
    decl_1(tipo, nome);
}

void Parser::decl_1(const std::string &tipo, const std::string &nome)
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema == ";" || tokenAtual.lexema == "[" || tokenAtual.lexema == "=" || tokenAtual.lexema == ",")
    {
        var_decl(tipo, nome);
        while (tokenAtual.lexema == ",")
        {
            nextToken();
            if (tokenAtual.type != Token::Type::IDENTIFICADOR)
            {
                erro("Esperava receber um identificador");
            }
            var_decl(tipo, tokenAtual.lexema);
        }
        // decl();
    }
    else if (tokenAtual.lexema == "(")
    {
        fun_decl(tipo, nome);
        decl();
    }
    else
    {
        erro("Esperava receber variável ou a função foi mal declarada");
    }
}

void Parser::var_decl(const std::string &tipo, const std::string &nome)
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema == ";")
    {

        adicionarVariavel(nome, tipo, "Lixo");

        nextToken();
    }
    else if (tokenAtual.lexema == "[")
    {
        nextToken();
        if (tokenAtual.type != Token::Type::INT)
        {
            erro("Esperava receber integer literal");
        }
        nextToken();
        if (tokenAtual.lexema != "]")
        {
            erro("Esperava receber ]");
        }
        nextToken();
        if (tokenAtual.lexema != ";")
        {
            erro("Esperava receber ';'");
        }
        adicionarVariavel(nome, tipo, "Lixo");
        nextToken();
    }
    else if (tokenAtual.lexema == "=")
    {
        nextToken();
        std::string valor = tokenAtual.lexema;
        string tipoRetorno;
        exp(tipoRetorno);
        if (tokenAtual.lexema != ";")
        {
            erro("Esperava receber ';'");
        }
        adicionarVariavel(nome, tipo, valor);
        if (tipo != tipoRetorno)
        {
            erroSemantico("Tipo da variável diferente do tipo de conteúdo");
        }
        tabela[nome].tipoRetorno = tipoRetorno;
        nextToken();
    }
    else if (tokenAtual.lexema == ",")
    {
        adicionarVariavel(nome, tipo, "Lixo");
        nextToken();
        if (tokenAtual.type != Token::Type::IDENTIFICADOR)
        {
            erro("Esperava receber um identificador");
        }
        var_decl(tipo, tokenAtual.lexema);
    }
    else
    {
        erro("Variável mal declarada");
    }
}

void Parser::fun_decl(const std::string &tipo, const std::string &nome)
{

    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema != "(")
    {
        erro("Esperava receber (");
    }
    nextToken();

    std::vector<std::string> parametros;
    params(parametros);
    if (tokenAtual.lexema != ")")
    {
        erro("Esperava receber )");
    }
    nextToken();
    funcaoAtual = nome;
    adicionarFuncao(nome, tipo, parametros);
    string tipoRetorno;
    fun_decl_1(tipoRetorno);
    tabela[nome].tipoRetorno = tipoRetorno;
    if(tipoRetorno.size() == 0)tipoRetorno = "void";
    if (tipo != tipoRetorno)
    {
        erroSemantico("Tipo da função diferente do tipo de conteúdo");
    }
}

void Parser::type_spec()
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema == "void" || tokenAtual.lexema == "double" || tokenAtual.lexema == "string" || tokenAtual.lexema == "int")
    {
        nextToken();
    }
    else
    {
        erro("Esperava receber um tipo de dado");
    }
}

void Parser::fun_decl_1(string &tipoRetorno)
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema == ";")
    {
        nextToken();
    }
    else
    {
        com_stmt(tipoRetorno);
    }
}

void Parser::params(std::vector<std::string> &parametros)
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema == "void")
    {
        nextToken();
    }
    else if (tokenAtual.lexema == "void" || tokenAtual.lexema == "double" || tokenAtual.lexema == "string" || tokenAtual.lexema == "int")
    {
        cout << "teste" << endl;
        param(parametros);
    }
    else
    {
        // vazio
    }
}

void Parser::param(std::vector<std::string> &parametros)
{
    if (tokenAtual.type == Token::Type::END)
        return;

    std::string tipo = tokenAtual.lexema;
    type_spec();
    if (tokenAtual.type != Token::Type::IDENTIFICADOR)
    {
        erro("Esperava receber um identificador");
    }
    parametros.push_back(tipo);
    nextToken();
    param_1(parametros);
}

void Parser::param_1(std::vector<std::string> &parametros)
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema == "[")
    {
        nextToken();
        if (tokenAtual.lexema != "]")
        {
            erro("Esperava receber ]");
        }
        nextToken();
        param_2(parametros);
    }
    else if (tokenAtual.lexema == ",")
    {
        nextToken();
        param(parametros);
    }
    else
    {
        // vazio
    }
}

void Parser::param_2(std::vector<std::string> &parametros)
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema == ",")
    {
        nextToken();
        param(parametros);
    }
    else
    {
        // vazio
    }
}

void Parser::stmt(string &tipoRetorno)
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema == ";" || tokenAtual.type == Token::Type::IDENTIFICADOR || tokenAtual.lexema == "+" || tokenAtual.lexema == "-" || tokenAtual.lexema == "!" || tokenAtual.lexema == "double" || tokenAtual.lexema == "string" || tokenAtual.lexema == "int")
    {
        exp_stmt();
    }
    else if (tokenAtual.lexema == "{")
    {
        string default_ = "null";
        com_stmt(default_);
    }
    else if (tokenAtual.lexema == "if")
    {
        if_stmt();
    }
    else if (tokenAtual.lexema == "while")
    {
        while_stmt();
    }
    else if (tokenAtual.lexema == "return")
    {
        return_stmt(tipoRetorno);
    }
    else if (tokenAtual.lexema == "break")
    {
        break_stmt();
    }
    else if (tokenAtual.lexema == "for")
    {
        for_stmt();
    }
    else if (tokenAtual.lexema == "do")
    {
        do_stmt();
    }
    else
    {
        erro("Statement Inválido");
    }
}

void Parser::exp_stmt()
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema == ";")
    {
        nextToken();
    }
    else
    {
        string default_ = "null";
        exp(default_);
        if (tokenAtual.lexema != ";")
        {
            erro("Esperava receber ';'");
        }
        nextToken();
    }
}

void Parser::com_stmt(string &tipoRetorno)
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema != "{")
    {
        erro("Esperava receber '{'");
    }
    abrirEscopo();
    nextToken();
    content(tipoRetorno);
    if (tokenAtual.lexema != "}")
    {
        erro("Esperava receber }");
    }
    fecharEscopo();
    nextToken();
}

void Parser::content(string &tipoRetorno)
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema == "void" || tokenAtual.lexema == "double" || tokenAtual.lexema == "string" || tokenAtual.lexema == "int")
    {
        local_decls();
    }
    while (tokenAtual.lexema != "}" && tokenAtual.type != Token::Type::END)
    {
        stmt(tipoRetorno);
    }
}

void Parser::do_stmt()
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema != "do")
    {
        erro("Esperava receber 'do'");
    }
    nextToken();
    string null = "null";
    stmt(null);
    if (tokenAtual.lexema != "while")
    {
        erro("Esperava receber 'while'");
    }
    nextToken();
    if (tokenAtual.lexema != "(")
    {
        erro("Esperava receber '('");
    }
    nextToken();
    string default_ = "null";
    exp(default_);
    if (tokenAtual.lexema != ")")
    {
        erro("Esperava receber ')'");
    }
    nextToken();
    if (tokenAtual.lexema != ";")
    {
        erro("Esperava receber ';'");
    }
    nextToken();
}

void Parser::while_stmt()
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema != "while")
    {
        erro("Esperava receber 'while'");
    }
    nextToken();
    if (tokenAtual.lexema != "(")
    {
        erro("Esperava receber '('");
    }
    nextToken();
    string default_ = "null";
    exp(default_);
    if (tokenAtual.lexema != ")")
    {
        erro("Esperava receber ')'");
    }
    nextToken();
    stmt(default_);
}

void Parser::local_decls()
{
    while ((tokenAtual.lexema == "void" || tokenAtual.lexema == "double" || tokenAtual.lexema == "string" || tokenAtual.lexema == "int") && tokenAtual.type != Token::Type::END)
    {
        local_decl_1();
    }
}

void Parser::local_decl_1()
{
    if (tokenAtual.type == Token::Type::END)
        return;

    std::string tipo = tokenAtual.lexema;
    type_spec();
    if (tokenAtual.type == Token::Type::END)
        return;
    if (tokenAtual.type != Token::Type::IDENTIFICADOR)
    {
        erro("Esperava receber um identificador");
    }
    std::string nome = tokenAtual.lexema;
    nextToken();
    local_decl_2(tipo, nome);
}

void Parser::local_decl_2(const std::string &tipo, const std::string &nome)
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema == ";")
    {
        adicionarVariavel(nome, tipo, "Lixo");

        nextToken();
    }
    else if (tokenAtual.lexema == "[")
    {
        nextToken();
        if (tokenAtual.type != Token::Type::INT)
        {
            erro("Esperava receber integer literal");
        }
        nextToken();
        if (tokenAtual.lexema != "]")
        {
            erro("Esperava receber ']'");
        }
        nextToken();
        if (tokenAtual.lexema != ";")
        {
            erro("Esperava receber ';'");
        }
        adicionarVariavel(nome, tipo, "Lixo");
        nextToken();
    }
    else if (tokenAtual.lexema == "=")
    {
        nextToken();
        std::string valor = tokenAtual.lexema, tipoRetorno;
        variavelAtual = nome;

        exp(tipoRetorno);
        if (tokenAtual.lexema != ";")
        {
            erro("Esperava receber ';'");
        }
        adicionarVariavel(nome, tipo, valor);
        if (tipo != tipoRetorno)
        {
            erroSemantico("Tipo da variável diferente do tipo de conteúdo");
        }
        tabela[nome].tipoRetorno = tipoRetorno;
        nextToken();
    }
    else
    {
        erro("Variável mal declarada");
    }
}

void Parser::if_stmt()
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema != "if")
    {
        erro("Esperava receber 'if'");
    }
    nextToken();
    if (tokenAtual.lexema != "(")
    {
        erro("Esperava receber '('");
    }
    nextToken();
    string default_ = "null";
    exp(default_);
    if (tokenAtual.lexema != ")")
    {
        erro("Esperava receber ')'");
    }
    nextToken();
    stmt(default_);
    if_stmt_1();
}

void Parser::if_stmt_1()
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema == "else")
    {
        nextToken();
        string default_ = "null";
        stmt(default_);
    }
}

void Parser::for_stmt()
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema != "for")
    {
        erro("Esperava receber 'for'");
    }
    nextToken();
    if (tokenAtual.lexema != "(")
    {
        erro("Esperava receber '('");
    }
    nextToken();
    string default_ = "null";
    exp(default_);
    if (tokenAtual.lexema != ";")
    {
        erro("Esperava receber ';'");
    }
    nextToken();
    exp(default_);
    if (tokenAtual.lexema != ";")
    {
        erro("Esperava receber ';'");
    }
    nextToken();
    exp(default_);
    if (tokenAtual.lexema != ")")
    {
        erro("Esperava receber ')'");
    }
    nextToken();
    stmt(default_);
}

void Parser::return_stmt(string &tipoRetorno)
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema != "return")
    {
        erro("Esperava receber 'return'");
    }
    nextToken();
    return_stmt_1(tipoRetorno);
}

void Parser::return_stmt_1(string &tipoRetorno)
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema == ";")
    {
        nextToken();
    }
    else
    {
        exp(tipoRetorno);
        if (tokenAtual.lexema != ";")
        {
            erro("Esperava receber ';'");
        }
        nextToken();
    }
}

void Parser::break_stmt()
{
    if (tokenAtual.type == Token::Type::END)
        return;

    if (tokenAtual.lexema != "break")
    {
        erro("Esperava receber 'break'");
    }
    nextToken();
    if (tokenAtual.lexema != ";")
    {
        erro("Esperava receber ';'");
    }
    nextToken();
}

void Parser::exp(string &tipo)
{
    exp_1(tipo);
}

void Parser::exp_1(string &tipo)
{
    exp_2(tipo);
    while (tokenAtual.lexema == "=")
    {
        nextToken();
        exp_2(tipo);
    }
}

void Parser::exp_2(string &tipo)
{
    exp_3(tipo);
}

void Parser::exp_3(string &tipo)
{
    if (tokenAtual.lexema == "+" || tokenAtual.lexema == "-")
    {
        nextToken();
    }
    if (tokenAtual.type == Token::Type::IDENTIFICADOR)
    {
        nextToken();
    }
    else if (tokenAtual.type == Token::Type::INT || tokenAtual.type == Token::Type::DOUBLE || tokenAtual.type == Token::Type::STRING)
    {
        if (tokenAtual.type == Token::Type::INT)
        {
            tipo = "int";
        }
        else if (tokenAtual.type == Token::Type::DOUBLE)
        {
            tipo = "double";
        }
        else if (tokenAtual.type == Token::Type::STRING)
        {
            tipo = "string";
        }
        nextToken();
    }
    else
    {
        erro("Expressão inválida");
    }
}

void Parser::arg_list_1()
{
    string t = "12";
    exp(t);
    while (tokenAtual.lexema == ",")
    {
        nextToken();
        exp(t);
    }
}

void Parser::args()
{
    if (tokenAtual.lexema != ")")
    {
        arg_list_1();
    }
}
void Parser::abrirEscopo()
{
    escopo_atual++;
}

void Parser::fecharEscopo()
{
    // for (auto it = tabela.begin(); it != tabela.end();)
    // {
    //     if (it->second.escopo == escopo_atual)
    //     {
    //         it = tabela.erase(it);
    //     }
    //     else
    //     {
    //         ++it;
    //     }
    // }
    escopo_atual--;
}

bool Parser::adicionarSimbolo(const std::string &nome, TipoSimbolo tipo, const std::string &tipo_dado,
                              const std::vector<std::string> &parametros, std::string valor)
{
    if (tabela.find(nome) != tabela.end() && tabela[nome].escopo == escopo_atual)
    {
        erroSemantico("Já existe uma variável/função com o mesmo nome neste escopo");
        return false;
    }

    tabela[nome] = Simbolo(tipo, tipo_dado, parametros, escopo_atual);
    tabela[nome].valor = valor;
    return true;
}

bool Parser::adicionarVariavel(const std::string &nome, const std::string &tipo_dado, std::string valor)
{
    return adicionarSimbolo(nome, TipoSimbolo::VARIAVEL, tipo_dado, {}, valor);
}

bool Parser::adicionarFuncao(const std::string &nome, const std::string &tipo_dado,
                             const std::vector<std::string> &parametros)
{
    return adicionarSimbolo(nome, TipoSimbolo::FUNCAO, tipo_dado, parametros);
}

bool Parser::buscarSimbolo(const std::string &nome)
{
    auto it = tabela.find(nome);
    return it != tabela.end() && it->second.escopo <= escopo_atual;
}

Simbolo Parser::obterSimbolo(const std::string &nome)
{
    auto it = tabela.find(nome);
    if (it != tabela.end() && it->second.escopo <= escopo_atual)
    {
        return it->second;
    }
    throw std::runtime_error("Símbolo não encontrado.");
}
void Parser::imprimirTabela()
{

    for (const auto &par : tabela)
    {
        const auto &nome = par.first;
        const auto &simbolo = par.second;
        std::cout << "Nome: " << nome
                  << ", Tipo: " << (simbolo.tipo == TipoSimbolo::VARIAVEL ? "Variável" : "Função")
                  << ", Tipo de dado: " << simbolo.tipo_dado
                  << ", Escopo: " << simbolo.escopo
                  << ", Tipo de retorno " << simbolo.tipoRetorno;
        if (simbolo.tipo == TipoSimbolo::FUNCAO)
        {
            std::cout << ", Parâmetros: ";
            for (const auto &param : simbolo.parametros)
            {
                std::cout << param << " ";
            }
        }
        if (simbolo.tipo == TipoSimbolo::VARIAVEL)
        {
            std::cout << ", Valor: " << simbolo.valor;
        }
        std::cout << std::endl;
    }
}