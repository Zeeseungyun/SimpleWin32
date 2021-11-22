#include "../../public/file/path.h"
namespace zee {
namespace file {
	const tstring paths::exe_dir = TEXT("./");
	const tstring paths::config_dir = exe_dir + TEXT("config/");
	const tstring paths::log_dir = exe_dir + TEXT("log/");

}//namespace zee::file
}//namespace zee 