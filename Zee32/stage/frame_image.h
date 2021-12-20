#pragma once
#include "../application/key_state.h"
#include "../win32helper/win32helper.h"
#include "../win32gdi/device_context.h"

namespace zee {
	using namespace win32gdi;

	class frame_image {
	public:
		static frame_image& get();

		void render(device_context_dynamic& dest_dc, const math::vec2i& dest_pos, const math::vec2i& frame_x, const math::vec2i& frame_y);
		void render_alphablend(device_context_dynamic& dest_dc, const math::vec2i& dest_pos, const math::vec2i& frame_x, const math::vec2i& frame_y);
		void load_frame_image(const math::vec2i& max_frame_size, const math::vec2i& frame_size, const wchar_t* filename);

	private:
		frame_image() noexcept;
		frame_image(const frame_image&) = delete;
		frame_image& operator=(const frame_image&) = delete;

		device_context_dynamic frame_image_;
		math::vec2i frame_size_;
		math::vec2i max_frame_size_;
	};
}