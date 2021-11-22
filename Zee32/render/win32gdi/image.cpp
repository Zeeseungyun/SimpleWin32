#include "image.h"
#include "core/core.h"
//#include "application/application.h"
//#include "util/file_supports.h"
//#include "log/log.h"

#include <Windows.h>

namespace zee {
namespace win32gdi {
	void simple_bitmap_desc::clear() {
		size = math::vec2i::constants::zero;
		type = image_type::none;
		load_type = image_load_type::none;
		file_path.clear();
		file_path.shrink_to_fit();
	}

	tstring image_load_result_to_string(image_load_result result) noexcept {
		switch (result)
		{
		case zee::win32gdi::image_load_result::success:
			return TEXT("success.");
			break;
		case zee::win32gdi::image_load_result::failed_unknown_error:
			return TEXT("failed_unknown_error.");
			break;
		case zee::win32gdi::image_load_result::failed_not_exist_image:
			return TEXT("failed_not_exist_image.");
			break;
		case zee::win32gdi::image_load_result::failed_application_not_started:
			return TEXT("failed_application_not_started.");
			break;
		case zee::win32gdi::image_load_result::failed_application_window_handle_invalid:
			return TEXT("failed_application_window_handle_invalid.");
			break;
		case zee::win32gdi::image_load_result::failed_unknown_check_output_debug:
			return TEXT("failed_unknown_check_output_debug.");
			break;
		default:
			break;
		}

		return TEXT("failed_unknown_error.");
	}

	image_load_result load_image(handle_t& out_handle, const tstring& file_path) noexcept {
		out_handle = NULL;

		if (!application::is_application_started()) {
			return image_load_result::failed_application_not_started;
		}
		
		if (!file::exist_file(file_path)) {
			return image_load_result::failed_not_exist_image;
		}

		handle_t window_handle = application::get().window_handle();
		if (window_handle == NULL) {
			return image_load_result::failed_application_window_handle_invalid;
		}

		handle_t instance_handle = application::get().instance_handle();
		HBITMAP loaded_bitmap = (HBITMAP)LoadImage((HINSTANCE)instance_handle, file_path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		if (loaded_bitmap == NULL) {
			ZEE_LOG_DETAIL(log::verbose_type::warning, TEXT("load_image is failed. detail: [%s]"), log::last_error_to_string().c_str());
			return image_load_result::failed_unknown_check_output_debug;
		}

		out_handle = loaded_bitmap;
		return image_load_result::success;
	}
}//namespace zee::win32gdi 
}//namespace zee