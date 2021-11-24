#include "../../public/config/config.h"
#include "../../public/zlog/zlog.h"

#include <filesystem>
#include <fstream>
#include <vector>

namespace zee {
namespace config {
#ifdef _DEBUG
	static constexpr bool is_allow_exception = true;
	static constexpr bool ignore_comments = false;
#else
	static constexpr bool is_allow_exception = false;
	static constexpr bool ignore_comments = true;
#endif

	namespace fs = std::filesystem;
	ini_base::ini_base(const tstring& load_file_name) : file_name_(load_file_name) {
		const tstring config_file_name = file::paths::config_dir() + file_name();
		if (!fs::exists(config_file_name)) {
			ZEE_LOG_DETAIL(warning, TEXT("config"), TEXT("file_name[%s] is not exists."), config_file_name.c_str());
			return;
		}

		std::ifstream in(config_file_name);//, std::ios_base::in);
		base_type::operator=(json::parse(in, nullptr, is_allow_exception, ignore_comments));
	}

	ini_base::~ini_base() {
		if (!fs::exists(file::paths::config_dir())) {
			fs::create_directories(file::paths::config_dir());
		}
		const tstring config_file_name = file::paths::config_dir() + file_name();
		std::ofstream out(config_file_name);//, std::ios_base::out);
		out << *this;
	}

}//namespace zee::config
}//namespace zee