/*
 *  LogicOperators.h
 *  lab2
 *
 *  Created by Alexey Streltsow on 4/23/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _logic_token_operators_h
#define _logic_token_operators_h

///////////////////////////////////////////////////////////////////////////////
// Unary
// Precendence = 2
class LogicTokenOperatorUnaryNOT : public LogicTokenOperatorUnary {
public:
	const char* operatorString() { return "!"; }
	int	precendence() { return 2; }
	
	int eval(int val1) {
		return !val1;
	}
};

class LogicTokenOperatorUnaryINV : public LogicTokenOperatorUnary {
public:
	const char* operatorString() { return "~"; }
	int	precendence() { return 2; }
	
	int eval(int val1) {
		return ~val1;
	}
};

///////////////////////////////////////////////////////////////////////////////
// Multiplication, division, modulo
// Precendence = 3
class LogicTokenOperatorMultiply : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "*"; }
	int	precendence() { return 3; }
	
	int eval(int val1, int val2) {
		return val1*val2;
	}
};

class LogicTokenOperatorDiv : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "/"; }
	int	precendence() { return 3; }
	
	int eval(int val1, int val2) {
		if ( val2 == 0 ) {
			throw new LogicInterpreterException("Division by zero");
		}
		return val1/val2;
	}
};


class LogicTokenOperatorMod : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "%"; }
	int	precendence() { return 3; }
	
	int eval(int val1, int val2) {
		return val1%val2;
	}
};

///////////////////////////////////////////////////////////////////////////////
// Addition and subtraction
// Precendence = 4
class LogicTokenOperatorPlus : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "+"; }
	int	precendence() { return 4; }	
	
	int eval(int val1, int val2) {
		return val1+val2;
	}
};

class LogicTokenOperatorMinus : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "-"; }
	int precendence() { return 4; }
	
	int eval(int val1, int val2) {
		return val1/val2;
	}
};

///////////////////////////////////////////////////////////////////////////////
// Bitwise shifts
// Precendence = 5
class LogicTokenOperatorShiftLeft : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "<<"; }
	int	precendence() { return 5; }	
	
	int eval(int val1, int val2) {
		return val1<<val2;
	}
};

class LogicTokenOperatorShiftRight : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return ">>"; }
	int precendence() { return 5; }
	
	int eval(int val1, int val2) {
		return val1>>val2;
	}
};

///////////////////////////////////////////////////////////////////////////////
// Comparisons: less-than
// Precendence = 6
class LogicTokenOperatorLess : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "<"; }
	int	precendence() { return 6; }	
	
	int eval(int val1, int val2) {
		return val1<val2;
	}
};

class LogicTokenOperatorGreater : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return ">"; }
	int precendence() { return 5; }
	
	int eval(int val1, int val2) {
		return val1>val2;
	}
};

class LogicTokenOperatorLessEqual : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "<="; }
	int	precendence() { return 5; }	
	
	int eval(int val1, int val2) {
		return val1<=val2;
	}
};

class LogicTokenOperatorGreaterEqual : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return ">="; }
	int precendence() { return 5; }
	
	int eval(int val1, int val2) {
		return val1>=val2;
	}
};

///////////////////////////////////////////////////////////////////////////////
// Comparisons: Equality
// Precendence = 6
class LogicTokenOperatorEqual : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "=="; }
	int	precendence() { return 6; }	
	
	int eval(int val1, int val2) {
		return val1==val2;
	}
};

class LogicTokenOperatorNotEqual : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "!="; }
	int precendence() { return 6; }
	
	int eval(int val1, int val2) {
		return val1!=val2;
	}
};

///////////////////////////////////////////////////////////////////////////////
// Bitwise operators
// Precendence = 6+
class LogicTokenOperatorBitwiseAND : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "&"; }
	int precendence() { return 7; }
	
	int eval(int val1, int val2) {
		return val1&val2;
	}
};

class LogicTokenOperatorBitwiseXOR : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "^"; }
	int precendence() { return 8; }
	
	int eval(int val1, int val2) {
		return val1^val2;
	}
};

class LogicTokenOperatorBitwiseOR : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "|"; }
	int	precendence() { return 9; }
	
	int eval(int val1, int val2) {
		return val1|val2;
	}
};


///////////////////////////////////////////////////////////////////////////////
// Logic elements
// Precendence 10+
class LogicTokenOperatorLogicAND : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "&&"; }
	int precendence() { return 10; }
	
	int eval(int val1, int val2) {
		return val1 && val2;
	}
};

class LogicTokenOperatorLogicOR : public LogicTokenOperatorBinary {
public:
	const char* operatorString() { return "||"; }
	int precendence() { return 11; }
	
	int eval(int val1, int val2) {
		return val1 || val2;
	}
};



#endif