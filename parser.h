#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include "lex.h"

enum class TipoSimbolo
{
    VARIAVEL,
    FUNCAO
};
struct Simbolo
{
    TipoSimbolo tipo;
    std::string tipo_dado;
    std::vector<std::string> parametros;
    std::string valor;
    std::string tipoRetorno;
    int escopo;
    bool declarada = false;

    // Construtor padrão
    Simbolo() : tipo(TipoSimbolo::VARIAVEL), tipo_dado(""), escopo(0) {}

    // Construtor com argumentos
    Simbolo(TipoSimbolo t, const std::string &tipo_d, const std::vector<std::string> &params = {}, int e = 0, std::string v = "")
        : tipo(t), tipo_dado(tipo_d), parametros(params), escopo(e), valor(v) {}
};
class Parser
{
public:
    Parser(const std::vector<Token> &tokens);
    void parse();
    void imprimirTabela();
    std::unordered_map<std::string, Simbolo> tabela;
    std::string funcaoAtual, variavelAtual;
    int erros;
private:
    std::vector<Token> tokens;
    Token tokenAtual;
    size_t indiceToken;

    int escopo_atual = 0;

    void nextToken();
    void erro(const std::string &mensagem);
    void programa();
    void decl_list();
    void decl();
    void decl_1(const std::string &tipo, const std::string &nome);
    void var_decl(const std::string &tipo, const std::string &nome);
    void fun_decl(const std::string &tipo, const std::string &nome);
    void fun_decl_1(string &tipoRetorno);
    void type_spec();
    void params(std::vector<std::string> &parametros);
    void param(std::vector<std::string> &parametros);
    void param_1(std::vector<std::string> &parametros);
    void param_2(std::vector<std::string> &parametros);
    void stmt(string &tipoRetorno);
    void exp_stmt();
    void com_stmt(string &tipoRetorno);
    void content(string &tipoRetorno);
    void do_stmt();
    void while_stmt();
    void local_decls();
    void  simple_exp(string &tipo);
    void local_decl_1();
    void local_decl_2(const std::string &tipo, const std::string &nome);
    void if_stmt();
    void if_stmt_1();
    void for_stmt();
    void return_stmt(string &tipoRetorno);
    void return_stmt_1(string &tipoRetorno);
    void break_stmt();
    void term(string &tipo);
    void simple_exp_1(const std::string &tipoEsq, std::string &tipoRetorno);
    void factor(std::string &tipoRetorno);
    void term_1(const std::string &tipoEsq, std::string &tipoRetorno);
    void call(std::string &tipoRetorno);
    
    void exp( string& tipo );
    void exp_1( string& tipo );
    void exp_2(string& tipo );
    void exp_3( string& tipo);
    void arg_list_1();
    void args();
    void abrirEscopo();
    void fecharEscopo();

    void erroSemantico(const std::string &mensagem);

    bool adicionarSimbolo(const std::string &nome, TipoSimbolo tipo, const std::string &tipo_dado,
                          const std::vector<std::string> &parametros = {}, const std::string valor = "");

    bool adicionarVariavel(const std::string &nome, const std::string &tipo_dado, const std::string valor);

    bool adicionarFuncao(const std::string &nome, const std::string &tipo_dado,
                         const std::vector<std::string> &parametros);

    bool buscarSimbolo(const std::string &nome);

    Simbolo obterSimbolo(const std::string &nome);
};

#endif // PARSER_H
