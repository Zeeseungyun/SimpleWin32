#include "../../public/file/path.h"
#include <memory>
#include <filesystem>
namespace yjj {
namespace file {
	namespace fs = std::filesystem;
	const paths& paths::get_paths() {
		static std::unique_ptr<paths> paths_instance;
		if (!paths_instance) {
			paths_instance = std::unique_ptr<paths>(new paths);
		}
		return *paths_instance;
	}

	bool exists(const tstring& file_path) noexcept {
		return fs::exists(file_path);
	}
}//namespace yjj::file
}//namespace yjj 