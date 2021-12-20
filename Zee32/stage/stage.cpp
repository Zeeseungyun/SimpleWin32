#include "stage.h"

namespace zee {
	stage::stage() noexcept : background_src_pos({ 0, 0 }) {}
	stage::~stage() noexcept {}

	void stage::on_app_started() {
		application_delegates::on_client_size_changed().add_sp(shared_from_this(), &stage::on_resize);
		tick_manager::get().add(shared_from_this());
		on_resize(application::get().config().client_size);
		application::get().window_handle<HWND>();	//윈도우 핸들 가져오기

		game_init();
	}
	void stage::game_init() {
		switch (kind_of_background)
		{
		case loop:
			background_image::get().load_background_image({ 800, 1200 }, TEXT("assets/game_background_loop_vertical.bmp"));
			break;
		case stop:
			background_image::get().load_background_image({ 1152, 2048 }, TEXT("assets/game_background_stop.bmp"));
			break;
		}
		//프레임 이미지
		frame_image::get().load_frame_image({ 1152, 2048 }, { 64, 64 }, TEXT("assets/walk.bmp"));
		//유닛 세팅
		unit_.set_size({ 64, 64 });
		unit_.set_max_move_size({ 705, 770 });
		unit_.set_now_pos({ 350, 650 });
	}

	void stage::on_resize(const math::vec2i& client_size) {
		if (back_buffer.is_valid()) {
			back_buffer.resize(client_size);
		}
		else {
			back_buffer.create_empty_image(client_size);
		}
	}

	void stage::tick(float delta_time) {
		switch (kind_of_background)
		{
		case loop:
			//배경 루프 이미지
			background_image::get().show_loop(delta_time, background_src_pos, 2);
			//유닛 이동
			unit_.set_frame_direction(delta_time);
			unit_.move(delta_time);
			break;

		case stop:
			//배경 정지 이미지
			static const int background_speed = 10;
			unit_.set_frame_direction(delta_time);
			if (unit_.get_is_pressed()) {
				switch (unit_.get_direction()) {
				case 0:
					if (unit_.get_now_pos().y > 0 && background_src_pos.y > 0) {
						background_src_pos.y -= background_speed;
					}
					break;
				case 1:
					if (unit_.get_now_pos().x > 0 && background_src_pos.x > 0) {
						background_src_pos.x -= background_speed;
					}
					break;
				case 2:
					if (unit_.get_now_pos().y < 770 && background_src_pos.y < 1152) {
						background_src_pos.y += background_speed;
					}
					break;
				case 3:
					if (unit_.get_now_pos().x < 705 && background_src_pos.x < 390) {
						background_src_pos.x += background_speed;
					}
					break;
				}
			}
			break;
		}
	}

	void stage::render(win32gdi::device_context_base& dest_dc) {
		if (back_buffer.is_valid()) {
			switch (kind_of_background)
			{
			case loop:
				background_image::get().render_loop(back_buffer, background_src_pos, 2);
				break;

			case stop:
				background_image::get().render(back_buffer, background_src_pos);
				break;
			}

			//frame_image::get().render(back_buffer, unit_.get_now_pos(), unit_.get_frame_x(), unit_.get_frame_y());
			frame_image::get().render_alphablend(back_buffer, unit_.get_now_pos(), unit_.get_frame_x(), unit_.get_frame_y());

			back_buffer.bit_blt(dest_dc, {});
		}
	}
}