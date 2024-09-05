
#ifndef LEX_H_
#define LEX_H_

#include <list>
#include <vector>
#include <stack>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <assert.h>
#include <algorithm>
#include <exception>
#include <chrono>
#include <variant>
#include <optional>

using namespace std;

struct Token;

struct Token
{
	enum class Type
	{
		STRING,
		INT,
		DOUBLE,
		IDENTIFICADOR,
		PALAVRA_CHAVE,
		PONTUACAO,
		TERMINADOR,
		END
	} type;
	std::string lexema;
	std::string token;

	const char *ficheiro;
	int linha;
	int coluna;

	Token(Type t, const std::string le, const std::string to, int l, int c);

	Token() : lexema(""),token(""), linha(0), coluna(0), type(Type::END)
	{
	}
	std::string toString() const
	{
		std::string typeStr;
		switch (type)
		{
		case Type::STRING:
			typeStr = "STRING";
			break;
		case Type::INT:
			typeStr = "INT";
			break;
		case Type::DOUBLE:
			typeStr = "DOUBLE";
			break;
		case Type::IDENTIFICADOR:
			typeStr = "IDENTIFICADOR";
			break;
		case Type::PALAVRA_CHAVE:
			typeStr = "PALAVRA_CHAVE";
			break;
		case Type::PONTUACAO:
			typeStr = "PONTUACAO";
			break;
		case Type::TERMINADOR:
			typeStr = "TERMINADOR";
			break;
		case Type::END:
			typeStr = "END";
			break;
		}

		return "GRUPO: " + typeStr + " | Lexema: " + lexema + " | Token: " + token + " | Filename: " + ficheiro + " | Line: " + to_string(linha);
	}
};

class Lexer
{
	int pos;
	int linha, coluna;
	const char *ficheiro;
	std::string source;
	std::vector<Token> tokenStream;

	char at(int idx);

	Token proximoChar();

	void consumir();

	char cur();

	char peek();

	char peek2();

	void pularEspaco();

	void pularComentario();

	int verificarComentario();

	bool verificarPalavraChave(const std::string &s);

	Token identificador();

	Token numero();

	Token punctuator();

	Token string();
	template <typename... Args>
	Token makeToken(Args... args)
	{
		auto t = Token(args...);
		t.ficheiro = ficheiro;
		return t;
	}

public:
	Lexer(const char *ficheiro, const std::string &s);
	const std::map<std::string, std::string> keywords = {
        {"auto", "TOKEN_AUTO"}, {"break", "TOKEN_BREAK"}, {"case", "TOKEN_CASE"}, {"char", "TOKEN_CHAR"},
        {"const", "TOKEN_CONST"}, {"continue", "TOKEN_CONTINUE"}, {"default", "TOKEN_DEFAULT"}, {"do", "TOKEN_DO"},
        {"double", "TOKEN_DOUBLE"}, {"else", "TOKEN_ELSE"}, {"enum", "TOKEN_ENUM"}, {"extern", "TOKEN_EXTERN"},
        {"float", "TOKEN_FLOAT"}, {"for", "TOKEN_FOR"}, {"goto", "TOKEN_GOTO"}, {"if", "TOKEN_IF"},
        {"inline", "TOKEN_INLINE"}, {"int", "TOKEN_INT"}, {"long", "TOKEN_LONG"}, {"register", "TOKEN_REGISTER"},
        {"restrict", "TOKEN_RESTRICT"}, {"return", "TOKEN_RETURN"}, {"short", "TOKEN_SHORT"},
        {"signed", "TOKEN_SIGNED"}, {"sizeof", "TOKEN_SIZEOF"}, {"static", "TOKEN_STATIC"},
        {"struct", "TOKEN_STRUCT"}, {"switch", "TOKEN_SWITCH"}, {"typedef", "TOKEN_TYPEDEF"},
        {"union", "TOKEN_UNION"}, {"unsigned", "TOKEN_UNSIGNED"}, {"void", "TOKEN_VOID"},
        {"while", "TOKEN_WHILE"}
    };
    const std::map<std::string, std::string> operators1 = {
        {"&&", "TOKEN_E_LOGICO"}, 
        {"||", "TOKEN_OU_LOGICO"}, 
        {"++", "TOKEN_INCREMENTO"}, 
        {"--", "TOKEN_DECREMENTO"}, 
        {"+=", "TOKEN_SOMA_ATRIBUICAO"}, 
        {"-=", "TOKEN_SUBTRACAO_ATRIBUICAO"}, 
        {"*=", "TOKEN_MULTIPLICACAO_ATRIBUICAO"}, 
        {"/=", "TOKEN_DIVISAO_ATRIBUICAO"}, 
        {"%=", "TOKEN_RESTO_ATRIBUICAO"}, 
        {"|=", "TOKEN_OU_BIT_A_BIT_ATRIBUICAO"}, 
        {"&=", "TOKEN_E_BIT_A_BIT_ATRIBUICAO"}, 
        {"^=", "TOKEN_OU_EXCLUSIVO_BIT_A_BIT_ATRIBUICAO"}, 
        {">=", "TOKEN_MAIOR_IGUAL"}, 
        {"<=", "TOKEN_MENOR_IGUAL"}, 
        {"!=", "TOKEN_DIFERENTE"}, 
        {"==", "TOKEN_IGUAL"}, 
        {"->", "TOKEN_SETA"}, 
        {">>", "TOKEN_DESLOCAMENTO_DIREITA"}, 
        {"<<", "TOKEN_DESLOCAMENTO_ESQUERDA"}
    };

     const std::map<std::string, std::string> operators2 = {
        {"+", "TOKEN_MAIS"}, 
        {"-", "TOKEN_MENOS"}, 
        {"*", "TOKEN_MULTIPLICACAO"}, 
        {"/", "TOKEN_DIVISAO"}, 
        {"%", "TOKEN_RESTO"}, 
        {"&", "TOKEN_E_BIT_A_BIT"}, 
        {"|", "TOKEN_OU_BIT_A_BIT"}, 
        {"^", "TOKEN_OU_EXCLUSIVO_BIT_A_BIT"}, 
        {"(", "TOKEN_PARENTESES_ESQUERDO"}, 
        {")", "TOKEN_PARENTESES_DIREITO"}, 
        {"[", "TOKEN_COLCHETES_ESQUERDO"}, 
        {"]", "TOKEN_COLCHETES_DIREITO"}, 
        {"{", "TOKEN_CHAVES_ESQUERDO"}, 
        {"}", "TOKEN_CHAVES_DIREITO"}, 
        {",", "TOKEN_VIRGULA"}, 
        {"=", "TOKEN_ATRIBUICAO"}, 
        {"\\", "TOKEN_BARRA_INVERTIDA"}, 
        {"<", "TOKEN_MENOR_QUE"}, 
        {">", "TOKEN_MAIOR_QUE"}, 
        {".", "TOKEN_PONTO"}, 
        {":", "TOKEN_DOIS_PONTOS"}, 
        {"?", "TOKEN_INTERROGACAO"}, 
        {"~", "TOKEN_BIT_A_BIT_NAO"}, 
        {"!", "TOKEN_NEGACAO_LOGICA"}
    };


	void lerTokens();

	std::vector<Token> &todosTokens();
};
#endif
