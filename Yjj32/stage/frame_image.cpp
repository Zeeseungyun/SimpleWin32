#include "frame_image.h"

namespace yjj {
	frame_image& frame_image::get() {
		static std::unique_ptr<frame_image> inst;
		if (!inst) {
			inst = std::unique_ptr<frame_image>(new frame_image());
		}
		return *inst;
	}

	frame_image::frame_image() noexcept
	{
		frame_sizes_.resize(image_num_);
		frame_images_.resize(image_num_);

		back_buffers_.resize(image_num_);
		back_buffers2_.resize(image_num_);
	}
	//load
	void frame_image::load_frame_image(const math::vec2i& frame_size, const TCHAR* filename, const int index) {
		if (!frame_images_[index].is_valid()) {
			frame_images_[index].load_image(filename);
			frame_sizes_[index] = frame_size;
		}
		if (!back_buffers_[index].is_valid()) {
			back_buffers_[index].create_empty_image(frame_sizes_[index]);
		}
	}

	//일반 렌더
	void frame_image::render_transparent(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& dest_pos, const math::vec2i& src_pos, const int index) {
		if (frame_images_[index].is_valid()) {
			frame_images_[index].transparent_blt(dest_dc, dest_pos, frame_sizes_[index], src_pos, frame_sizes_[index], RGB(255, 255, 255));
		}
	}

	void frame_image::render_alphablend(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& dest_pos, const math::vec2i& src_pos, const int index) {
		if (frame_images_[index].is_valid()) {
			frame_images_[index].transparent_blt(back_buffers_[index], {}, frame_sizes_[index], src_pos, frame_sizes_[index], RGB(255, 255, 255));
			back_buffers_[index].alphablend(dest_dc, dest_pos, 0.5f);
		}
	}


	//plg 관련
	void frame_image::load_plg_image(const math::vec2i& frame_size, const TCHAR* filename, const int index) {
		if (!frame_images_[index].is_valid()) {
			frame_images_[index].load_image(filename);
			frame_sizes_[index] = frame_size;
		}

		if (!back_buffers_[index].is_valid()) {
			back_buffers_[index].create_empty_image({
				//회전할 때 src_pos 밖의 이미지가 자꾸 침범해서 간격을 만듦
				360 * (frame_sizes_[index].x + frame_sizes_[index].x * 2),
				frame_sizes_[index].y 
			});
		}

		if (!back_buffers2_[index].is_valid()) {
			back_buffers2_[index].create_empty_image(frame_sizes_[index]);
		}
	}

	void frame_image::render_plg(const math::vec2f& point, const float angle, const int index) {
		frame_images_[index].plg_blt(back_buffers_[index], point, angle, frame_sizes_[index]);
	}

	void frame_image::render_plg_transparent(win32gdi::device_context_dynamic& dest_dc, const math::vec2i& dest_pos, const math::vec2i& src_pos, const int index) {

		if (back_buffers_[index].is_valid()) {
			//검정 배경색 없애려고 + 전에 남은 이미지 없애려고 또 다른 백버퍼를 매번 클리어하고 사이즈 세팅하고 그려줬다...
			back_buffers2_[index].clear();
			back_buffers2_[index].create_empty_image(frame_sizes_[index]);

			back_buffers_[index].transparent_blt(back_buffers2_[index], {}, frame_sizes_[index], src_pos, frame_sizes_[index], RGB(255, 255, 255));
			back_buffers2_[index].transparent_blt(dest_dc, dest_pos, RGB(0, 0, 0));
		}
	}
}