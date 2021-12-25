#include "stage.h"

namespace zee {
	using namespace math;

	stage::stage() noexcept : 
		background_src_pos_({ 0.0f, 0.0f })
		, background_src_size_({ 0, 0 }) {
	}
	stage::~stage() noexcept {
	}

	void stage::on_app_started() {
		application_delegates::on_client_size_changed().add_sp(shared_from_this(), &stage::on_resize);
		tick_manager::get().add(shared_from_this());
		on_resize(application::get().config().client_size);
		application::get().window_handle<HWND>();	//윈도우 핸들 가져오기
		
		game_init();

		//테스트용
		matrix m;
		//행렬식
		m.set_m2f({ {2,3}, {4,6} });
		m.determinant();
		//항등행렬
		m.set_m3f({ {1, 0, 0}, {0,1,0}, {0,0,1} });
		if (m.is_identity()) {
			ZEE_LOG(warning, TEXT("항등행렬 임"), TEXT("-"));
		}
		else {
			ZEE_LOG(warning, TEXT("항등행렬 아니니니니"), TEXT("-"));
		}
		//역행렬
		std::vector<math::vec2f> vv = { {2,3}, {4,5} };
		m.inverse(vv);
	}

	void stage::game_init() {
		std::shared_ptr<unit> spawned_unit = std::make_shared<unit>();
		std::shared_ptr<monster> spawned_monster = std::make_shared<monster>();

		switch (kind_of_background)	{
		case loop:
			background_image::get().load_background_image({ 800, 1200 }, TEXT("assets/game_background_loop_vertical.bmp"));
			background_src_pos_ = { 0.0f, 0.0f };
			background_src_size_ = { 800, 1200 };
			break;
		case scroll:
			background_image::get().load_background_image({ 1152, 2048 }, TEXT("assets/game_background_stop.bmp"));
			background_src_pos_ = { 0.0f, 1100.0f};	//y축 시작위치 설정
			break;
		}
		//프레임 이미지
		frame_image::get().load_frame_image({ 1152, 2048 }, { 64, 64 }, TEXT("assets/walk.bmp"));


		//유닛 세팅
		spawned_unit->set_size({ 64, 64 });
		spawned_unit->set_max_move_size({ 705, 770 });
		spawned_unit->set_now_pos({ 350, 650 });
		units_.push_back(spawned_unit);
		//몬스터 세팅
		spawned_monster->set_rotate_point({ 0.0f, 0.0f });
		monsters_.push_back(spawned_monster);
	}

	void stage::on_resize(const math::vec2i& client_size) {
		if (back_buffer_.is_valid()) {
			back_buffer_.resize(client_size);
		}
		else {
			back_buffer_.create_empty_image(client_size);
		}
	}

	void stage::tick(float delta_time) {
		switch (kind_of_background) {
		case loop: {
			static const float show_loop_time = 120.0f;
			switch (background_direction_) {
				//배경 루프 이미지
				//direction 이미지 기준 0: 좌->우.. 1: 우->좌.. 2: 상->하.. 3: 하->상
			case 0:
			case 1:
				background_src_pos_.x += delta_time * show_loop_time;
				background_src_pos_.x = (float)math::fmod(background_src_pos_.x, background_src_size_.x);
				break;
			case 2:
			case 3:
				background_src_pos_.y += delta_time * show_loop_time;
				background_src_pos_.y = (float)math::fmod(background_src_pos_.y, background_src_size_.y);
				break;
			}
			break;
		}

		case scroll: {
			static const int background_speed = 10;
			if (units_[0]->get_is_pressed()) {
				switch (units_[0]->get_direction_()) {
				//배경 정지 이미지
				case 0:
					if (units_[0]->get_now_pos().y > 0 && background_src_pos_.y > 0) {
						background_src_pos_.y -= background_speed;
					}
					break;
				case 1:
					if (units_[0]->get_now_pos().x > 0 && background_src_pos_.x > 0) {
						background_src_pos_.x -= background_speed;
					}
					break;
				case 2:
					if (units_[0]->get_now_pos().y < 770 && background_src_pos_.y < 1152) {
						background_src_pos_.y += background_speed;
					}
					break;
				case 3:
					if (units_[0]->get_now_pos().x < 705 && background_src_pos_.x < 390) {
						background_src_pos_.x += background_speed;
					}
					break;
				}
			}
			break;
		}//case
		}//switch (kind_of_background)


		for (auto& unit_obj : units_) {
			unit_obj->tick(delta_time);
		}
		for (auto& mon_obj : monsters_) {
			mon_obj->tick(delta_time);
		}
	}

	void stage::render(win32gdi::device_context_base& dest_dc) {
		if (back_buffer_.is_valid()) {
			switch (kind_of_background)	{
			case loop:
				//배경 루프 이미지
				//direction 이미지 기준 0: 좌->우.. 1: 우->좌.. 2: 상->하.. 3: 하->상
				switch (background_direction_)
				{
				case 0:
					background_image::get().render(
						back_buffer_
						, -background_src_pos_
					);
					background_image::get().render(
						back_buffer_ 
						, { -background_src_pos_.x + background_src_size_.x
						, background_src_pos_.y }
					);
					break;
				case 1:
					background_image::get().render(
						back_buffer_
						, background_src_pos_
					);
					background_image::get().render(
						back_buffer_
						, { background_src_pos_.x - background_src_size_.x
						, background_src_pos_.y }
					);
					break;
				case 2:
					background_image::get().render(
						back_buffer_
						, -background_src_pos_
					);
					background_image::get().render(
						back_buffer_
						, { background_src_pos_.x
						, -background_src_pos_.y + background_src_size_.y }
					);
					break;
				case 3:
					background_image::get().render(
						back_buffer_
						, background_src_pos_
					);
					background_image::get().render(
						back_buffer_
						, { background_src_pos_.x
						, background_src_pos_.y - background_src_size_.y }
					);
					break;
				}
				break;

			case scroll:
				background_image::get().render(back_buffer_, background_src_pos_);
				break;
			}
		}

		//유닛
		for (auto& unit_obj : units_) {
			unit_obj->render(back_buffer_);
		}

		for (auto& mon_obj : monsters_) {
			mon_obj->render(back_buffer_);
		}

		back_buffer_.bit_blt(dest_dc, {});
	}

	const math::vec2f& stage::get_background_src_pos() const {
		return background_src_pos_;
	}
	const math::vec2f& stage::get_background_src_size() const {
		return background_src_size_;
	}
	void stage::set_background_src_pos(const math::vec2f& src_pos) {
		background_src_pos_ = src_pos;
	}
	void stage::set_background_src_size(const math::vec2f& size) {
		background_src_size_ = size;
	}
}