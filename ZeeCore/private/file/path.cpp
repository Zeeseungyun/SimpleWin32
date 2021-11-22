#include "../../public/file/path.h"
#include <memory>
namespace zee {
namespace file {

	const paths& paths::get_paths() {
		static std::unique_ptr<paths> paths_instance;
		if (!paths_instance) {
			paths_instance = std::unique_ptr<paths>(new paths);
		}
		return *paths_instance;
	}

}//namespace zee::file
}//namespace zee 