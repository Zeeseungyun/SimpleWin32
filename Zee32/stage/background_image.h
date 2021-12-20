#pragma once
#include "../win32gdi/device_context.h"

namespace zee {
	using namespace win32gdi;
	class background_image {
	public:
		static background_image& get();

		void load_background_image(const math::vec2i& background_size, const wchar_t* filename);
		void show_loop(const float& delta_time, math::vec2i& src_pos, const int& direction);
		void render(device_context_dynamic& dest_dc, const math::vec2i& src_pos);
		void render_loop(device_context_dynamic& dest_dc, const math::vec2i& src_pos, const int& direction);

		const math::vec2i& get_size() const;

	private:
		background_image() noexcept;
		background_image(const background_image&) = delete;
		background_image& operator=(const background_image&) = delete;

		device_context_dynamic background_image_;
		math::vec2i size_;
		math::vec2i max_size_;
	};
}