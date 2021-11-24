#include "ini_test.h"

zee::test::test_config::test_config()
	: config::ini_base(TEXT("test_config.ini")) {
	if (!is_loaded()) {
		std::string default_test_config =
		#include "default_config/default_test_config.ini"
		;

		ZEE_LOG(warning, TEXT("config"), TEXT("load from default test config.ini %s"), to_tstring(default_test_config).c_str());
		json default_config = json::parse(default_test_config.begin(), default_test_config.end());
		swap(default_config);
	}
}
