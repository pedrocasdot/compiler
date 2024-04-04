#include "lexer.h"

Token::Token(Type type, string token, int linha, int coluna) : 
	type(type), token(token), linha(linha), coluna(coluna) {
			
}
