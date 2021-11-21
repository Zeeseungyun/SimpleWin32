#pragma once
#include "../core/string.h"
#include "../file/path.h"
#include "../json/json.hpp"

namespace zee {
namespace config {

	class ini_base {
	public:
		ini_base(const tstring& path);
		
		const tstring& config_path() const noexcept { return config_path_; }
		const nlohmann::json& parsed_config() const noexcept { return parsed_config_; }

	private:
		nlohmann::json parsed_config_;
		tstring config_path_;
	};

}//namespace zee::config 
}//namespace zee
