#pragma once
#include "zconfig/zconfig.h"
#include "zlog/zlog.h"



namespace zee {
namespace test {
	class test_config : public config::ini_base {

	public:
		test_config();
	};

}//namespace zee::test
}//namespace zee