#include "file_supports.h"
#include "log/log.h"
#include <Windows.h>

//https://docs.microsoft.com/en-us/windows/win32/fileio/file-attribute-constants
//https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfileattributesa

namespace zee {
namespace file {
	file_attribute_flags get_file_attribute(const tstring& path) noexcept {
		const DWORD result = GetFileAttributes(path.c_str());
		file_attribute_flags ret = (file_attribute_flags)file_attribute_flag::none;
		if (result != INVALID_FILE_ATTRIBUTES) {
			if (result & FILE_ATTRIBUTE_ARCHIVE) {
				if (result & FILE_ATTRIBUTE_DIRECTORY) {
					ret |= (file_attribute_flags)file_attribute_flag::directory;
				} else {
					ret |= (file_attribute_flags)file_attribute_flag::file;
				}
			}

			if (result & FILE_ATTRIBUTE_READONLY) {
				ret |= (file_attribute_flags)file_attribute_flag::readonly;
			}

			if (result & FILE_ATTRIBUTE_TEMPORARY) {
				ret |= (file_attribute_flags)file_attribute_flag::temporary;
			}

			if (result & FILE_ATTRIBUTE_HIDDEN) {
				ret |= (file_attribute_flags)file_attribute_flag::hidden;
			}

		} else {
			ZEE_LOG_DETAIL(log::verbose::warning, TEXT("Path was invalid. detail: [%s]"), log::last_error_to_string().c_str());
		}

		return ret;
	}

	bool exist(const tstring& path) noexcept {
		return get_file_attribute(path);
	}

	bool exist_file(const tstring& path) noexcept	{
		return get_file_attribute(path) & (file_attribute_flags)file_attribute_flag::file;
	}

	bool exist_directory(const tstring& path) noexcept {
		return get_file_attribute(path) & (file_attribute_flags)file_attribute_flag::directory;
	}

}//namespace zee::file
}//namespace zee