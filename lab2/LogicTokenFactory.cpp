/*
 *  LogicTokenFactory.cpp
 *  lab2
 *
 *  Created by Alexey Streltsow on 4/22/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */
#include "common.h"
#include "LogicTokenFactory.h"

#define MAKE_OBJECT(input, token, class) if ( !strcmp(input, token) ) return new class();

LogicTokenOperator*	LogicTokenFactory::createOperatorToken(const char *opToken) {
	MAKE_OBJECT(opToken, "+", LogicTokenOperatorPlus);
	MAKE_OBJECT(opToken, "-", LogicTokenOperatorMinus);
	MAKE_OBJECT(opToken, "*", LogicTokenOperatorMultiply);
	MAKE_OBJECT(opToken, "/", LogicTokenOperatorDiv);
	MAKE_OBJECT(opToken, "%", LogicTokenOperatorMod);	
	
	MAKE_OBJECT(opToken, ">", LogicTokenOperatorGreater);
	MAKE_OBJECT(opToken, "<", LogicTokenOperatorLess);
	MAKE_OBJECT(opToken, ">=", LogicTokenOperatorGreaterEqual);
	MAKE_OBJECT(opToken, "<=", LogicTokenOperatorLessEqual);
	
	MAKE_OBJECT(opToken, "==", LogicTokenOperatorEqual);
	MAKE_OBJECT(opToken, "!=", LogicTokenOperatorNotEqual);
	
	MAKE_OBJECT(opToken, "&", LogicTokenOperatorBitwiseAND);
	MAKE_OBJECT(opToken, "|", LogicTokenOperatorBitwiseOR);
	MAKE_OBJECT(opToken, "^", LogicTokenOperatorBitwiseXOR);

	MAKE_OBJECT(opToken, "&&", LogicTokenOperatorLogicAND);
	MAKE_OBJECT(opToken, "||", LogicTokenOperatorLogicOR);
	
	MAKE_OBJECT(opToken, "<<", LogicTokenOperatorShiftLeft);
	MAKE_OBJECT(opToken, ">>", LogicTokenOperatorShiftRight);
	
	return NULL;
}


LogicTokenOperator*	LogicTokenFactory::createUnaryOperatorToken(const char *opToken) {
	MAKE_OBJECT(opToken, "!", LogicTokenOperatorUnaryNOT);
	MAKE_OBJECT(opToken, "~", LogicTokenOperatorUnaryINV);
	
	return NULL;
}

LogicTokenVariable* LogicTokenFactory::createVariableToken(const char *varToken) {	
	return new LogicTokenVariable(varToken);
}

LogicToken*	LogicTokenFactory::createToken(const char *token, bool isUnary) {
	LogicToken *tokenobj = isUnary ? createUnaryOperatorToken(token) : createOperatorToken(token) ;

	if ( *token == '(' )
		tokenobj = new LogicTokenParenthesisOpen();
	
	if ( *token == ')' )
		tokenobj = new LogicTokenParenthesisClose();
	
	if ( !tokenobj ) {
		tokenobj = createVariableToken( token );
	}
	
	return tokenobj;
}
