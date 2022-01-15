#include "stage.h"
#include "frame_image.h"
#include "player.h"
#include "monster.h"
#include "bullet.h"
#include "item.h"
#include "effect.h"
#include "../stat/simple_stat.h"

namespace zee {
	using namespace math;

	void stage::on_app_started() {
		application_delegates::on_client_size_changed().add_sp(shared_from_this(), &stage::on_resize);
		tick_manager::get().add(shared_from_this());
		on_resize(application::get().config().client_size);
		application::get().window_handle<HWND>();	//윈도우 핸들 가져오기

		//이미지 로드
		//배경
		if (background_type == loop) {
			background_image::get().load_background_image(coords[back_max_size]
				, TEXT("assets/game_background_loop_vertical.bmp"));
		}
		else if (background_type == scroll) {
			background_image::get().load_background_image(coords_stage[back_scroll_max_size]
				, TEXT("assets/game_background_stop.bmp"));
		}

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
		std::shared_ptr<effect> effect_obj = std::make_shared<effect>();
		effect_obj->load_image();
		//아이템
		std::shared_ptr<item> item_obj = std::make_shared<item>();
		item_obj->load_image();


		//게임 초기화
		init_game();


		//행렬 테스트
		/*
		matrix2f m2;
		matrix3f m3;
		//전치행렬
		m2.set_vec({ { 0,1 }, { 3,4 } });
		m2.transposed();
		m2.transposed();
		m3.set_vec({ { 0,1,2 }, { 3,4,5 }, {6,7,8} });
		m3.transposed();
		m3.transposed();
		//행렬식
		m2.set_vec({ {2,3}, {4,6} });
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
		//곱
		matrix3f m4{ {1,2,3},{4,5,6},{7,8,9} };
		matrix3f m5{ {2,3,4},{5,6,7},{8,9,10} };
		m4.mul(m5);
		*/
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

		//배경
		switch (background_type) {
		//@[[likely]] likely and unlikely attributes: c++ 20. 해당 레이블이 실행될 가능성이 높다는 힌트를 제공함.
		[[likely]] case loop:
			set_background_src_pos({});
			set_background_src_size(coords_stage[back_loop_max_size]);
			break;
		case scroll:
			set_background_src_size(coords_stage[back_scroll_default_unit_pos]);	//y축 시작위치 설정
			break;
		}


		///obj 초기화

		//플레이어
		std::shared_ptr<player> spawned_player = std::make_shared<player>();
		spawned_player->init();
		players_.push_back(spawned_player);


		//몬스터
		for (int i = 0; i != monster_spawn_num; i++) {
			std::shared_ptr<monster> spawned_monster = std::make_shared<monster>();
			spawned_monster->init();
			monsters_.push_back(spawned_monster);
		}


		//이펙트
		for (int i = 0; i != effect_spawn_num; i++) {
			std::shared_ptr<effect> spawned_effect = std::make_shared<effect>();
			spawned_effect->init();
			effects_.push_back(spawned_effect);
		}


		//아이템
		for (int i = 0; i != item_spawn_num; i++) {
			std::shared_ptr<item> spawned_item = std::make_shared<item>();
			spawned_item->init();
			items_.push_back(spawned_item);
		}
	}

	void stage::tick(float delta_time) {

		//시간 틱
		set_game_time(get_game_time() + delta_time);

		//배경 틱
		switch (background_type) {
		[[likely]] case loop: {
			const float speed = 120.0f;
			switch (background_direction) {
				//배경 루프 이미지
			case (int)background_dir::left:
			case (int)background_dir::right:
				set_background_src_pos_x(get_background_src_pos().x + delta_time * speed);
				set_background_src_pos_x((float)math::fmod(get_background_src_pos().x, get_background_src_size().x));

				break;
			[[likely]] case (int)background_dir::up:
			case (int)background_dir::down:
				set_background_src_pos_y(get_background_src_pos().y + delta_time * speed);
				set_background_src_pos_y((float)math::fmod(get_background_src_pos().y, get_background_src_size().y));
				break;
			}
			break;
		}//case loop

		case scroll: {
			const float background_speed = 10.0f;
			if (get_players()[0]->get_is_dir_key_pressed()) {
				switch (get_players()[0]->get_direction()) {
					//배경 정지 이미지
				case (int)background_dir::up:
					if (get_players()[0]->get_now_pos().y > 0 && get_background_src_pos().y > 0) {
						set_background_src_pos_y(get_background_src_pos().y - delta_time * background_speed);
					}
					break;
				case (int)background_dir::left:
					if (get_players()[0]->get_now_pos().x > 0 && get_background_src_pos().x > 0) {
						set_background_src_pos_x(get_background_src_pos().x - delta_time * background_speed);
					}
					break;
				case (int)background_dir::down:
					if (get_players()[0]->get_now_pos().y < coords_stage[back_scroll_unit_max_move].y
						&& get_background_src_pos().y < coords_stage[back_scroll_max].y) {
						set_background_src_pos_y(get_background_src_pos().y + delta_time * background_speed);
					}
					break;
				case (int)background_dir::right:
					if (get_players()[0]->get_now_pos().x < coords_stage[back_scroll_unit_max_move].x
						&& get_background_src_pos().x < coords_stage[back_scroll_max].x) {
						set_background_src_pos_x(get_background_src_pos().x + delta_time * background_speed);
					}
					break;
				}
			}
			break;

		}//case scroll

		}//switch (kind_of_background)

		/*
		{//이런식으로 스코프로 감싸서
			simple_stat<> stat;//생성자 호출되면 여기서 초기화를하고.
			//포문 돌고나서 저기서 저 함수 호출하면 시간이 재지거든여? ㅇㅎㅇㅎ
			ZEE_LOG(normal, TEXT("tick check"), TEXT("player tick : %f"), stat.mili_sec());
		}
		*/

		///////
		//플레이어 틱
		///////

		//이동
		get_players()[0]->move(delta_time);

		//발사
		get_players()[0]->shoot(delta_time);

		//충돌: 아이템 vs 플레이어
		for (auto& item_obj : get_items()) {
			if (item_obj->get_state() == (int)obj_state::idle) {
				if (shape::intersect(
					get_players()[0]->get_body()
					, item_obj->get_body())
					!= shape::collide_type::none) 
				{
					item_obj->hit_from(get_players()[0], delta_time);
				}
			}
		}

		//플레이어 사망판단
		if (get_players()[0]->get_hp() == 0 || !(get_players()[0]->in_screen())) {
			get_players()[0]->destroy(delta_time);
		}



		//플레이어 뷸렛 사망판단
		for (auto& bullet_player_obj : get_players()[0]->get_bullets()) {
			if (bullet_player_obj->get_hp() == 0 || !(bullet_player_obj->in_screen())) {
				bullet_player_obj->destroy(delta_time);
			}
		}



		///////
		//적 틱
		///////
		for (auto& mon_obj : get_monsters()) {
			//먼저, 적 어라운드 타입이라면 플레이어 추적 위치 설정
			if (mon_obj->get_obj_type()
				== (int)obj_type::monster_arround)
			{
				//플레이어와의 방향 벡터
				math::vec2f v_mon_for_player{
					get_players()[0]->get_body().origin - mon_obj->get_body().origin
				};
				mon_obj->set_vec_for_player(v_mon_for_player);
			}


			//적 이동
			mon_obj->move(delta_time);

			//적 발사
			mon_obj->shoot(delta_time);


			//충돌: 적 vs 플레이어
			if (shape::intersect(
				mon_obj->get_body(),
				get_players()[0]->get_body())
				!= shape::collide_type::none)
			{
				//이펙트: 피격 판정 전에 먼저 위치 기록
				for (auto& effect_obj : get_effects()) {
					if (effect_obj->get_state() == (int)obj_state::die) {
						effect_obj->spawn_from(get_players()[0]);
						break;
					}
				}

				//피격
				get_players()[0]->hit_from(mon_obj, delta_time);
			}



			//적 뷸렛
			for (auto& bullet_monster_obj : mon_obj->get_bullets()) {
				//적 호밍 뷸렛(유도탄) 타입
				//호밍 뷸렛이 플레이어를 추적
				if (bullet_monster_obj->get_obj_type()
					== (int)obj_type::monster_bullet_homing)
				{
					//플레이어와의 방향 벡터: normalize를 여기서 해줘야 함
					math::vec2f v_bullet_for_player{
						get_players()[0]->get_body().origin - bullet_monster_obj->get_body().origin
					};
					v_bullet_for_player = v_bullet_for_player.normalize();
					bullet_monster_obj->set_vec_for_player(v_bullet_for_player);
				}


				//충돌: 호밍 뷸렛 vs 플레이어 뷸렛
				if (bullet_monster_obj->get_obj_type() == (int)obj_type::monster_bullet_homing) {

					for (auto& bullet_player_obj : get_players()[0]->get_bullets())
					{
						if (shape::intersect(
							bullet_player_obj->get_body(),
							bullet_monster_obj->get_body())
							!= shape::collide_type::none)
						{
 							//이펙트: 피격 판정 전에 먼저 위치 기록
 							for (auto& effect_obj : get_effects()) {
								if (effect_obj->get_state() == (int)obj_state::die) {
									effect_obj->spawn_from(bullet_monster_obj);
									break;
								}
							}

							//피격
							//적 뷸렛은 플레이어 뷸렛에게 맞았다고 판단 (플레이어 뷸렛의 공격력만큼 피해입음)
							//플레이어 뷸렛은 적 뷸렛에게 맞았다고 판단 (적 뷸렛의 공격력만큼 피해입음)
							bullet_monster_obj->hit_from(bullet_player_obj, delta_time);
							bullet_player_obj->hit_from(bullet_monster_obj, delta_time);

						}//if
					}//for
				}//if
				

				//충돌: 적 뷸렛 vs 플레이어
				if (shape::intersect(
					get_players()[0]->get_body(),
					bullet_monster_obj->get_body())
					!= shape::collide_type::none)
				{
					//이펙트: 피격 판정 전에 먼저 위치 기록
					for (auto& effect_obj : get_effects()) {
						if (effect_obj->get_state() == (int)obj_state::die) {
							effect_obj->spawn_from(get_players()[0]);
							break;
						}
					}

					//피격
					//플레이어는 적 뷸렛에게 맞았다고 판단 (뷸렛의 공격력만큼 피해입음)
					get_players()[0]->hit_from(bullet_monster_obj, delta_time);
					bullet_monster_obj->hit_from(get_players()[0], delta_time);
				}



				//적 뷸렛 사망판단
				if (bullet_monster_obj->get_hp() == 0 || !(bullet_monster_obj->in_screen())) {
					bullet_monster_obj->destroy(delta_time);
				}
			}


			//충돌: 플레이어 뷸렛 vs 적
			for (auto& bullet_player_obj : get_players()[0]->get_bullets()) {
				if (shape::intersect(
					mon_obj->get_body(),
					bullet_player_obj->get_body())
					!= shape::collide_type::none)
				{
					//이펙트: 피격 판정 전에 먼저 위치 기록
					for (auto& effect_obj : get_effects()) {
						if (effect_obj->get_state() == (int)obj_state::die) {
							effect_obj->spawn_from(bullet_player_obj);
							break;
						}
					}

					//피격
					//적은 뷸렛이 아닌 플레이어에게 맞았다고 판단(점수 주기 위함)(플레이어의 공격력만큼 피해입음)
					mon_obj->hit_from(get_players()[0], delta_time);
					bullet_player_obj->hit_from(mon_obj, delta_time);
				}
			}



			//몬스터 사망판단
			if (mon_obj->get_hp() == 0 || !(mon_obj->in_screen())) {

				//아이템: 피격 판정 전에 먼저 위치 기록
				for (auto& item_obj : get_items()) {
					if (item_obj->get_state() == (int)obj_state::die) {
						item_obj->spawn_from(mon_obj);
						break;
					}
				}

				//사망
				mon_obj->destroy(delta_time);
			}

		}//for (auto& mon_obj : monsters_)


		//이펙트 사망: 지연시간 다 되면.
		for (auto& effect_obj : get_effects()) {
			if (effect_obj->get_state() == (int)obj_state::idle) {
				effect_obj->destroy(delta_time);
			}
		}

		//점수
		if (get_players()[0]->get_high_score() < get_players()[0]->get_my_score()) {
			get_players()[0]->set_high_score(get_players()[0]->get_my_score());
		}



		//리스폰: 전부 폭파
		if (key_state::is_down(keys::R)) {

			get_players()[0]->spawn();
			for (auto& bullet_player_obj : get_players()[0]->get_bullets()) {
				bullet_player_obj->destroy(delta_time);
			}

			for (auto& monster_obj : get_monsters()) {
				monster_obj->destroy(delta_time);
				for (auto& bullet_monster_obj : monster_obj->get_bullets()) {
					bullet_monster_obj->destroy(delta_time);
				}
			}
			for (auto& effect_obj : get_effects()) {
				effect_obj->destroy(delta_time);
			}
			for (auto& item_obj : get_items()) {
				item_obj->destroy(delta_time);
			}

			get_players()[0]->set_my_score(0);
			set_game_time(0.0f);
		}
	}


	void stage::render(win32gdi::device_context_base& dest_dc, const float g_fps) {
		if (back_buffer_.is_valid()) {
			
			//배경
			switch (background_type) {
			[[likely]] case loop:
				//배경 루프 이미지
				switch (background_direction)
				{
				[[likely]] case (int)background_dir::up:
					background_image::get().render(back_buffer_, -get_background_src_pos());
					background_image::get().render(
						back_buffer_,
						{ get_background_src_pos().x,
						-get_background_src_pos().y + get_background_src_size().y}
					);
					break;

				case (int)background_dir::left:
					background_image::get().render(back_buffer_, get_background_src_pos());
					background_image::get().render(
						back_buffer_,
						{ get_background_src_pos().x - get_background_src_size().x,
						get_background_src_pos().y }
					);
					break;

				case (int)background_dir::down:
					background_image::get().render(back_buffer_, get_background_src_pos());
					background_image::get().render(
						back_buffer_,
						{ get_background_src_pos().x,
						get_background_src_pos().y - get_background_src_size().y }
					);
					break;

				case (int)background_dir::right:
					background_image::get().render(back_buffer_, -get_background_src_pos());
					background_image::get().render(
						back_buffer_,
						{ -get_background_src_pos().x + get_background_src_size().x,
						get_background_src_pos().y }
					);
					break;

				}
				break;

			case scroll:
				background_image::get().render(back_buffer_, get_background_src_pos());
				break;
			}
		}

		//플레이어
		for (auto& player_obj : get_players()) {
			player_obj->render(back_buffer_);
		}
		
		get_monsters()[0]->render(back_buffer_);
		
		//몬스터
		for (auto& mon_obj : get_monsters()) {
			mon_obj->render(back_buffer_);
		}
		
		//아이템 렌더
		for (auto& item_obj : get_items()) {
			item_obj->render(back_buffer_);
		}

		//이펙트 렌더
		for (auto& bomb_obj : get_effects()) {
			bomb_obj->render(back_buffer_);
		}


		//인포박스
		shape::rectf box(700, 0, 1200, 1100);
		back_buffer_.rectangle(box);
		shape::rectf box_info(830, 700, 1030, 900);
		back_buffer_.rectangle(box_info);
		back_buffer_.print_text({ 850, 720 }, TEXT("프레임: "));
		back_buffer_.print_text({ 930, 720 }, to_tstring(to_wstring(g_fps)));
		back_buffer_.print_text({ 850, 740 }, TEXT("시간: "));
		back_buffer_.print_text({ 930, 740 }, to_tstring(to_wstring(static_cast<int>(get_game_time()))));
		back_buffer_.print_text({ 850, 760 }, TEXT("점수: "));
		back_buffer_.print_text({ 930, 760 }, to_tstring(to_wstring(get_players()[0]->get_my_score())));
		back_buffer_.print_text({ 850, 780 }, TEXT("최고점수: "));
		back_buffer_.print_text({ 930, 780 }, to_tstring(to_wstring(get_players()[0]->get_high_score())));
		back_buffer_.print_text({ 850, 820 }, TEXT("충돌범위:    Tab"));
		back_buffer_.print_text({ 850, 840 }, TEXT("리스폰:        R"));

		
		//한 번에 그리기
		back_buffer_.bit_blt(dest_dc, {});
	}

	const math::vec2f stage::get_background_src_pos() const {
		return background_src_pos_;
	}
	const math::vec2f stage::get_background_src_size() const {
		return background_src_size_;
	}
	const std::vector<std::shared_ptr<player>> stage::get_players() const {
		return players_;
	}
	const std::vector<std::shared_ptr<monster>> stage::get_monsters() const {
		return monsters_;
	}
	const std::vector<std::shared_ptr<effect>> stage::get_effects() const {
		return effects_;
	}
	const std::vector<std::shared_ptr<item>> stage::get_items() const {
		return items_;
	}
	const float stage::get_game_time() const {
		return game_time_;
	}

	void stage::set_background_src_pos(const math::vec2f& src_pos) {
		background_src_pos_ = src_pos;
	}
	void stage::set_background_src_pos_x(const float x) {
		background_src_pos_.x = x;
	}
	void stage::set_background_src_pos_y(const float y) {
		background_src_pos_.y = y;
	}
	void stage::set_background_src_size(const math::vec2f& size) {
		background_src_size_ = size;
	}
	void stage::set_game_time(const float game_time) {
		game_time_ = game_time;
	}
}