#include "frame_image.h"

namespace zee {
	frame_image::frame_image() noexcept : frame_size_({ 0, 0 }), max_frame_size_({ 0, 0 }) {
	}
	frame_image& frame_image::get() {
		static std::unique_ptr<frame_image> inst;
		if (!inst) {
			inst = std::unique_ptr<frame_image>(new frame_image());
		}
		return *inst;
	}

	void frame_image::load_frame_image(const math::vec2i& max_frame_size, const math::vec2i& frame_size, const wchar_t* filename) {
		if (!frame_image_.is_valid()) {
			frame_image_.load_image(filename);
		}
		max_frame_size_ = max_frame_size;
		frame_size_ = frame_size;
	}

	void frame_image::render(device_context_dynamic& dest_dc, const math::vec2i& dest_pos, const math::vec2i& frame_x, const math::vec2i& frame_y) {
		if (frame_image_.is_valid()) {
			frame_image_.transparent_blt(dest_dc, dest_pos, frame_size_, frame_x + frame_y, frame_size_, RGB(255, 255, 255));
		}
	}

	void frame_image::render_alphablend(device_context_dynamic& dest_dc, const math::vec2i& dest_pos, const math::vec2i& frame_x, const math::vec2i& frame_y) {
		if (frame_image_.is_valid()) {
			device_context_dynamic temp_dc;

			temp_dc.create_empty_image(max_frame_size_);
			dest_dc.bit_blt(temp_dc, {});
			frame_image_.transparent_blt(temp_dc, dest_pos, frame_size_, frame_x + frame_y, frame_size_, RGB(255, 255, 255));

			temp_dc.alphablend(dest_dc, {}, 0.4f);
		}
	}
}