#include "stage.h"

namespace zee {
	using namespace math;

	stage::stage() noexcept {
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
		/*matrix m;
		//행렬식
		m.set_m2f({ {2,3}, {4,6} });
		m.determinant();
		//항등행렬
		m.set_m3f({ {1, 0, 0}, {0,1,0}, {0,0,1} });
		if (m.is_identity()) {
			ZEE_LOG(normal, TEXT("항등행렬 임"), TEXT("-"));
		}
		else {
			ZEE_LOG(warning, TEXT("항등행렬 아니니니니"), TEXT("-"));
		}
		//역행렬
		std::vector<math::vec2f> vv = { {2,3}, {4,5} };
		m.inverse(vv);*/
	}

	void stage::game_init() {
		switch (background_type_)	{
		case loop:
			background_image::get().load_background_image({ (int)back_loop_max_size_x, (int)back_loop_max_size_y }
				, TEXT("assets/game_background_loop_vertical.bmp"));
			background_src_pos_ = { 0.0f, 0.0f };
			background_src_size_ = { (int)back_loop_max_size_x, (int)back_loop_max_size_y };
			break;
		case scroll:
			background_image::get().load_background_image({ (int)back_scroll_max_size_x, (int)back_scroll_max_size_y }
				, TEXT("assets/game_background_stop.bmp"));
			background_src_pos_ = { (float)back_scroll_default_unit_pos_x, (float)back_scroll_default_unit_pos_y };	//y축 시작위치 설정
			break;
		}
		
		//유닛 스폰
		std::shared_ptr<unit> spawned_unit = std::make_shared<unit>();
		spawned_unit->set_size({ (int)unit_size_x, (int)unit_size_y });
		spawned_unit->set_body({ (float)unit_default_pos_x, (float)unit_default_pos_y });
		spawned_unit->set_frame_size({ (int)unit_default_frame_x, (int)unit_default_frame_y });
		spawned_unit->set_max_move_size({ (int)unit_max_move_size_x, (int)unit_max_move_size_y });
		units_.push_back(spawned_unit);

		//몬스터 스폰
		spawn_monster((int)obj_spawn::mon_left);
		spawn_monster((int)obj_spawn::mon_middle);
		spawn_monster((int)obj_spawn::mon_right);
	}
	//몬스터 스폰
	void stage::spawn_monster(const int& i) {
		std::shared_ptr<monster> spawned_monster = std::make_shared<monster>();
		spawned_monster->set_size({ (int)monster_size_x, (int)monster_size_y });
		spawned_monster->set_shoot_type({ (int)obj_shoot_type::straight });
		switch (i)
		{
		case (int)obj_spawn::mon_left:
			spawned_monster->set_spawn_pos_type((int)obj_spawn::mon_left);
			spawned_monster->set_body({ (float)monster_default_left_pos_x, (float)monster_default_left_pos_y });
			break;
		case (int)obj_spawn::mon_middle:
			spawned_monster->set_spawn_pos_type((int)obj_spawn::mon_middle);
			spawned_monster->set_body({ (float)monster_default_middle_pos_x, (float)monster_default_middle_pos_y });
			break;		
		case (int)obj_spawn::mon_right:
			spawned_monster->set_spawn_pos_type((int)obj_spawn::mon_right);
			spawned_monster->set_body({ (float)monster_default_right_pos_x, (float)monster_default_right_pos_y });
			break;
		}
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
		switch (background_type_) {
		case loop: {
			const float speed = 120.0f;
			switch (background_direction_) {
			//배경 루프 이미지
			//direction 이미지 기준 0: 좌->우.. 1: 우->좌.. 2: 상->하.. 3: 하->상
			case 0:
			case 1:
				background_src_pos_.x += delta_time * speed;
				background_src_pos_.x = (float)math::fmod(background_src_pos_.x, background_src_size_.x);
				break;
			case 2:
			case 3:
				background_src_pos_.y += delta_time * speed;
				background_src_pos_.y = (float)math::fmod(background_src_pos_.y, background_src_size_.y);
				break;
			}
			break;	//loop 끝
		}

		case scroll: {
			const int background_speed = 10;
			if (units_[0]->get_is_pressed()) {
				switch (units_[0]->get_direction()) {
				//배경 정지 이미지
				case 0:
					if (units_[0]->get_body().data[0].y > 0 && background_src_pos_.y > 0) {
						background_src_pos_.y -= background_speed;
					}
					break;
				case 1:
					if (units_[0]->get_body().data[0].x > 0 && background_src_pos_.x > 0) {
						background_src_pos_.x -= background_speed;
					}
					break;
				case 2:
					if (units_[0]->get_body().data[0].y < (int)back_scroll_unit_max_move_y && background_src_pos_.y < (int)back_scroll_max_y) {
						background_src_pos_.y += background_speed;
					}
					break;
				case 3:
					if (units_[0]->get_body().data[0].x < (int)back_scroll_unit_max_move_x && background_src_pos_.x < (int)back_scroll_max_x) {
						background_src_pos_.x += background_speed;
					}
					break;
				}
			}
			break;
		}//case
		}//switch (kind_of_background)

		//유닛 틱
		for (auto& unit_obj : units_) {
			unit_obj->tick(delta_time);
		}
		for (auto& mon_obj : monsters_) {
			//적 틱
			mon_obj->tick(delta_time);

			//적 유도탄
			static float delay = 0.0f;
			const float frame = 0.2f;
			delay += delta_time;
			if (delay >= frame) {
				for (auto& bullet_obj : mon_obj->get_bullets()) {
					if (bullet_obj->get_move_type() == (int)obj_shoot_type::follow) {
						//이동 방향 벡터
						math::vec2f v{
							units_[0]->get_body().data[0] - bullet_obj->get_body().data[0]
						};
						//단위화 위해 거리 구하기
						float dist = sqrtf(v.x * v.x + v.y * v.y);
						//단위화
						v /= dist;
						bullet_obj->set_vec_for_player(v);
					}
				}
				delay = (float)math::fmod(delay, frame);
			}

			//플레이어 총알 vs 적 충돌 틱
			for (auto& bullet_obj : units_[0]->get_bullets()) {
				if (shape::intersect(mon_obj->get_body(), bullet_obj->get_body()) 
					!= shape::collide_type::none) {
					//유저 총알 처리
					bullet_obj->set_state((int)obj_state::hit);
					bullet_obj->set_bomb_point(bullet_obj->get_body().data[0]);
					bullet_obj->set_body({ (int)back_loop_max_size_x, (int)back_loop_max_size_y });

					//몬스터 및 몬스터 소유 총알 처리
					mon_obj->destroy(delta_time);
				}
			}

			//적 총알 vs 플레이어 충돌 틱
			for (auto& bullet_obj : mon_obj->get_bullets()) {
				if (shape::intersect(units_[0]->get_body(), bullet_obj->get_body()) 
					!= shape::collide_type::none) {

					bullet_obj->set_body({ (int)back_destroy_zone_x, (int)back_destroy_zone_y });
					units_[0]->set_state((int)obj_state::hit);
				}
			}

			//적 vs 플레이어 충돌 틱
			if (shape::intersect(mon_obj->get_body(), units_[0]->get_body())
				!= shape::collide_type::none) {

				units_[0]->set_state((int)obj_state::hit);
			}

		}
	}

	void stage::render(win32gdi::device_context_base& dest_dc, const float& g_fps) {
		if (back_buffer_.is_valid()) {
			switch (background_type_)	{
			case loop:
				//배경 루프 이미지
				//direction 이미지 기준 0: 좌->우.. 1: 우->좌.. 2: 상->하.. 3: 하->상
				switch (background_direction_)
				{
				case 0:
					background_image::get().render(back_buffer_, -background_src_pos_);
					background_image::get().render(
						back_buffer_
						, { -background_src_pos_.x + background_src_size_.x
						, background_src_pos_.y }
					);
					break;
				case 1:
					background_image::get().render(back_buffer_, background_src_pos_);
					background_image::get().render(
						back_buffer_
						, { background_src_pos_.x - background_src_size_.x
						, background_src_pos_.y }
					);
					break;
				case 2:
					background_image::get().render(back_buffer_, -background_src_pos_);
					background_image::get().render(
						back_buffer_
						, { background_src_pos_.x
						, -background_src_pos_.y + background_src_size_.y }
					);
					break;
				case 3:
					background_image::get().render(back_buffer_, background_src_pos_);
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
		//몬스터
		for (auto& mon_obj : monsters_) {
			mon_obj->render(back_buffer_);
		}

		back_buffer_.print_text({ 0, 0 }, to_tstring(to_wstring(g_fps)));
		back_buffer_.bit_blt(dest_dc, {});
	}

	const math::vec2f stage::get_background_src_pos() const {
		return background_src_pos_;
	}
	const math::vec2f stage::get_background_src_size() const {
		return background_src_size_;
	}
	const std::vector<std::shared_ptr<unit>> stage::get_units() const {
		return units_;
	}
	void stage::set_background_src_pos(const math::vec2f& src_pos) {
		background_src_pos_ = src_pos;
	}
	void stage::set_background_src_size(const math::vec2f& size) {
		background_src_size_ = size;
	}
}