#ifndef NODES_H
#define NODES_H

typedef enum NodeBase {
	NullStatementNodeBase = 0,
	ExpressionStatementNodeBase,
	
	IntegerNodeBase,
	FloatNodeBase,
	VariableNodeBase,
	
	UnaryMinusNodeBase,
	UnaryPlusNodeBase,
	
	AddNodeBase,
	SubtractNodeBase,
	MultiplyNodeBase,
	DivideNodeBase,
	ModuloNodeBase,
} NodeBase;

typedef struct IntegerNode {
	NodeBase type;
	long long value;
} IntegerNode;
IntegerNode *new_IntegerNode(long long value);

typedef struct FloatNode {
	NodeBase type;
	double value;
} FloatNode;
FloatNode *new_FloatNode(double value);

typedef struct UnaryMinusNode {
	NodeBase type;
	NodeBase *operand; 
} UnaryMinusNode;
UnaryMinusNode *new_UnaryMinusNode(NodeBase *operand);

typedef struct UnaryPlusNode {
	NodeBase type;
	NodeBase *operand; 
} UnaryPlusNode;
UnaryPlusNode *new_UnaryPlusNode(NodeBase *operand);

typedef struct AddNode {
	NodeBase type;
	NodeBase *left;
	NodeBase *right;
} AddNode;
AddNode *new_AddNode(NodeBase *left, NodeBase *right);

typedef struct SubtractNode {
	NodeBase type;
	NodeBase *left;
	NodeBase *right;
} SubtractNode;
SubtractNode *new_SubtractNode(NodeBase *left, NodeBase *right);

typedef struct MultiplyNode {
	NodeBase type;
	NodeBase *left;
	NodeBase *right;
} MultiplyNode;
MultiplyNode *new_MultiplyNode(NodeBase *left, NodeBase *right);

typedef struct DivideNode {
	NodeBase type;
	NodeBase *left;
	NodeBase *right;
} DivideNode;
DivideNode *new_DivideNode(NodeBase *left, NodeBase *right);

typedef struct ModuloNode {
	NodeBase type;
	NodeBase *left;
	NodeBase *right;
} ModuloNode;
ModuloNode *new_ModuloNode(NodeBase *left, NodeBase *right);



#endif