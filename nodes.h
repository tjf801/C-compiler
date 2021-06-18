#ifndef NODES_H
#define NODES_H

typedef enum NodeBase {
	NullStatementNodeBase = 0,
	BlockNodeBase,
	ExpressionStatementNodeBase,
	ReturnStatementNodeBase,
	
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
	
	EqualityNodeBase,
	InequalityNodeBase,
	LessThanOrEqualToNodeBase,
	GreaterThanOrEqualToNodeBase,
	LessThanNodeBase,
	GreaterThanNodeBase,
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

typedef struct EqualityNode {
	NodeBase type;
	NodeBase *left;
	NodeBase *right;
} EqualityNode;
EqualityNode *new_EqualityNode(NodeBase *left, NodeBase *right);
typedef struct InequalityNode {
	NodeBase type;
	NodeBase *left;
	NodeBase *right;
} InequalityNode;
InequalityNode *new_InequalityNode(NodeBase *left, NodeBase *right);
typedef struct LessThanOrEqualToNode {
	NodeBase type;
	NodeBase *left;
	NodeBase *right;
} LessThanOrEqualToNode;
LessThanOrEqualToNode *new_LessThanOrEqualToNode(NodeBase *left, NodeBase *right);
typedef struct GreaterThanOrEqualToNode {
	NodeBase type;
	NodeBase *left;
	NodeBase *right;
} GreaterThanOrEqualToNode;
GreaterThanOrEqualToNode *new_GreaterThanOrEqualToNode(NodeBase *left, NodeBase *right);
typedef struct LessThanNode {
	NodeBase type;
	NodeBase *left;
	NodeBase *right;
} LessThanNode;
LessThanNode *new_LessThanNode(NodeBase *left, NodeBase *right);
typedef struct GreaterThanNode {
	NodeBase type;
	NodeBase *left;
	NodeBase *right;
} GreaterThanNode;
GreaterThanNode *new_GreaterThanNode(NodeBase *left, NodeBase *right);


typedef struct ExpressionStatementNode {
	NodeBase type;
	NodeBase *expression;
} ExpressionStatementNode;
ExpressionStatementNode *new_ExpressionStatementNode(NodeBase *expression);

typedef struct ReturnStatementNode {
	NodeBase type;
	NodeBase *expression;
} ReturnStatementNode;
ReturnStatementNode *new_ReturnStatementNode(NodeBase* expression);

typedef struct BlockNode {
	NodeBase type;
	size_t max_size;
	size_t block_length;
	NodeBase **block_items;
} BlockNode;
BlockNode *new_BlockNode(size_t initial_size);
void BlockNode_push_BlockItem(BlockNode *self, NodeBase *block_item);

#endif