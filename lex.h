
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
		ERRO
	} type;
	std::string lexema;
	std::string token;

	const char *ficheiro;
	int linha;
	int coluna;

	Token(Type t, const std::string le, const std::string to, int l, int c);

	Token() : lexema(""),token(""), linha(0), coluna(0), type(Type::ERRO)
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
		case Type::ERRO:
			typeStr = "ERRO";
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

	void lerTokens();

	std::vector<Token> &todosTokens();
};
#endif
