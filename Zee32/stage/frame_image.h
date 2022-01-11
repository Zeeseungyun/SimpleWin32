#pragma once
#include "../win32helper/win32helper.h"
#include "../win32gdi/device_context.h"

namespace zee {
	class frame_image {
	public:
		static frame_image& get();

		void load_frame_image(const math::vec2i& max_frame_size, const math::vec2i& frame_size, const TCHAR* filename, const int i);
		void render_destdc_to_backbuffer(win32gdi::device_context_dynamic& dest_dc, const int i);
		void render_alphablend(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& dest_pos, const math::vec2i& src_pos, const int i);
		void render_transparent(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& dest_pos, const math::vec2i& src_pos, const int i);
		void render_transparent_backbuffer_to_destdc(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& dest_pos, const int i);
		void render_plg(win32gdi::device_context_dynamic& dest_dc, const math::vec2f& point, const float angle, const int i);
	
	private:
		frame_image() noexcept;
		frame_image(const frame_image&) = delete;
		frame_image& operator=(const frame_image&) = delete;

		win32gdi::device_context_dynamic back_buffer_;
		std::vector<win32gdi::device_context_dynamic> back_buffers_;
		std::vector<win32gdi::device_context_dynamic> frame_images_;
		std::vector<math::vec2i> frame_sizes_;
		std::vector<math::vec2i> max_frame_sizes_;

		/// <summary>
		/// 이미지 개수
		/// </summary>
		const int image_num_  = 15;
	};
}