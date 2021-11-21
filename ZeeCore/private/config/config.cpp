#include "..\..\public\config\config.h"
#include <filesystem>
#include <fstream>

namespace zee {
namespace config {
	namespace fs = std::filesystem;
	ini_base::ini_base(const tstring& path)	{
		if (!fs::exists(path)) {
			assert(0);
		}

		TCHAR buffer[512];
		std::basic_ifstream<TCHAR> in(path);

		if (in) {
			tstring str_buffer;
			while (in.read(buffer, std::size(buffer))) {
				str_buffer += buffer;
			}
			parsed_config_ = str_buffer;
		}
	}

}//namespace zee::config
}//namespace zee