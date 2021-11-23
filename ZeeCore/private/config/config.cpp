#include "../../public/config/config.h"
#include "../../public/zlog/zlog.h"

#include <filesystem>
#include <fstream>
#include <vector>

namespace zee {
namespace config {
	namespace fs = std::filesystem;
	ini_base::ini_base(const tstring& path)	{
		if (!fs::exists(path)) {
			ZEE_LOG_DETAIL(warning, TEXT("config"), TEXT("path[%s] is not exist."), path.c_str());
			return;
		}

		int mode = 0;
		mode |= std::ios_base::in;
		
		std::basic_ifstream<char> in(path, mode);
		std::vector<char> buffer;
		const auto file_size = (size_t)fs::file_size(path);
		buffer.resize(file_size, 0);
		in.read(&buffer[0], file_size);
		parsed_config_ = buffer;
	}

}//namespace zee::config
}//namespace zee