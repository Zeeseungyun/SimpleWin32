#pragma once
#include "../win32gdi/device_context.h"

namespace yjj {
	class background_image {
	public:
		static background_image& get();

		void load_background_image(const math::vec2i& background_size, const TCHAR* filename);
		void render(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& src_pos);

	private:
		background_image() noexcept = default;
		background_image(const background_image&) = delete;
		background_image& operator=(const background_image&) = delete;

		win32gdi::device_context_dynamic background_image_;
		math::vec2i size_;
	};
}