#pragma once
#include "../application/key_state.h"
#include "../win32helper/win32helper.h"
#include "../win32gdi/device_context.h"
#include <vector>

namespace zee {
	class frame_image {
	public:
		static frame_image& get();

		void render(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& dest_pos, const math::vec2i& frame_x, const math::vec2i& frame_y);
		void render_alphablend(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& dest_pos, const math::vec2i& frame_x, const math::vec2i& frame_y);
		void render_rotate(win32gdi::device_context_dynamic& dest_dc, const math::vec2f& point, const float& angle);
		void load_frame_image(const math::vec2i& max_frame_size, const math::vec2i& frame_size, const TCHAR* filename);

	private:
		frame_image() noexcept;
		frame_image(const frame_image&) = delete;
		frame_image& operator=(const frame_image&) = delete;

		win32gdi::device_context_dynamic frame_image_;
		win32gdi::device_context_dynamic alpha_buffer;
		math::vec2i frame_size_;
		math::vec2i max_frame_size_;

	};
}