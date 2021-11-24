#pragma once
#include "../core/string.h"
#include "../file/path.h"
#include "../json/json.h"

namespace zee {
namespace config {

	class ini_base : public json {
	private:
		typedef json base_type;

	protected:
		ini_base(const tstring& load_file_name);
		~ini_base();

	public:
		const tstring& file_name() const noexcept { return file_name_; }
		bool is_loaded() const { return !is_null(); }

	private:
		tstring file_name_;
	};

}//namespace zee::config 
}//namespace zee
