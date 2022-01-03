#include "frame_image.h"
#include "stage.h"

namespace zee {
	frame_image::frame_image() noexcept {
		frame_sizes_.resize((int)obj_type::max);
		max_frame_sizes_.resize((int)obj_type::max);
		frame_images_.resize((int)obj_type::max);
	}
	frame_image& frame_image::get() {
		static std::unique_ptr<frame_image> inst;
		if (!inst) {
			inst = std::unique_ptr<frame_image>(new frame_image());
		}
		return *inst;
	}

	void frame_image::load_frame_image(const math::vec2i& max_frame_size, const math::vec2i& frame_size, const TCHAR* filename, const int i) {
		if (!frame_images_[i].is_valid()) {
			frame_images_[i].load_image(filename);
			max_frame_sizes_[i] = max_frame_size;
			frame_sizes_[i] = frame_size;
		}

		if (!back_buffer_.is_valid()) {
			back_buffer_.create_empty_image(max_frame_sizes_[i]);
		}
	}

	void frame_image::render_destdc_to_backbuffer(win32gdi::device_context_dynamic& dest_dc) {
		dest_dc.bit_blt(back_buffer_, {});
	}

	void frame_image::render_alphablend(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& dest_pos, const math::vec2i& src_pos, const int i) {
		if (frame_images_[i].is_valid()) {
			frame_images_[i].transparent_blt(back_buffer_, dest_pos, frame_sizes_[i], src_pos, frame_sizes_[i], RGB(255, 255, 255));
			back_buffer_.alphablend(dest_dc, {}, 0.5f);
		}
	}

	void frame_image::render_transparent(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& dest_pos, const math::vec2i& src_pos, const int i) {
		if (frame_images_[i].is_valid()) {
			frame_images_[i].transparent_blt(back_buffer_, dest_pos, frame_sizes_[i], src_pos, frame_sizes_[i], RGB(255, 255, 255));
			back_buffer_.bit_blt(dest_dc, {});
		}
	}

	void frame_image::render_transparent_backbuffer_to_destdc(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& dest_pos) {
		back_buffer_.transparent_blt(dest_dc, dest_pos, RGB(255, 255, 255));
	}

	void frame_image::render_plg(win32gdi::device_context_dynamic& dest_dc, const math::vec2f& point, const float angle, const int i) {
		if (frame_images_[i].is_valid()) {
			frame_images_[i].plg_blt(back_buffer_, point, angle, frame_sizes_[i]);
		}
	}

}