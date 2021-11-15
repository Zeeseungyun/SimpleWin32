#pragma once
#include "core/core.h"
namespace zee {
namespace file {
	
	enum class file_attribute_flag {
		none      = 0,
		file      = 1 << 0,
		directory = 1 << 1,
		readonly  = 1 << 2,
		temporary = 1 << 3,
		hidden    = 1 << 4,
	};

	typedef uint32 file_attribute_flags;

	file_attribute_flags get_file_attribute(const tstring& path) noexcept;
	bool exist(const tstring& path) noexcept;
	bool exist_file(const tstring& path) noexcept;
	bool exist_directory(const tstring& path) noexcept;
	
}//namespace zee::file
}//namespace zee