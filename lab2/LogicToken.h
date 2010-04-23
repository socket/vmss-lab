/*
 *  LogicToken.h
 *  lab2
 *
 *  Created by Alexey Streltsow on 4/22/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _logic_token_h
#define _logic_token_h

class LogicToken {
public:
	virtual bool isValue() {
		return false;
	}
	
	virtual bool isParenthesis() {
		return false;
	}
	
	virtual bool isParenthesisOpen() {
		return false;
	}
	
	virtual bool isParenthesisClose() {
		return false;
	}
	
	virtual bool isOperator() { 
		return false; 
	}
	
	virtual bool isVariable() { 
		return false; 
	}
	
	virtual const char* toString() {
		return "UNK";
	}
	
protected:
	LogicToken() { };
	
public:
	virtual ~LogicToken() {}
};

class LogicInterpreterException {
	std::string _error;
	
public:
	LogicInterpreterException(const char* error) {
		if ( error != NULL ) {
			_error = error;
		}
	}
};

class LogicTokenValue : public LogicToken {
	int _value;
	
public:
	LogicTokenValue(int value = 0) { _value = 0; }
	
	int value() { return _value; }
	
	const char* toString() { return "CONST"; }
	
	bool isValue() {
		return true;
	}
};

class LogicTokenVariable : public LogicTokenValue {
	std::string _name;
	
public:
	LogicTokenVariable(const char* name, int value = 0) : LogicTokenValue(value) {
		_name = name;
	};
	
	bool isVariable() { 
		return true; 
	}
	
	const char* name() { return _name.c_str(); }
	
	const char* toString() { return name(); }
};

////////////////////////////////////////////////////////////////////////////////////////
// Operators

class LogicTokenOperator : public LogicToken {
public:
	const char * toString() { return operatorString(); }
	
	virtual const char * operatorString() { return "OP?"; };
	
	virtual bool isUnary() { return false; }
	virtual bool isBinary() { return false; }
	
	virtual bool isLeftAssociative() { return true; }
	virtual bool isRightAssociative() { return false; }
	virtual int	 precendence() { return 1; }
	
	bool isOperator() { 
		return true; 
	}
};

class LogicTokenOperatorBinary : public LogicTokenOperator {
public:
	bool isBinary() { return true; }
	
	int eval(int val1, int val2);
};

class LogicTokenOperatorUnary : public LogicTokenOperator {
public:
	bool isUnary() { return true; }
	
	int eval(int val1);
};

#include "LogicTokenOperators.h"

////////////////////////////////////////////////////////////////////////////////////////
// Parenthesis ('(' and ')')

class LogicTokenParenthesis : public LogicToken {
protected:
	LogicTokenParenthesis() { }
	
public:
	bool isParenthesis() { return true; }
};

class LogicTokenParenthesisOpen: public LogicTokenParenthesis {
public:
	LogicTokenParenthesisOpen() {}
	bool isParenthesisOpen() { return true; }	
};

class LogicTokenParenthesisClose: public LogicTokenParenthesis {
public:
	LogicTokenParenthesisClose() {}
	bool isParenthesisClose() { return true; }	
};

#endif
