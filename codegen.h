#ifndef CODEGEN_H
#define CODEGEN_H
#include "lexer.h"
#include "nodes.h"
#include "parser.h"

void codegen(const NodeBase *node);
void codegen_statement(const NodeBase *node);
void codegen_expression(const NodeBase *node);
void codegen_block_item(const NodeBase *node);
void codegen_block(const NodeBase *node);

#endif