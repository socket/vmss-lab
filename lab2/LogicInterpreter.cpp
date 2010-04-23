/*
 *  LogicInterpreter.cpp
 *  lab2
 *
 *  Created by Alexey Streltsow on 4/22/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#include "LogicInterpreter.h"

LogicInterpreter::LogicInterpreter() {
	
}

LogicInterpreter::LogicInterpreter(const char *inputString) {
	processInput( inputString );
}

LogicInterpreter::~LogicInterpreter() {
	reset();
}

void LogicInterpreter::reset() {
	std::vector<LogicToken *> :: iterator it_end = _postfixData.end();
	std::vector<LogicToken *> :: iterator it = _postfixData.begin();
	
	for( ; it != it_end; ++it ) {
		delete *it;
	}
	
	_postfixData.clear();
}

void LogicInterpreter::processInput(const char *inputString) THROWS {
	static const char* separators = " \t\r\n";
	
	assert( inputString != NULL );
	if ( ! inputString ) { 
		throw LogicInterpreterException("Input string cannot be nil");
	}
	
	reset();
	
	// copy the string so the initial one will remain intact
	char *pTokenString = (char *)malloc(strlen(inputString) + 16);
	char *pAlloc = pTokenString;
	
	strcpy(pTokenString, inputString);
	
	// tokenize the string to extract Operators
	pTokenString = strtok(pTokenString, separators);
	
	std::stack<LogicToken*> opStack;
	
	bool isUnary = true; // first operand should be unary
	
	while (pTokenString != NULL) {
		LogicToken *pToken = LogicTokenFactory::createToken( pTokenString, isUnary );
		
		if ( ! pToken ) {
			free(pAlloc);
			throw LogicInterpreterException("Unable to parse token");
		}
		
		if ( pToken->isValue() || pToken->isVariable() ) {	 // variables and raw values
			_postfixData.push_back( pToken );
			
			// unary operator handling
			while ( opStack.size() > 0 ) {
				LogicTokenOperator *pOperator = dynamic_cast<LogicTokenOperator*> ( pToken );
				
				if ( pOperator && pOperator->isUnary() ) {
					_postfixData.push_back( pOperator );
					opStack.pop();
				}
				else {
					break;
				}
			}
			
			isUnary = false;
		}
		else if ( pToken->isParenthesisOpen() ) {
			opStack.push( pToken );
			isUnary = true;
		}
		else if ( pToken->isParenthesisClose() ) {
			isUnary = false;
			while( opStack.size() > 0 ) {
				LogicToken *stackToken = opStack.top();
				if ( ! stackToken->isParenthesisOpen() && opStack.size() == 0 ) {
					free(pAlloc);
					throw LogicInterpreterException("Invalid parenthesis in expression");
				}
				else if ( ! stackToken->isParenthesisOpen() ) {
					_postfixData.push_back( stackToken );
					opStack.pop();
				}
				else {
					opStack.pop();
					break;
				}
			}
		}
		else if ( pToken->isOperator() ) {
			LogicTokenOperator *pOperator = dynamic_cast<LogicTokenOperator*> ( pToken );
			assert( pOperator );
			
			if ( pOperator->isBinary() ) { // just push unary operators as-is without precendence ???
				while( opStack.size() > 0 ) {
					LogicTokenOperator *pTopStackOperator = dynamic_cast<LogicTokenOperator *>( opStack.top() );
					
					if ( pTopStackOperator && ( pOperator->isLeftAssociative() && pOperator->precendence() > pTopStackOperator->precendence() ) ||
							 ( pOperator->isRightAssociative() && pOperator->precendence() >= pTopStackOperator->precendence() )) {
						_postfixData.push_back( opStack.top() );
						opStack.pop();
					}
					else {
						break;
					}
				}
			}
			
			opStack.push( pOperator );
			isUnary = true;
		}

		pTokenString = strtok(NULL, separators);
	}

	while ( opStack.size() > 0 ) {
		LogicToken *pTopToken = opStack.top();
		if ( pTopToken->isParenthesis() ) {
			free(pAlloc);
			throw LogicInterpreterException("Mismatched parenthesis left in stack");
		}
		_postfixData.push_back( opStack.top() );
		opStack.pop();
	}
	
	free(pAlloc);
}

void LogicInterpreter::printPostfix() {
	std::vector<LogicToken *> :: iterator it_end = _postfixData.end();
	std::vector<LogicToken *> :: iterator it = _postfixData.begin();
	
	for( ; it != it_end; ++it ) {
		printf("%s ", (*it)->toString() );
	}	
}

void LogicInterpreter::getVariables(char **vars, int **values, int &count) {
	
}

void LogicInterpreter::setVariables(char **vars, int **values, int &count) {
	
}
