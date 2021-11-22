#pragma once
#include "../core/string.h"
namespace zee {
namespace file {
	struct paths {
		static const tstring& exe_dir() { return get_paths().exe_dir_; }
		static const tstring& config_dir() { return get_paths().config_dir_; }
		static const tstring& log_dir() { return get_paths().log_dir_; }

	private:
		paths() = default;
		static const paths& get_paths();

	private:
		const tstring exe_dir_ = TEXT("./");
		const tstring config_dir_ = exe_dir_ + TEXT("config/");
		const tstring log_dir_ = exe_dir_ + TEXT("log/");
	};

}//namespace zee::file
}//namespace zee 