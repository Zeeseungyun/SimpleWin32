//#include "inline_func.h"
inline int func() {
	return 1;
}

#include <iostream>
void print_source2() {
	std::cout << func() << std::endl;
}
