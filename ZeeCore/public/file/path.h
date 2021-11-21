#pragma once
#include "../core/string.h"
namespace zee {
namespace file {
	struct paths {
		static const tstring exe_dir; // TEXT("./");
		static const tstring config_dir; // exe_dir + TEXT("config/");
		static const tstring log_dir; // exe_dir + TEXT("log/");
	};

}//namespace zee::file
}//namespace zee 