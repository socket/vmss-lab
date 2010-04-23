/*
 *  LogicInterpreter.h
 *  lab2
 *
 *  Created by Alexey Streltsow on 4/22/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */
#ifndef _logic_interpreter_h
#define _logic_interpreter_h

#include "common.h"
#include "LogicToken.h"
#include "LogicTokenFactory.h"

typedef int LI_RESULT;

class LogicInterpreter {
private:
	std::vector<LogicToken*> _postfixData;
	
public:
	// constructors
	LogicInterpreter();
	LogicInterpreter(const char *inputString);
	
	// destructor
	virtual ~LogicInterpreter();
	
	// resets internal storage
	void reset();
	
	// processes input string to internal storage
	void processInput(const char *inputString) THROWS;
	
	bool evaluate(int &result);
	
	void printPostfix();
	
	void getVariables(char **vars, int **values, int &count);
	void setVariables(char **vars, int **values, int &count);
};

#endif
