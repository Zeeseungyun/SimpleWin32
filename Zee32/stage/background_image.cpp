#include "background_image.h"

namespace zee {
	background_image::background_image() noexcept {
	}
	background_image& background_image::get() {
		static std::unique_ptr<background_image> inst;
		if (!inst) {
			inst = std::unique_ptr<background_image>(new background_image());
		}
		return *inst;
	}

	void background_image::load_background_image(const math::vec2i& background_size, const TCHAR* filename) {
		if (!background_image_.is_valid()) {
			background_image_.load_image(to_tstring(filename));
			size_ = background_size;
		}
	}

	void background_image::render(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& src_pos) {
		background_image_.bit_blt(dest_dc, {}, src_pos, size_);
	}
}