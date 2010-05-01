
#include <iostream>
#include "LogicInterpreter.h"

int main (int argc, char * const argv[]) {
    // insert code here...
  //  std::cout << "Hello, World!\n";

	int result = 0;
	
	LogicInterpreter li;
	try {
		li.processInput("~ ! ( ! x || y ) && ! ! z || x");
		li.printPostfix();
		li.eval(&result);		
	}
	catch (LogicInterpreterException e) {
		
	}
	
	return 0;
}
