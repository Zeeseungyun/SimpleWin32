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
	namespace fs = std::filesystem;

	config_base::config_base(const tstring& config_file_path, const tstring& config_dir, const tstring& config_name) noexcept
		: config_file_path_(config_file_path)
		, config_dir_(config_dir)
		, config_name_(config_name) {

	}

	bool config_base::load_impl() noexcept {
		if (!fs::exists(config_file_path())) {
			ZEE_LOG(warning, config_name().c_str() , TEXT("Config file does not exists.[%s]"), config_file_path().c_str());
			return false;
		}

		std::ifstream in(config_file_path());
		if (in) {
			json_ = json::parse(in, 0, is_allow_exception, ignore_comments);
			ZEE_LOG(warning, config_name().c_str(), TEXT("Config file loaded successfully.[%s]"), config_file_path().c_str());
			return true;
		}

		ZEE_LOG(warning, config_name().c_str(), TEXT("An error occurred while loading the config file.[%s]"), config_file_path().c_str());
		return false;
	}

	void config_base::save_impl() noexcept {
		if (!fs::exists(config_file_path())) {
			fs::create_directories(config_dir());
		}

		std::ofstream out(config_file_path());
		if (out) {
			out << raw_json();
			ZEE_LOG(normal, config_name().c_str(), TEXT("Config file saved successfully.[%s]"), config_file_path().c_str());
		} else {
			ZEE_LOG(warning, config_name().c_str(), TEXT("There was an error creating the config file.[%s]"), config_file_path().c_str());
		}
	}

}//namespace zee