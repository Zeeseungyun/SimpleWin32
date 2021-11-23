#include "config/config.h"

namespace zee {
namespace config {
	class test_config : public ini_base {
	public:
		test_config() : ini_base(file::paths::config_dir() + TEXT("test.json")) {
			const auto& temp = parsed_config();
			int d = 0;
		}
	} te;
}//namespace zee::config
}//namespace zee