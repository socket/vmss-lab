/*
 *  yaul_codegen.h
 *  lab11
 *
 *  Created by Alexey Streltsow on 5/9/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */
#ifndef _yaul_codegen_h
#define _yaul_codegen_h

#include <vector>
#include <map>

#include "yaul_parser.h"
#include "yaul_opcodes.h"


class CodeGenException {
	std::string _error;
	
public:
	CodeGenException(const char* error) { _error = error; }
	const char* error() { return _error.c_str(); }
};

class CodeBlock {
public:
	std::map<std::string, int> _locals;
	
	int _push_count;
	
	CodeBlock() : _push_count(0) { }
	
	bool hasLocal(const char *local) {
		std::string lc = local;
		return _locals.find(lc) != _locals.end();
	}
	
	void addLocal(const char *local) {
		std::string lc = local;
		_locals[lc] = 1;
	}
};

class CodeGen {
	
	std::vector<yaul_op*>		_code;
	std::stack<CodeBlock>		_blocks;
	
public:
	CodeGen();
	~CodeGen();
	
	bool generate(ParseNode *node, yaul_op **code, int *count = 0);
	void print(FILE *f);
	
	
private:
	void throwError(const char* err, ...);

	void genChunk(ParseNode *node, ParseNode *parent);
	void genBlock(ParseNode *node, ParseNode *parent);
	void genStatement(ParseNode *node, ParseNode *parent);
	void genExp(ParseNode *node, ParseNode *parent);
	void genFuncCall(ParseNode *node, ParseNode *parent);
	
	yaul_op* addOp(yaul_opcode opcode, long arg = NULL);
	
	int calcJumpPos(yaul_op* op);
	
	CodeBlock* getCodeBlock();
	
};

#endif