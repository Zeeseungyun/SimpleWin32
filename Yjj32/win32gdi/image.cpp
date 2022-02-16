#include "image.h"
#include "core/core.h"
#include "../application/application.h"
#include "../win32helper/windows_with_macro.h"
#include "../win32helper/win32helper.h"
#include <zlog/zlog.h>
#include <file/path.h>

namespace yjj {
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
		case yjj::win32gdi::image_load_result::success:
			return TEXT("success.");
			break;
		case yjj::win32gdi::image_load_result::failed_unknown_error:
			return TEXT("failed_unknown_error.");
			break;
		case yjj::win32gdi::image_load_result::failed_not_exist_image:
			return TEXT("failed_not_exist_image.");
			break;
		case yjj::win32gdi::image_load_result::failed_application_not_started:
			return TEXT("failed_application_not_started.");
			break;
		case yjj::win32gdi::image_load_result::failed_application_window_handle_invalid:
			return TEXT("failed_application_window_handle_invalid.");
			break;
		case yjj::win32gdi::image_load_result::failed_unknown_check_output_debug:
			return TEXT("failed_unknown_check_output_debug.");
			break;
		default:
			break;
		}

		return TEXT("failed_unknown_error.");
	}

	image_load_result load_image(handle_t& out_handle, const tstring& file_path) noexcept {
		out_handle = NULL;
		auto& app = application::get();
		if (!app.is_started()) {
			return image_load_result::failed_application_not_started;
		}
		
		if (!file::exists(file_path)) {
			return image_load_result::failed_not_exist_image;
		}

		if (!app.window_handle()) {
			return image_load_result::failed_application_window_handle_invalid;
		}

		out_handle = LoadImage(app.instance_handle<HINSTANCE>(), file_path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		if (out_handle == NULL) {
			ZEE_LOG_DETAIL(warning, TEXT("image"), TEXT("load_image is failed. detail: [%s]"), 
				win32helper::last_error_to_tstring().c_str()
			);
			return image_load_result::failed_unknown_check_output_debug;
		}

		return image_load_result::success;
	}
}//namespace yjj::win32gdi 
}//namespace yjj