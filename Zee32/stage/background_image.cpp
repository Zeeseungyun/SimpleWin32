#include "background_image.h"

namespace zee {
	background_image::background_image() noexcept : size_({ 0, 0 }) {
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

	//direction 이미지 기준 0: 좌->우.. 1: 우->좌.. 2: 상->하.. 3: 하->상
	void background_image::render_loop(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& src_pos, const int& direction) {
		if (background_image_.is_valid()) {
			switch (direction)
			{
			case 0:
				background_image_.bit_blt(dest_dc, {}, -src_pos, size_);
				background_image_.bit_blt(dest_dc, {}, { -src_pos.x + size_.x, src_pos.y }, size_);
				break;
			case 1:
				background_image_.bit_blt(dest_dc, {}, src_pos, size_);
				background_image_.bit_blt(dest_dc, {}, { src_pos.x - size_.x, src_pos.y }, size_);
				break;
			case 2:
				background_image_.bit_blt(dest_dc, {}, -src_pos, size_);
				background_image_.bit_blt(dest_dc, {}, { src_pos.x, -src_pos.y + size_.y }, size_);
				break;
			case 3:
				background_image_.bit_blt(dest_dc, {}, src_pos, size_);
				background_image_.bit_blt(dest_dc, {}, { src_pos.x, src_pos.y - size_.y }, size_);
				break;
			}
		}
	}
}