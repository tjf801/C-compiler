#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nodes.h"

IntegerNode *new_IntegerNode(long long value) {
	IntegerNode * I = (IntegerNode*) malloc( sizeof(IntegerNode) );
	I->type = IntegerNodeBase;
	I->value = value;
	return I;
}
FloatNode *new_FloatNode(double value) {
	FloatNode * F = (FloatNode*) malloc( sizeof(FloatNode) );
	F->type = FloatNodeBase;
	F->value = value;
	return F;
}

UnaryMinusNode *new_UnaryMinusNode(NodeBase *operand) {
	UnaryMinusNode *M = malloc(sizeof(AddNode));
	M->type = UnaryMinusNodeBase;
	M->operand = operand;
	return M;
}
UnaryPlusNode *new_UnaryPlusNode(NodeBase *operand) {
	UnaryPlusNode *M = malloc(sizeof(AddNode));
	M->type = UnaryPlusNodeBase;
	M->operand = operand;
	return M;
}

AddNode *new_AddNode(NodeBase *left, NodeBase *right) {
	AddNode *A = malloc(sizeof(AddNode));
	A->type = AddNodeBase;
	A->left = left;
	A->right = right;
	return A;
}
SubtractNode *new_SubtractNode(NodeBase *left, NodeBase *right) {
	SubtractNode *S = (SubtractNode*) malloc(sizeof(SubtractNode));
	S->type = SubtractNodeBase;
	S->left = left;
	S->right = right;
	return S;
}
MultiplyNode *new_MultiplyNode(NodeBase *left, NodeBase *right) {
	MultiplyNode *M = (MultiplyNode*) malloc(sizeof(MultiplyNode));
	M->type = MultiplyNodeBase;
	M->left = left;
	M->right = right;
	return M;
}
DivideNode *new_DivideNode(NodeBase *left, NodeBase *right) {
	DivideNode *D = (DivideNode*) malloc(sizeof(DivideNode));
	D->type = DivideNodeBase;
	D->left = left;
	D->right = right;
	return D;
}
ModuloNode *new_ModuloNode(NodeBase *left, NodeBase *right) {
	ModuloNode *M = (ModuloNode*) malloc(sizeof(ModuloNode));
	M->type = ModuloNodeBase;
	M->left = left;
	M->right = right;
	return M;
}

EqualityNode *new_EqualityNode(NodeBase *left, NodeBase *right) {
	EqualityNode *E = malloc(sizeof(EqualityNode));
	E->type = EqualityNodeBase;
	E->right = right;
	E->left = left;
	return E;
}
InequalityNode *new_InequalityNode(NodeBase *left, NodeBase *right) {
	InequalityNode *I = malloc(sizeof(InequalityNode));
	I->type = InequalityNodeBase;
	I->left = left;
	I->right = right;
	return I;
}
LessThanOrEqualToNode *new_LessThanOrEqualToNode(NodeBase *left, NodeBase *right) {
	LessThanOrEqualToNode *L = malloc(sizeof(LessThanOrEqualToNode));
	L->type = LessThanOrEqualToNodeBase;
	L->left = left;
	L->right = right;
	return L;
}
GreaterThanOrEqualToNode *new_GreaterThanOrEqualToNode(NodeBase *left, NodeBase *right) {
	GreaterThanOrEqualToNode *G = malloc(sizeof(GreaterThanOrEqualToNode));
	G->type = GreaterThanOrEqualToNodeBase;
	G->left = left;
	G->right = right;
	return G;
}
LessThanNode *new_LessThanNode(NodeBase *left, NodeBase *right) {
	LessThanNode *L = malloc(sizeof(LessThanNode));
	L->type = LessThanNodeBase;
	L->left = left;
	L->right = right;
	return L;
}
GreaterThanNode *new_GreaterThanNode(NodeBase *left, NodeBase *right) {
	GreaterThanNode *G = malloc(sizeof(GreaterThanNode));
	G->type = GreaterThanNodeBase;
	G->left = left;
	G->right = right;
	return G;
}


ExpressionStatementNode *new_ExpressionStatementNode(NodeBase *expression) {
	ExpressionStatementNode *E = malloc(sizeof(ExpressionStatementNode));
	E->type = ExpressionStatementNodeBase;
	E->expression = expression;
	return E;
}