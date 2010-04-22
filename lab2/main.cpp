
#include <iostream>
#include "LogicInterpreter.h"

int main (int argc, char * const argv[]) {
    // insert code here...
  //  std::cout << "Hello, World!\n";

	LogicInterpreter li;
	li.processInput("x + y + z - a");
	li.printPostfix();
	
	return 0;
}
