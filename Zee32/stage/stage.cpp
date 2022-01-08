#include "stage.h"

namespace zee {
	using namespace math;

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
		//플레이어
		std::shared_ptr<player> player_obj = std::make_shared<player>();
		player_obj->load_image();
		//몬스터
		std::shared_ptr<monster> monster_obj = std::make_shared<monster>();
		monster_obj->load_image();
		//뷸렛
		std::shared_ptr<bullet> bullet_obj = std::make_shared<bullet>();
		bullet_obj->load_image();

		//이펙트
		std::shared_ptr<effect> bomb_obj = std::make_shared<effect>();
		bomb_obj->load_image();
		//아이템
		std::shared_ptr<item> item_obj = std::make_shared<item>();
		item_obj->load_image();

		//게임 초기화
		init_game();

		//행렬 테스트
		matrix2f m2;
		matrix3f m3;
		//곱
		//plg_blt 참고
		//전치행렬
		m2.set_vec({ { 0,1 }, { 3,4 } });
		m2.transposed();
		m2.transposed();
		m3.set_vec({ { 0,1,2 }, { 3,4,5 }, {6,7,8} });
		m3.transposed();
		m3.transposed();
		//행렬식
		m2.set_vec({{2,3}, {4,6}});
		float a2 = m2.determinant();
		m3.set_vec({ {2,3,4}, {5,6,7}, {8,9,10} });
		float a3 = m3.determinant();
		//항등행렬
		m2.set_vec({ {1,0}, {0,1} });
		bool b2 = m2.is_identity();
		m3.set_vec({ {1,0,0}, {0,1,0}, {0,0,1} });
		bool b3 = m3.is_identity();
		//역행렬
		std::vector<math::vec2f> vv2 = { {2,3}, {4,5} };
		m2.inverse(vv2);
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

		switch (background_type) {
		case loop:
			background_src_pos_ = { 0.0f, 0.0f };
			background_src_size_ = coords[back_loop_max_size];
			break;
		case scroll:
			background_src_pos_ = coords[back_scroll_default_unit_pos];	//y축 시작위치 설정
			break;
		}

		//유닛 스폰
		std::shared_ptr<player> spawned_player = std::make_shared<player>();
		players_.push_back(spawned_player);
		//유닛 초기화
		players_.front()->init();

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
	}

	void stage::tick(float delta_time) {
		//시간 틱
		game_time_ += delta_time;

		//배경 틱
		switch (background_type) {
		case loop: {
			const float speed = 120.0f;
			switch (background_direction) {
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
			if (players_.front()->get_is_dir_key_pressed()) {
				switch (players_.front()->get_direction()) {
					//배경 정지 이미지
				case 0:
					if (players_.front()->get_now_pos().y > 0 && background_src_pos_.y > 0) {
						background_src_pos_.y -= delta_time * background_speed;
					}
					break;
				case 1:
					if (players_.front()->get_now_pos().x > 0 && background_src_pos_.x > 0) {
						background_src_pos_.x -= delta_time * background_speed;
					}
					break;
				case 2:
					if (players_.front()->get_now_pos().y < coords[back_scroll_unit_max_move].y
						&& background_src_pos_.y < coords[back_scroll_max].y) {
						background_src_pos_.y += delta_time * background_speed;
					}
					break;
				case 3:
					if (players_.front()->get_now_pos().x < coords[back_scroll_unit_max_move].x
						&& background_src_pos_.x < coords[back_scroll_max].x) {
						background_src_pos_.x += delta_time * background_speed;
					}
					break;
				}
			}
			break;
		}//case scroll
		}//switch (kind_of_background)

		//플레이어 틱
		for (auto& player_obj : players_) {
			player_obj->move(delta_time);
			player_obj->shoot(delta_time);
			player_obj->destroy(delta_time);

			//아이템 vs 플레이어 충돌
			for (auto& item_obj : items_) {
				item_obj->destroy(delta_time);
				if (shape::intersect(player_obj->get_body(), item_obj->get_body())
					!= shape::collide_type::none) 
				{
					item_obj->hit_from(player_obj, delta_time);
				}
			}
		}
		for (auto& mon_obj : monsters_) {
			//적 틱
			mon_obj->move(delta_time);
			mon_obj->shoot(delta_time);
			mon_obj->destroy(delta_time);

			//적 호밍
			if (mon_obj->get_obj_type() 
				== (int)unit::obj_type::monster_arround) 
			{
				//플레이어와의 방향 벡터
				math::vec2f v_mon_for_player{
					players_.front()->get_body().origin - mon_obj->get_body().origin
				};
				mon_obj->set_vec_for_player(v_mon_for_player);
			}

			//적 호밍탄(유도탄)
			for (auto& bullet_obj : mon_obj->get_bullets()) {
				//호밍 로직
				if (bullet_obj->get_obj_type() 
					== (int)unit::obj_type::monster_bullet_homing) 
				{
					//플레이어와의 방향 벡터: normalize를 여기서 해줘야 함
					math::vec2f v_bullet_for_player{
						players_.front()->get_body().origin - bullet_obj->get_body().origin
					};
					v_bullet_for_player = v_bullet_for_player.normalize();
					bullet_obj->set_vec_for_player(v_bullet_for_player);
				}

				//적 뷸렛(특수한 뷸렛만) vs 플레이어 뷸렛 충돌 틱
				if (bullet_obj->get_obj_type() == (int)unit::obj_type::monster_bullet_homing) {

					for (auto& bullet_player_obj : players_.front()->get_bullets())
					{
						if (shape::intersect(bullet_player_obj->get_body(), bullet_obj->get_body())
							!= shape::collide_type::none)
						{
							//폭발 이펙트: 피격 판정 전에 먼저 위치 기록
							spawn_bomb(bullet_obj);

							//피격
							//적 뷸렛은 플레이어 뷸렛에게 맞았다고 판단 (플레이어 뷸렛의 공격력만큼 피해입음)
							//플레이어 뷸렛은 적 뷸렛에게 ~
							bullet_obj->hit_from(bullet_player_obj, delta_time);
							bullet_player_obj->hit_from(bullet_obj, delta_time);
							bullet_obj->set_now_pos_and_body(coords[back_destroy_zone]);
							bullet_player_obj->set_now_pos_and_body(coords[back_destroy_zone]);
						}//if
					}//for
				}//if
			}


			//플레이어 뷸렛 vs 적 충돌 틱
			for (auto& bullet_obj : players_.front()->get_bullets()) {
				if (shape::intersect(mon_obj->get_body(), bullet_obj->get_body()) 
					!= shape::collide_type::none) 
				{
					//폭발 이펙트: 피격 판정 전에 먼저 위치 기록
					spawn_bomb(mon_obj);
					//아이템
					spawn_item(mon_obj);

					//피격
					//적은 뷸렛이 아닌 플레이어에게 맞았다고 판단(점수 주기 위함)(플레이어의 공격력만큼 피해입음)
					mon_obj->hit_from(players_.front(), delta_time);
					bullet_obj->hit_from(mon_obj, delta_time);
				}
			}

			//적 뷸렛 vs 플레이어 충돌 틱
			for (auto& bullet_obj : mon_obj->get_bullets()) {
				if (shape::intersect(players_.front()->get_body(), 
					bullet_obj->get_body())
					!= shape::collide_type::none) 
				{
					//폭발 이펙트: 피격 판정 전에 먼저 위치 기록
					spawn_bomb(players_.front());

					//피격
					//플레이어는 적 뷸렛에게 맞았다고 판단 (뷸렛의 공격력만큼 피해입음)
					players_.front()->hit_from(bullet_obj, delta_time);
					bullet_obj->hit_from(players_.front(), delta_time);
					bullet_obj->set_now_pos_and_body(coords[back_destroy_zone]);
				}
			}

			//적 vs 플레이어 충돌 틱
			if (shape::intersect(mon_obj->get_body(), 
				players_.front()->get_now_pos())
				!= shape::collide_type::none) 
			{
				//폭발 이펙트: 피격 판정 전에 먼저 위치 기록
				spawn_bomb(players_.front());

				//피격 (몬스터의 공격력만큼 피해입음)
				players_.front()->hit_from(mon_obj, delta_time);

			}
		}//for (auto& mon_obj : monsters_)


		//이펙트 틱
		for (auto& bomb_obj : bombs_) {
			bomb_obj->destroy(delta_time);
		}

		//이펙트 제거
		for (int i = 0; i != bombs_.size(); ) {
			if (!(bombs_[i]->in_screen()) || bombs_[i]->get_state() == (int)unit::obj_state::die) {
				bombs_.erase(bombs_.begin() + i);
			}
			else {
				i++;
			}
		}

		//아이템 제거
		for (int i = 0; i != items_.size(); ) {
			if (!(items_[i]->in_screen()) || items_[i]->get_state() == (int)unit::obj_state::die) {
				items_.erase(items_.begin() + i);
			}
			else {
				i++;
			}
		}
	}
	void stage::spawn_bomb(std::shared_ptr<unit> other) {
		std::shared_ptr<effect> spawned_bomb = std::make_shared<effect>();
		spawned_bomb->init();
		spawned_bomb->set_now_pos_and_body(
			{ other->get_body().origin.x - other->get_body().radius,
			other->get_body().origin.y - other->get_body().radius }
		);
		bombs_.push_back(spawned_bomb);
	}
	void stage::spawn_item(std::shared_ptr<unit> other) {
		std::shared_ptr<item> spawned_item = std::make_shared<item>();
		spawned_item->init();
		spawned_item->set_now_pos_and_body(
			{ other->get_body().origin.x - other->get_body().radius,
			other->get_body().origin.y - other->get_body().radius }
		);
		items_.push_back(spawned_item);
	}


	void stage::render(win32gdi::device_context_base& dest_dc, const float g_fps) {
		if (back_buffer_.is_valid()) {
			switch (background_type) {
			case loop:
				//배경 루프 이미지
				//direction 이미지 기준 0: 좌->우.. 1: 우->좌.. 2: 상->하.. 3: 하->상
				switch (background_direction)
				{
				case 0:
					background_image::get().render(back_buffer_, -background_src_pos_);
					background_image::get().render(
						back_buffer_,
						{ -background_src_pos_.x + background_src_size_.x,
						background_src_pos_.y }
					);
					break;
				case 1:
					background_image::get().render(back_buffer_, background_src_pos_);
					background_image::get().render(
						back_buffer_,
						{ background_src_pos_.x - background_src_size_.x,
						background_src_pos_.y }
					);
					break;
				case 2:
					background_image::get().render(back_buffer_, -background_src_pos_);
					background_image::get().render(
						back_buffer_,
						{ background_src_pos_.x,
						-background_src_pos_.y + background_src_size_.y }
					);
					break;
				case 3:
					background_image::get().render(back_buffer_, background_src_pos_);
					background_image::get().render(
						back_buffer_,
						{ background_src_pos_.x,
						background_src_pos_.y - background_src_size_.y }
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
		for (auto& player_obj : players_) {
			player_obj->render(back_buffer_);
		}
		//몬스터
		for (auto& mon_obj : monsters_) {
			mon_obj->render(back_buffer_);
		}

		//뷸렛: 유닛과 몬스터 render에 포함함

		//이펙트
		for (auto& bomb_obj : bombs_) {
			bomb_obj->render(back_buffer_);
		}

		//아이템
		for (auto& item_obj : items_) {
			item_obj->render(back_buffer_);
		}


		//인포박스
		shape::rectf box_info(830, 700, 1030, 880);
		back_buffer_.rectangle(box_info);
		back_buffer_.print_text({ 850, 720 }, TEXT("프레임: "));
		back_buffer_.print_text({ 920, 720 }, to_tstring(to_wstring(g_fps)));
		back_buffer_.print_text({ 850, 740 }, TEXT("시간: "));
		back_buffer_.print_text({ 920, 740 }, to_tstring(to_wstring(static_cast<int>(game_time_))));
		back_buffer_.print_text({ 850, 760 }, TEXT("점수: "));
		back_buffer_.print_text({ 920, 760 }, to_tstring(to_wstring(players_.front()->get_my_score())));
		back_buffer_.print_text({ 850, 800 }, TEXT("충돌범위:    Tab"));
		back_buffer_.print_text({ 850, 820 }, TEXT("리스폰:        R"));

		back_buffer_.bit_blt(dest_dc, {});
	}

	const math::vec2f stage::get_background_src_pos() const {
		return background_src_pos_;
	}
	const math::vec2f stage::get_background_src_size() const {
		return background_src_size_;
	}
	void stage::set_background_src_pos(const math::vec2f& src_pos) {
		background_src_pos_ = src_pos;
	}
	void stage::set_background_src_size(const math::vec2f& size) {
		background_src_size_ = size;
	}
}