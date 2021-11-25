#include "../../public/zconfig/zconfig.h"
#include "../../public/zlog/zlog.h"

#include <filesystem>
#include <fstream>
#include <vector>

#ifdef _DEBUG
static constexpr bool is_allow_exception = true;
static constexpr bool ignore_comments = false;
#else
static constexpr bool is_allow_exception = false;
static constexpr bool ignore_comments = true;
#endif

namespace zee {
namespace config {
	namespace fs = std::filesystem;

namespace impl {
	config_base_impl::config_base_impl(const tstring& ini_file_name, const tstring& config_dir, const tstring& config_name) noexcept
		: ini_file_name_(ini_file_name)
		, config_dir_(config_dir)
		, config_name_(config_name) {

	}

	bool config_base_impl::load_impl() noexcept {
		const tstring config_file_path = config_dir() + ini_file_name();
		if (!fs::exists(config_file_path)) {
			ZEE_LOG_DETAIL(warning, config_name().c_str() , TEXT("config file[%s] is not exists."), config_file_path.c_str());
			return false;
		}

		std::ifstream in(config_file_path);
		if (in) {
			in >> json_;
		}

		return true;
	}

	void config_base_impl::save_impl() noexcept {
		if (!fs::exists(config_dir() + ini_file_name())) {
			fs::create_directories(file::paths::config_dir() + ini_file_name());
		}

		std::ofstream out(file::paths::config_dir() + ini_file_name());
		if (out) {
			out << json_;
		}
	}

}//namespace zee::config::impl
}//namespace zee::config
}//namespace zee