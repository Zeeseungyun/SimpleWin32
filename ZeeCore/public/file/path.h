#pragma once
#include "../core/string.h"
namespace zee {
namespace file {
	struct paths {

		//./
		static const tstring& exe_dir() { return get_paths().exe_dir_; }
		//exe_dir/config/
		static const tstring& config_dir() { return get_paths().config_dir_; }
		//exe_dir/log/
		static const tstring& log_dir() { return get_paths().log_dir_; }

	private:
		paths() = default;
		static const paths& get_paths();

	private:
		const tstring exe_dir_ = TEXT("./");
		const tstring config_dir_ = exe_dir_ + TEXT("config/");
		const tstring log_dir_ = exe_dir_ + TEXT("log/");
	};

	bool exists(const tstring& file_path) noexcept;

	template<typename T>
	tstring type_to_path() noexcept {
		const tstring class_names[] = { TEXT("struct"), TEXT("class"), TEXT(" ") };

		tstring ret = to_tstring(typeid(T).name());

		size_t found_offset = tstring::npos;
		for (const auto& elem : class_names) {
			found_offset = ret.find(elem);
			if (found_offset != tstring::npos) {
				ret.erase(found_offset, elem.size());
			}
		}

		while ((found_offset = ret.find(TEXT("::"))) != tstring::npos) {
			ret.replace(found_offset, 2, TEXT("/"));
		}

		return ret;
	}

}//namespace zee::file
}//namespace zee 