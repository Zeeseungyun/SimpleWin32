#include <iostream>
#include "json/json.h"
#include "math/vec/vec.h"
#include "math/to_string.h"
#include "ini_test.h"
#include "zlog/zlog.h"
#include "shape/rect.h"
#include "shape/to_string.h"
#include <core/core.h>
#include <zconfig/zconfig.h>
#include <util/helper_macros.h>
#include <chrono>
#include <thread>
#include "inline_func.h"
#include <iostream>
#include <functional>

struct A {
	A() {
		std::cout << "sibla";
	}
};

struct B : A {

};

int main()
{
	std::function<void(A*&)> ff = [](A*& p){
		new (p)B();
	};

	return 0;
}
