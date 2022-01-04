#include "stage.h"

namespace zee {
	using namespace math;

	stage::stage() noexcept :
		delay_bomb_() {
	}
	stage::~stage() noexcept {
	}

	void stage::on_app_started() {
		application_delegates::on_client_size_changed().add_sp(shared_from_this(), &stage::on_resize);
		tick_manager::get().add(shared_from_this());
		on_resize(application::get().config().client_size);
		application::get().window_handle<HWND>();	//윈도우 핸들 가져오기

		//이미지 로드
		//배경
		background_image::get().load_background_image(coords[back_max_size]
			, TEXT("assets/game_background_loop_vertical.bmp"));
		background_image::get().load_background_image(coords[back_scroll_max_size]
			, TEXT("assets/game_background_stop.bmp"));
		//유닛
		frame_image::get().load_frame_image(coords[back_max_size], coords[unit_size]
			, TEXT("assets/player.bmp"), (int)obj_type::unit);
		frame_image::get().load_frame_image(coords[back_max_size], coords[monster_1_size]
			, TEXT("assets/monster_1.bmp"), (int)obj_type::monster_1);
		frame_image::get().load_frame_image(coords[back_max_size], coords[monster_2_size]
			, TEXT("assets/monster_2.bmp"), (int)obj_type::monster_2);
		frame_image::get().load_frame_image(coords[back_max_size], coords[monster_3_size]
			, TEXT("assets/monster_3.bmp"), (int)obj_type::monster_3);
		frame_image::get().load_frame_image(coords[back_max_size], coords[monster_4_size]
			, TEXT("assets/monster_4.bmp"), (int)obj_type::monster_4);
		frame_image::get().load_frame_image(coords[back_max_size], coords[monster_5_size]
			, TEXT("assets/monster_5.bmp"), (int)obj_type::monster_5);
		//뷸렛
		frame_image::get().load_frame_image(coords[back_max_size], coords[unit_bullet_straight_size]
			, TEXT("assets/unit_bullet.bmp"), (int)obj_type::unit_bullet_straight);
		frame_image::get().load_frame_image(coords[back_max_size], coords[monster_bullet_straight_size]
			, TEXT("assets/monster_bullet_straight.bmp"), (int)obj_type::monster_bullet_straight);
		frame_image::get().load_frame_image(coords[back_max_size], coords[monster_bullet_circle_size]
			, TEXT("assets/monster_bullet_circle.bmp"), (int)obj_type::monster_bullet_circle);
		frame_image::get().load_frame_image(coords[back_max_size], coords[monster_bullet_homing_size]
			, TEXT("assets/monster_bullet_homing.bmp"), (int)obj_type::monster_bullet_homing);
		frame_image::get().load_frame_image(coords[back_max_size], coords[monster_bullet_arround_size]
			, TEXT("assets/monster_bullet_arround.bmp"), (int)obj_type::monster_bullet_arround);
		frame_image::get().load_frame_image(coords[back_max_size], coords[monster_bullet_wave_size]
			, TEXT("assets/monster_bullet_wave.bmp"), (int)obj_type::monster_bullet_wave);
		//폭발
		frame_image::get().load_frame_image(coords[back_max_size], coords[effect_bomb_size]
			, TEXT("assets/bomb.bmp"), (int)obj_type::bomb);

		//게임 초기화
		init_game();

		//행렬 테스트
		/*matrix m;
		//전치행렬
		m.set_m3f({ { 0,1,2 }, { 3,4,5 } });
		m.transposed();	
		m.transposed();
		//행렬식
		m.set_m2f({{2,3}, {4,6}});
		m.determinant();
		//항등행렬
		m.set_m3f({ {1, 0, 0}, {0, 1, 0}, {0, 0, 1} });
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

	void stage::on_resize(const math::vec2i& client_size) {
		if (back_buffer_.is_valid()) {
			back_buffer_.resize(client_size);
		}
		else {
			back_buffer_.create_empty_image(client_size);
		}
	}

	void stage::init_game() {

		switch (background_type_)	{
		case loop:
			background_src_pos_ = { 0.0f, 0.0f };
			background_src_size_ = coords[back_loop_max_size];
			break;
		case scroll:
			background_src_pos_ = coords[back_scroll_default_unit_pos];	//y축 시작위치 설정
			break;
		}
		
		//유닛 스폰
		std::shared_ptr<unit> spawned_unit = std::make_shared<unit>();
		units_.push_back(spawned_unit);
		//유닛 초기화
		units_.front()->init();

		//몬스터 스폰
		//for (int i = 0; i != 1; i++) {	//테스트
		for (int i = 0; i != monster_spawn_num; i++) {
			std::shared_ptr<monster> spawned_monster = std::make_shared<monster>();
			monsters_.push_back(spawned_monster);
		}
		//몬스터 초기화
		for (auto& monster_obj : monsters_) {
			monster_obj->init();
		}

		//이펙트
		for (int i = 0; i != effect_bomb_max_num; i++) {
			std::shared_ptr<bomb> spawned_bomb = std::make_shared<bomb>();
			spawned_bomb->dest_pos_ = { 0.0f, 0.0f };
			spawned_bomb->src_pos_ = { 0.0f, 0.0f };
			spawned_bomb->src_size_ = coords[effect_bomb_size];
			spawned_bomb->hp_ = (int)obj_state::die;
			bombs_.push_back(spawned_bomb);
		}
	}

	void stage::tick(float delta_time) {
		//배경 틱
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
			break;
		}//case loop

		case scroll: {
			const float background_speed = 10.0f;
			if (units_.front()->get_is_dir_key_pressed()) {
				switch (units_.front()->get_direction()) {
				//배경 정지 이미지
				case 0:
					if (units_.front()->get_now_pos().y > 0 && background_src_pos_.y > 0) {
						background_src_pos_.y -= delta_time * background_speed;
					}
					break;
				case 1:
					if (units_.front()->get_now_pos().x > 0 && background_src_pos_.x > 0) {
						background_src_pos_.x -= delta_time * background_speed;
					}
					break;
				case 2:
					if (units_.front()->get_now_pos().y < coords[back_scroll_unit_max_move].y 
						&& background_src_pos_.y < coords[back_scroll_max].y) {
						background_src_pos_.y += delta_time * background_speed;
					}
					break;
				case 3:
					if (units_.front()->get_now_pos().x < coords[back_scroll_unit_max_move].x 
						&& background_src_pos_.x < coords[back_scroll_max].x) {
						background_src_pos_.x += delta_time * background_speed;
					}
					break;
				}
			}
			break;
		}//case scroll
		}//switch (kind_of_background)

		//유닛 틱
		for (auto& unit_obj : units_) {
			unit_obj->tick(delta_time);
		}
		for (auto& mon_obj : monsters_) {
			//적 틱
			mon_obj->tick(delta_time);

			//적 호밍
			if (mon_obj->get_shoot_type() == (int)obj_shoot_type::arround) {
				//플레이어와의 방향 벡터
				math::vec2f v_mon_for_player{
					units_.front()->get_body().origin - mon_obj->get_body().origin
				};
				mon_obj->set_vec_for_player(v_mon_for_player);
			}

			//적 유도탄 호밍
			for (auto& bullet_obj : mon_obj->get_bullets()) {
				if (bullet_obj->get_move_type() == (int)obj_shoot_type::homing) {
					//플레이어와의 방향 벡터 : 뷸렛 초기화 한 다음에 판단함. normalize를 여기서 해줘야 함
					math::vec2f v_bullet_for_player{
						units_.front()->get_body().origin - bullet_obj->get_body().origin
					};
					v_bullet_for_player = v_bullet_for_player.normalize();
					bullet_obj->set_vec_for_player(v_bullet_for_player);
				}
			}


			//플레이어 총알 vs 적 충돌 틱
			for (auto& bullet_obj : units_.front()->get_bullets()) {
				if (shape::intersect(mon_obj->get_body(), bullet_obj->get_body()) != shape::collide_type::none) {

					//폭발 이펙트 위치 설정
					for (auto& bomb_obj : bombs_) {
						if (bomb_obj->hp_ == (int)obj_state::die) {
							bomb_obj->hp_ = (int)obj_state::idle;
							bomb_obj->dest_pos_ = {
								mon_obj->get_body().origin.x - mon_obj->get_body().radius,
								mon_obj->get_body().origin.y - mon_obj->get_body().radius
							};
							bomb_obj->src_pos_ = {0, 0};
							break;
						}
					}

					//사망처리
					mon_obj->set_hp((int)obj_state::die);
					bullet_obj->set_now_pos_and_body(coords[back_destroy_zone]);
				}
			}

			//폭발 이펙트 틱
			const float speed = 2.0f;
			const float frame = 3.0f;
			delay_bomb_ += delta_time * speed;
			
			for (auto& bomb_obj : bombs_) {
				if (bomb_obj->hp_ == (int)obj_state::idle) {
					if (delay_bomb_ >= frame) {
						delay_bomb_ = (float)math::fmod(delay_bomb_, frame);
						bomb_obj->hp_ = (int)obj_state::die;
					}
					bomb_obj->src_pos_.x = coords[effect_bomb_size].x * (int)delay_bomb_;
					break;
				}
			}

			//적 총알 vs 플레이어 충돌 틱
			for (auto& bullet_obj : mon_obj->get_bullets()) {
				if (bullet_obj->in_screen()
					&& shape::intersect(units_.front()->get_body(), bullet_obj->get_body()) != shape::collide_type::none) {

					units_.front()->set_hp((int)obj_state::die);
					bullet_obj->set_now_pos_and_body(coords[back_destroy_zone]);
				}
			}

			//적 vs 플레이어 충돌 틱
			if (shape::intersect(mon_obj->get_body(), units_.front()->get_now_pos())	!= shape::collide_type::none) {

				units_.front()->set_hp((int)obj_state::die);
			}
		}//for (auto& mon_obj : monsters_)
	}

	void stage::render(win32gdi::device_context_base& dest_dc, const float g_fps) {
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

		//폭발
		for (auto& bomb_obj : bombs_) {
			if (bomb_obj->hp_ == (int)obj_state::idle) {
				frame_image::get().render_transparent(
					back_buffer_
					, bomb_obj->dest_pos_
					, bomb_obj->src_pos_
					, (int)obj_type::bomb
				);
			}
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