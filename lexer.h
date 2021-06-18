#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#define NUM_KEYWORDS 45
#define MAX_IDENTIFIER_LENGTH 64

typedef enum TokenBase {
	OperatorTokenBase,
	KeywordTokenBase,
	IdentifierTokenBase,
	ConstantTokenBase,
	StringLiteralTokenBase,
	EOFTokenBase
} TokenBase;

typedef enum OperatorTokenType {
	Ellipsis,             // "..."
	RightShiftAssign,     // ">>="
	LeftShiftAssign,      // "<<=""
	AddAssign,            // "+="
	SubtractAssign,       // "-="
	MultiplyAssign,       // "*="
	DivideAssign,         // "/="
	ModuloAssign,         // "%="
	AndAssign,            // "&="
	XorAssign,            // "^="
	OrAssign,             // "|="
	RightShift,           // ">>"
	LeftShift,            // "<<"
	Increment,            // "++"
	Decrement,            // "--"
	ArrowOperator,        // "->"
	LogicalAnd,           // "&&"
	LogicalOr,            // "||"
	LessThanOrEqualTo,    // "<="
	GreaterThanOrEqualTo, // ">="
	EqualTo,              // "=="
	NotEqualTo,           // "!="
	
	Semicolon,            // ";"
	OpenBrace,            // ("{"|"<%")
	CloseBrace,           // ("}"|"%>")
	Comma,                // ","
	Colon,                // ":"
	Equals,               // "="
	OpenParenthesis,      // "("
	CloseParenthesis,     // ")"
	OpenBracket,          // ("["|"<:")
	CloseBracket,         // ("]"|":>")
	Period,               // "."
	Ampersand,            // "&"
	ExclamationPoint,     // "!"
	Tilde,                // "~"
	Minus,                // "-"
	Plus,                 // "+"
	Asterisk,             // "*"
	ForwardSlash,         // "/"
	Percent,              // "%"
	LessThan,             // "<"
	GreaterThan,          // ">"
	Caret,                // "^"
	Pipe,                 // "|"
	QuestionMark,         // "?"
	
} OperatorTokenType;

typedef enum KeywordTokenType {
	Auto,
	Break,
	Case,
	Char,
	Const,
	Continue,
	Default,
	Do,
	Double,
	Else,
	Enum,
	Extern,
	Float,
	For,
	Goto,
	If,
	Inline,
	Int,
	Long,
	Register,
	Restrict,
	Return,
	Short,
	Signed,
	Sizeof,
	Static,
	Struct,
	Switch,
	Typedef,
	Union,
	Unsigned,
	Void,
	Volatile,
	While,
	AlignAs,
	Alignof,
	Atomic,
	Bool,
	Complex,
	Generic,
	Imaginary,
	Noreturn,
	StaticAssert,
	ThreadLocal,
	Func
} KeywordTokenType;

typedef enum ConstantTokenType {
	IntegerTokenType,
	CharacterLiteralTokenType,
	DoubleTokenType,
	FloatTokenType,
} ConstantTokenType;

typedef struct Token {
	TokenBase token_base;
	char *location;
	size_t length;
} Token;
Token *new_Token(TokenBase type, char *location, size_t length);

typedef struct OperatorToken {
	TokenBase token_base;
	char *location;
	size_t length;
	OperatorTokenType type;
} OperatorToken;
OperatorToken *new_OperatorToken(OperatorTokenType type, char *location, size_t length);

typedef struct KeywordToken {
	TokenBase token_base;
	char *location;
	size_t length;
	KeywordTokenType type;
} KeywordToken;
KeywordToken *new_KeywordToken(KeywordTokenType type, char *location, size_t length);

typedef struct IdentifierToken {
	TokenBase token_base;
	char *location;
	size_t length;
	char value[MAX_IDENTIFIER_LENGTH];
} IdentifierToken;
IdentifierToken *new_IdentifierToken(const char name[MAX_IDENTIFIER_LENGTH], char *location, size_t length);

typedef struct ConstantToken {
	TokenBase token_base;
	char *location;
	size_t length;
	ConstantTokenType type;
	union data {
		unsigned long long as_integer_constant;
		float as_float_constant;
		double as_double_constant;
		char as_character_constant;
	} data;
} ConstantToken;
ConstantToken *new_ConstantToken(ConstantTokenType type, void *data, char *location, size_t length);

typedef struct Lexer {
	int position;
	char *text;
} Lexer;
Lexer *new_Lexer(char *text);
Token *Lexer_getNextToken(Lexer *self);

#endif