#ifndef LEX_H
#define LEX_H


#include <list>
#include <vector>
#include <stack>
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <assert.h>
#include <algorithm>
#include <exception>
#include <chrono>
#include <variant>
#include <optional>

using namespace std;



struct Token;

struct Token{

	enum  class Type{
		String, Int, Float, Idenficador, PalavraChave, Pontuacao, Terminador, Erro
	}type;
	string token; //string token
	const char *ficheiro;//ficheiro onde o token foi encontrado 
	int linha, coluna;
	
	Token(Type type, string token, int linha, int coluna);//constructor

	Token() :
			token(""), linha(0), coluna(0), type(Type::Erro) {
	}	
};

class Lexer{
	int posicao, linha, coluna;
	const char * ficheiro;
	string fonte;
	vector<Token> tokenStream;//lista de tokens 
	
	Token proximo();
	
	void consumir();
	
	void pularEspaco();

	void pularComentario();
	
	void isComentario();

	bool isPalavraChave();
	
	Token identificador();

	Token numero();
	
	Token pontuacao();

	Token string();

	template<typename ...Args>
	Token makeToken(Args ... args){
		auto tok = Token(args...);	
		tok.ficheiro = ficheiro;
		return tok;
	}
	public:
		Lexer(const char *fichero, std::string &s);
		
		void ler();
		
		vector<std::string> &getTokenStream();	
};			
#endif
