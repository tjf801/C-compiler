#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//NOTE: each keyword is the exact same index as the enum
const char *keywords[NUM_KEYWORDS] = {
	"auto",
	"break",
	"case",
	"char",
	"const",
	"continue",
	"default",
	"do",
	"double",
	"else",
	"enum",
	"extern",
	"float",
	"for",
	"goto",
	"if",
	"inline",
	"int",
	"long",
	"register",
	"restrict",
	"return",
	"short",
	"signed",
	"sizeof",
	"static",
	"struct",
	"switch",
	"typedef",
	"union",
	"unsigned",
	"void",
	"volatile",
	"while",
	"_Alignas",
	"_Alignof",
	"_Atomic",
	"_Bool",
	"_Complex",
	"_Generic",
	"_Imaginary",
	"_Noreturn",
	"_Static_assert",
	"_Thread_local",
	"__func__",
};
_Static_assert(Func-Auto+1==NUM_KEYWORDS, "number of keywords is not set correctly");

Token *new_Token(TokenBase base, char *location, size_t length) {
	Token *T = (Token*) malloc(sizeof(Token));
	T->token_base = base;
	T->location = location;
	T->length = length;
	return T;
}

KeywordToken *new_KeywordToken(KeywordTokenType type, char *location, size_t length) {
	KeywordToken *K = malloc(sizeof(KeywordToken));
	K->token_base = KeywordTokenBase;
	K->location = location;
	K->length = length;
	K->type = type;
	return K;
}

IdentifierToken *new_IdentifierToken(const char name[64], char *location, size_t length) {
	IdentifierToken *I = malloc(sizeof(IdentifierToken));
	I->token_base = IdentifierTokenBase;
	I->location = location;
	I->length = length;
	strcpy(I->value, name);
	return I;
}

OperatorToken *new_OperatorToken(OperatorTokenType type, char *location, size_t length) {
	OperatorToken *O = malloc(sizeof(OperatorToken));
	O->token_base = OperatorTokenBase;
	O->location = location;
	O->length = length;
	O->type = type;
	return O;
}

ConstantToken *new_ConstantToken(ConstantTokenType type, void *data, char *location, size_t length) {
	ConstantToken *C = malloc(sizeof(ConstantToken));
	C->token_base = ConstantTokenBase;
	C->location = location;
	C->length = length;
	C->type = type;
	if (type==IntegerTokenType) {
		C->data.as_integer_constant = *(int*)data;
	}
	return C;
}

Lexer *new_Lexer(char *text) {
	Lexer *L = (Lexer*) malloc(sizeof(Lexer));
	L->position=0;
	L->text = text;
	return L;
}

Token *Lexer_getNextToken(Lexer *self) {
	lex_start:
	while (isspace(self->text[self->position])) self->position++; //skip any whitespace
	if (self->text[self->position]=='/' && self->text[self->position+1]=='/') { //single line comments
		while (self->text[self->position]!='\n' && self->text[self->position]!='\0') ++self->position;
		goto lex_start; //im so sorry
	}
	else if (self->text[self->position]=='/' && self->text[self->position+1]=='*') { /*multi line comments*/
		while (!(self->text[self->position]=='*' && self->text[self->position+1]=='/') && self->text[self->position]!='\0') ++self->position;
		if (self->text[self->position]!='\0') return NULL; //TODO: raise error
		goto lex_start; //im so sorry again
	}
	else if (self->text[self->position]=='\0') return new_Token(EOFTokenBase, &(self->text[self->position]), 1);
	else if (self->text[self->position]=='"') { //string literal
		//TODO
		return NULL;
	}
	else if (self->text[self->position]=='\'') { // character literal
		//TODO
		return NULL;
	}
	else if (self->text[self->position]=='_' || isalpha(self->text[self->position])) { // identifier or keyword
		//TODO: theres probably a better way to do all of this
		// gets the length of the identifier
		int id_end;
		for (id_end = self->position; self->text[id_end]=='_' || isalnum(self->text[id_end]); ++id_end);
		int id_len = id_end - self->position;
		
		const int clamped_id_len = id_len>MAX_IDENTIFIER_LENGTH-1?MAX_IDENTIFIER_LENGTH-1:id_len;
		
		char id_name[MAX_IDENTIFIER_LENGTH];
		strncpy(id_name, &(self->text[self->position]), clamped_id_len);
		id_name[clamped_id_len+1] = '\0';
		
		Token *result = NULL;
		
		for(int i = 0; i < NUM_KEYWORDS; ++i) {
			if (memcmp(keywords[i], id_name, strlen(keywords[i]))==0) {
				result = (Token*)new_KeywordToken((KeywordTokenType)i, &(self->text[self->position]), clamped_id_len);
				break;
			}
		}
		
		if (result==NULL) {
			result = (Token*)new_IdentifierToken(id_name, &(self->text[self->position]), clamped_id_len);
		}
		
		self->position += id_len;
		
		return result;
	}
	else if (isdigit(self->text[self->position])) { // numeric things
		// TODO: parse numbers besides plain decimal integers
		// TODO: this fucking sucks
		char *location = &(self->text[self->position]);
		char *end = NULL;
		unsigned long long value = strtoull(location, &end, 10);
		self->position += (end - location);
		
		if (end==location) return NULL;
		
		return (Token*)new_ConstantToken(IntegerTokenType, &value, location, end-location);
	}
	else { // operator or invalid symbol
		//TODO: use a switch statement ffs
		//the resulting token (that should be an operator)
		Token *result = NULL;
		
		if (self->text[self->position]=='.') {
			if (self->text[self->position+1]=='.'&&self->text[self->position+2]=='.') {
				result = (Token*)new_OperatorToken(Ellipsis, &(self->text[self->position]), 3);
				self->position += 3;
			}
			else {
				result = (Token*)new_OperatorToken(Period, &(self->text[self->position]), 1);
				self->position += 1;
			}
		}
		else if (self->text[self->position]=='<') {
			if (self->text[self->position+1]=='<'&&self->text[self->position+2]=='=') {
				result = (Token*)new_OperatorToken(LeftShiftAssign, &(self->text[self->position]), 3);
				self->position += 3;
			}
			else if (self->text[self->position+1]=='=') {
				result = (Token*)new_OperatorToken(LessThanOrEqualTo, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else if (self->text[self->position+1]=='<') {
				result = (Token*)new_OperatorToken(LeftShift, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else if (self->text[self->position+1]=='%') {
				result = (Token*)new_OperatorToken(OpenBrace, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else if (self->text[self->position+1]==':') {
				result = (Token*)new_OperatorToken(OpenBrace, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else {
				result = (Token*)new_OperatorToken(LessThan, &(self->text[self->position]), 1);
				self->position += 1;
			}
		}
		else if (self->text[self->position]=='>') {
			if (self->text[self->position+1]=='>'&&self->text[self->position+2]=='=') {
				result = (Token*)new_OperatorToken(RightShiftAssign, &(self->text[self->position]), 3);
				self->position += 3;
			}
			else if (self->text[self->position+1]=='=') {
				result = (Token*)new_OperatorToken(GreaterThanOrEqualTo, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else if (self->text[self->position+1]=='>') {
				result = (Token*)new_OperatorToken(RightShift, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else {
				result = (Token*)new_OperatorToken(GreaterThan, &(self->text[self->position]), 1);
				self->position += 1;
			}
		}
		else if (self->text[self->position]=='+') {
			if (self->text[self->position+1]=='=') {
				result = (Token*)new_OperatorToken(AddAssign, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else if (self->text[self->position+1]=='+') {
				result = (Token*)new_OperatorToken(Increment, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else {
				result = (Token*)new_OperatorToken(Plus, &(self->text[self->position]), 1);
				self->position += 1;
			}
		}
		else if (self->text[self->position]=='-') {
			if (self->text[self->position+1]=='=') {
				result = (Token*)new_OperatorToken(SubtractAssign, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else if (self->text[self->position+1]=='-') {
				result = (Token*)new_OperatorToken(Decrement, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else if (self->text[self->position+1]=='>') {
				result = (Token*)new_OperatorToken(ArrowOperator, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else {
				result = (Token*)new_OperatorToken(Minus, &(self->text[self->position]), 1);
				self->position += 1;
			}
		}
		else if (self->text[self->position]=='*') {
			if (self->text[self->position+1]=='=') {
				result = (Token*)new_OperatorToken(MultiplyAssign, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else {
				result = (Token*)new_OperatorToken(Asterisk, &(self->text[self->position]), 1);
				self->position += 1;
			}
		}
		else if (self->text[self->position]=='/') {
			if (self->text[self->position+1]=='=') {
				result = (Token*)new_OperatorToken(DivideAssign, &(self->text[self->position]), 2);
				self->position += 2;
			} 
			else {
				result = (Token*)new_OperatorToken(ForwardSlash, &(self->text[self->position]), 1);
				self->position += 1;
			}
		}
		else if (self->text[self->position]=='%') {
			if (self->text[self->position+1]=='=') {
				result = (Token*)new_OperatorToken(ModuloAssign, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else if (self->text[self->position+1]=='>') {
				result = (Token*)new_OperatorToken(CloseBrace, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else {
				result = (Token*)new_OperatorToken(Percent, &(self->text[self->position]), 2);
				self->position += 1;
			}
		}
		else if (self->text[self->position]=='&') {
			if (self->text[self->position+1]=='=') {
				result = (Token*)new_OperatorToken(AndAssign, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else if (self->text[self->position+1]=='&') {
				result = (Token*)new_OperatorToken(LogicalAnd, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else {
				result = (Token*)new_OperatorToken(Ampersand, &(self->text[self->position]), 1);
				self->position += 1;
			}
		}
		else if (self->text[self->position]=='|') {
			if (self->text[self->position+1]=='=') {
				result = (Token*)new_OperatorToken(OrAssign, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else if (self->text[self->position+1]=='|') {
				result = (Token*)new_OperatorToken(LogicalOr, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else {
				result = (Token*)new_OperatorToken(Pipe, &(self->text[self->position]), 1);
				self->position += 1;
			}
		}
		else if (self->text[self->position]=='^') {
			if (self->text[self->position+1]=='=') {
				result = (Token*)new_OperatorToken(XorAssign, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else {
				result = (Token*)new_OperatorToken(Caret, &(self->text[self->position]), 1);
				self->position += 1;
			}
		}
		else if (self->text[self->position]=='=') {
			if (self->text[self->position+1]=='=') {
				result = (Token*)new_OperatorToken(EqualTo, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else {
				result = (Token*)new_OperatorToken(Equals, &(self->text[self->position]), 1);
				self->position += 1;
			}
		}
		else if (self->text[self->position]=='!') {
			if (self->text[self->position+1]=='=') {
				result = (Token*)new_OperatorToken(NotEqualTo, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else {
				result = (Token*)new_OperatorToken(ExclamationPoint, &(self->text[self->position]), 1);
				self->position += 1;
			}
		}
		else if (self->text[self->position]==':') {
			if (self->text[self->position+1]=='>') {
				result = (Token*)new_OperatorToken(OpenBracket, &(self->text[self->position]), 2);
				self->position += 2;
			}
			else {
				result = (Token*)new_OperatorToken(Colon, &(self->text[self->position]), 1);
				self->position += 1;
			}
		}
		else if (self->text[self->position]==';') {
			result = (Token*)new_OperatorToken(Semicolon, &(self->text[self->position]), 1);
			self->position += 1;
		}
		else if (self->text[self->position]=='{') {
			result = (Token*)new_OperatorToken(OpenBrace, &(self->text[self->position]), 1);
			self->position += 1;
		}
		else if (self->text[self->position]=='}') {
			result = (Token*)new_OperatorToken(CloseBrace, &(self->text[self->position]), 1);
			self->position += 1;
		}
		else if (self->text[self->position]==',') {
			result = (Token*)new_OperatorToken(Comma, &(self->text[self->position]), 1);
			self->position += 1;
		}
		else if (self->text[self->position]=='(') {
			result = (Token*)new_OperatorToken(OpenParenthesis, &(self->text[self->position]), 1);
			self->position += 1;
		}
		else if (self->text[self->position]==')') {
			result = (Token*)new_OperatorToken(CloseParenthesis, &(self->text[self->position]), 1);
			self->position += 1;
		}
		else if (self->text[self->position]=='[') {
			result = (Token*)new_OperatorToken(OpenBracket, &(self->text[self->position]), 1);
			self->position += 1;
		}
		else if (self->text[self->position]==']') {
			result = (Token*)new_OperatorToken(CloseBracket, &(self->text[self->position]), 1);
			self->position += 1;
		}
		else if (self->text[self->position]=='~') {
			result = (Token*)new_OperatorToken(Tilde, &(self->text[self->position]), 1);
			self->position += 1;
		}
		else if (self->text[self->position]=='?') {
			result = (Token*)new_OperatorToken(QuestionMark, &(self->text[self->position]), 1);
			self->position += 1;
		}
		else {
			//todo: throw some sort of error
			return NULL;
		}
		
		return result;
	}
}