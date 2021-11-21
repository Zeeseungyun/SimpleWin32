#pragma once
#include "../core/string.h"
#include "../file/path.h"
namespace zee {
namespace config {

	class ini_base {
	public:
		ini_base(const tstring& path);
		
		const tstring& config_path() const noexcept;

	private:
		tstring config_path_;
	};

}//namespace zee::config 
}//namespace zee
