#include <stdio.h>
#include "lexer.h"
#include "nodes.h"
#include "parser.h"
#include "codegen.h"

static int stack_depth = 0;

static void push() {
	printf("  push %%rax\n");
	stack_depth++;
}
static void pop() {
	printf("  pop %%rdi\n");
	stack_depth--;
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
		
	}
}

void codegen(const NodeBase *node) {
	printf("  .globl main\n");
	printf("main:\n");
	codegen_expression(node);
	printf("  ret\n");
}