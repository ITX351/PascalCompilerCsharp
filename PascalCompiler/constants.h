#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

const int KEYWORD_NUM = 16, BUFFER_SIZE = 4095;

typedef enum
{
	T_ARRAY = 0, T_BEGIN, T_DO, T_ELSE, T_END,
	T_FUNCTION, T_IF, T_INTTYPE, T_NOT, T_OF,
	T_PRODEDURE, T_PROGRAM, T_REALTYPE, T_THEN, T_VAR,
	T_WHILE,

    T_ADD, T_ASS, T_COLON, T_COMMA, T_DOUBLEPERIOD,
    T_EQL, T_GT, T_GTE, T_IDN, T_INT,
    T_LBRKPAR, T_LPAR, T_LT, T_LTE, T_MUL,
    T_NEQ, T_RBRKPAR, T_REAL, T_RPAR, T_SEMICL,
    T_SLASH, T_SUB, T_EOF, T_OTHERS // 40
} TokenType;

static const char* keywordList[] = {
	"array", "begin", "do", "else", "end",
	"function", "if", "integer", "not", "of",
	"procedure", "program", "real", "then", "var",
	"while"
};

#endif // CONSTANTS_H_INCLUDED
