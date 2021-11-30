//#include "inline_func.h"
inline int func() {
	return 3;
}

#include <iostream>
void print_source1() {
	std::cout << func() << std::endl;
}
