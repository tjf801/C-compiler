#include <stdio.h>
#include <stdarg.h>
#include "lexer.h"
#include "nodes.h"
#include "parser.h"
#include "codegen.h"

static void codegen_error(char *format, ...) {
	va_list ap;
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	fprintf(stderr, "\n");
	exit(3);
}

static int stack_depth = 0;

static void push() {
	printf("  push %%rax\n");
	stack_depth++;
}
static void pop() {
	printf("  pop %%rdi\n");
	stack_depth--;
}

static void codegen_address(const NodeBase *node) {
	if (*node == VariableNodeBase) {
		int offset = (((VariableNode*)node)->identifier->value[0] - 'a' + 1) * 8;
		printf("  lea %d(%%rbp), %%rax\n", -offset);
		return;
	}
	codegen_error("not an lvalue");
}

void codegen_block(const NodeBase *node) {
	if (*node==BlockNodeBase) {
		BlockNode *block = (BlockNode*)node;
		for (int block_number = 0; block_number < block->block_length; ++block_number) {
			codegen_block_item(block->block_items[block_number]);
		}
	} else codegen_error("expected a block");
}

void codegen_block_item(const NodeBase *node) {
	return codegen_statement(node); //TODO
}

void codegen_statement(const NodeBase *node) {
	switch (*node) {
		case NullStatementNodeBase: {
			return;
		}
		case ExpressionStatementNodeBase: {
			codegen_expression(((ExpressionStatementNode*)node)->expression);
			return;
		}
		case ReturnStatementNodeBase: {
			if (((ReturnStatementNode*)node)->expression!=NULL) {
				codegen_expression(((ReturnStatementNode*)node)->expression);
			}
			printf("  jmp .L.return\n");
			return;
		}
		default: {
			codegen_error("invalid statement type");
		}
	}
}

void codegen_expression(const NodeBase *node) {
	switch (*node) {
		case IntegerNodeBase: {
			printf("  mov $%d, %%rax\n", ((IntegerNode*)node)->value);
			return;
		}
		case UnaryMinusNodeBase: {
			codegen_expression(((UnaryMinusNode*)node)->operand);
			printf("  neg %%rax\n");
			return;
		}
		case UnaryPlusNodeBase: {
			//unary plus is kinda useless lol; all it does is integral promotion and makes an expression an rvalue
			codegen_expression(((UnaryPlusNode*)node)->operand);
			return;
		}
		case AddNodeBase: {
			codegen_expression(((AddNode*)node)->right);
			push();
			codegen_expression(((AddNode*)node)->left);
			pop();
			printf("  add %%rdi, %%rax\n");
			return;
		}
		case SubtractNodeBase: {
			codegen_expression(((SubtractNode*)node)->right);
			push();
			codegen_expression(((SubtractNode*)node)->left);
			pop();
			printf("  sub %%rdi, %%rax\n");
			return;
		}
		case MultiplyNodeBase: {
			codegen_expression(((MultiplyNode*)node)->right);
			push();
			codegen_expression(((MultiplyNode*)node)->left);
			pop();
			printf("  imul %%rdi, %%rax\n");
			return;
		}
		case DivideNodeBase: {
			codegen_expression(((DivideNode*)node)->right);
			push();
			codegen_expression(((DivideNode*)node)->left);
			pop();
			printf("  cqo\n");
    		printf("  idiv %%rdi\n");
			return;
		}
		case EqualityNodeBase: {
			codegen_expression(((EqualityNode*)node)->right);
			push();
			codegen_expression(((EqualityNode*)node)->left);
			pop();
			printf("  cmp %%rdi, %%rax\n");
			printf("  sete %%al\n");
			printf("  movzb %%al, %%rax\n");
			return;
		}
		case InequalityNodeBase: {
			codegen_expression(((InequalityNode*)node)->right);
			push();
			codegen_expression(((InequalityNode*)node)->left);
			pop();
			printf("  cmp %%rdi, %%rax\n");
			printf("  setne %%al\n");
			printf("  movzb %%al, %%rax\n");
			return;
		}
		case LessThanNodeBase: {
			codegen_expression(((LessThanNode*)node)->right);
			push();
			codegen_expression(((LessThanNode*)node)->left);
			pop();
			printf("  cmp %%rdi, %%rax\n");
			printf("  setl %%al\n");
			printf("  movzb %%al, %%rax\n");
			return;
		}
		case LessThanOrEqualToNodeBase: {
			codegen_expression(((LessThanOrEqualToNode*)node)->right);
			push();
			codegen_expression(((LessThanOrEqualToNode*)node)->left);
			pop();
			printf("  cmp %%rdi, %%rax\n");
			printf("  setle %%al\n");
			printf("  movzb %%al, %%rax\n");
			return;
		}
		case GreaterThanNodeBase: {
			codegen_expression(((GreaterThanNode*)node)->right);
			push();
			codegen_expression(((GreaterThanNode*)node)->left);
			pop();
			printf("  cmp %%rdi, %%rax\n");
			printf("  setg %%al\n");
			printf("  movzb %%al, %%rax\n");
			return;
		}
		case GreaterThanOrEqualToNodeBase: {
			codegen_expression(((GreaterThanOrEqualToNode*)node)->right);
			push();
			codegen_expression(((GreaterThanOrEqualToNode*)node)->left);
			pop();
			printf("  cmp %%rdi, %%rax\n");
			printf("  setge %%al\n");
			printf("  movzb %%al, %%rax\n");
			return;
		}
		
		case VariableNodeBase: {
			codegen_address(node);
			printf("  mov (%%rax), %%rax\n");
			return;
		}
		case AssignNodeBase: {
			codegen_address(((AssignNode*)node)->left);
			push();
			codegen_expression(((AssignNode*)node)->right);
			pop();
			printf("  mov %%rax, (%%rdi)\n");;
			return;
		}
		default: {
			codegen_error("invalid expression type %d", *node);
		}
		
	}
}

void codegen(const NodeBase *node) {
	printf("  .globl main\n");
	printf("main:\n");
	printf("  push %%rbp\n");
	printf("  mov %%rsp, %%rbp\n");
	printf("  sub $208, %%rsp\n"); //makes space for the 26 variables
	codegen_block(node);
	printf(".L.return:\n");
	printf("  mov %%rbp, %%rsp\n");
	printf("  pop %%rbp\n");
	printf("  ret\n");
}