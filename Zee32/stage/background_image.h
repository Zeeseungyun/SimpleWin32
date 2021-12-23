#pragma once
#include "../win32gdi/device_context.h"

namespace zee {
	class background_image {
	public:
		static background_image& get();

		void load_background_image(const math::vec2i& background_size, const TCHAR* filename);
		void render(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& src_pos);
		void render_loop(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& src_pos, const int& direction);


	private:
		background_image() noexcept;
		background_image(const background_image&) = delete;
		background_image& operator=(const background_image&) = delete;

		win32gdi::device_context_dynamic background_image_;
		math::vec2i size_;
		math::vec2i max_size_;
	};
}