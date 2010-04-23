/*
 *  LogicTokenFactory.h
 *  lab2
 *
 *  Created by Alexey Streltsow on 4/22/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _logic_token_factory_h
#define _logic_token_factory_h
#include "LogicToken.h"

class LogicTokenFactory {
public:
	static LogicTokenOperator*		createOperatorToken(const char *opToken);
	static LogicTokenVariable*		createVariableToken(const char *varToken);
	static LogicToken*						createToken(const char *token, bool isUnary);
	
	static LogicTokenOperator*		createUnaryOperatorToken(const char *opToken);
	
private:
	LogicTokenFactory();
};

#endif //_logic_token_factory_h