#pragma once
#include "core/string.h"
#include "shape/rect.h"
#include "math/math.h"

namespace zee {
	typedef std::size_t size_t;
	typedef void* handle_t;

namespace win32gdi {
	enum class image_type {
		none,
		bmp, //현재 bmp만 지원
	};

	enum class image_load_type {
		none,
		file,
		buffer_
	};

	struct simple_bitmap_desc {
	public:
		math::vec2i size;
		image_type type = image_type::none;
		image_load_type load_type = image_load_type::none;
		tstring file_path;

	public:
		void clear();
	};

	enum class image_load_result {
		success,
		failed_unknown_error,
		failed_not_exist_image,
		failed_application_not_started,
		failed_application_window_handle_invalid,
		failed_unknown_check_output_debug,
	};

	image_load_result load_image(handle_t& out_handle, const tstring& file_path) noexcept;
	tstring image_load_result_to_string(image_load_result result) noexcept;
}//namespace zee::win32gdi
}//namespace zee