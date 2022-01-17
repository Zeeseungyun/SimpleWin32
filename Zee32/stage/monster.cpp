#include "monster.h"
#include "player.h"

namespace zee {
	using namespace math;

	void monster::load_image() {
		frame_image::get().load_frame_image(coords_[monster_straight_size]
			, TEXT("assets/monster_straight.bmp"), (int)obj_type::monster_straight);
		frame_image::get().load_frame_image(coords_[monster_circle_size]
			, TEXT("assets/monster_circle.bmp"), (int)obj_type::monster_circle);
		frame_image::get().load_frame_image(coords_[monster_homing_size]
			, TEXT("assets/monster_homing.bmp"), (int)obj_type::monster_homing);
		frame_image::get().load_frame_image(coords_[monster_wave_size]
			, TEXT("assets/monster_wave.bmp"), (int)obj_type::monster_wave);



		//회전
		frame_image::get().load_plg_image(coords_[monster_arround_size]
			, TEXT("assets/monster_arround.bmp"), (int)obj_type::monster_arround);


		//회전: 모든 각도의 이미지 미리 찍기
		for (float degree = 0.0f; degree != 360; degree++) {
			//회전할 때 src_pos 밖의 이미지가 자꾸 침범해서 간격을 만듦
			math::vec2f point;
			point.x =
				degree < 1.0f ?
				coords_[monster_arround_size].x / 2 :
				coords_[monster_arround_size].x / 2 + degree * (coords_[monster_arround_size].x + coords_[monster_arround_size].x / 2);

			point.y = static_cast<float>(coords_[monster_arround_size].y / 2);

			frame_image::get().render_plg(
				point,
				math::deg_to_rad(degree),
				(int)obj_type::monster_arround
			);
		}
	}

	void monster::init() {
		//몬스터는 랜덤 스폰으로 init함수와 spawn함수가 절반 이상 같다.
		monster::spawn();
		set_my_score(1);

		//몬스터 뷸렛
		for (int i = 0; i != monster_bullet_max_num; i++) {
			std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();
			spawned_bullet->init();
			bullets_.push_back(spawned_bullet);
		}
	}

	void monster::spawn() {
		plane::init();
		set_my_score(1);
		set_delay(0.0f);

		//monster_num
		switch (int random_shoot_type = rand(1, 5))
		{
		case 1: set_obj_type((int)obj_type::monster_straight);
			break;
		case 2: set_obj_type((int)obj_type::monster_circle);
			break;
		case 3: set_obj_type((int)obj_type::monster_homing);
			break;
		case 4: set_obj_type((int)obj_type::monster_arround);
			break;
		case 5: set_obj_type((int)obj_type::monster_wave);
			break;
		}

		//테스트	--------------------------------------------------------
		//set_obj_type((int)obj_type::monster_wave);


		switch (get_obj_type())
		{
		case (int)obj_type::monster_straight:
			set_size(coords_[monster_straight_size]);
			break;
		case (int)obj_type::monster_circle:
			set_size(coords_[monster_circle_size]);
			break;
		case (int)obj_type::monster_homing:
			set_size(coords_[monster_homing_size]);
			break;
		case (int)obj_type::monster_arround:
			set_size(coords_[monster_arround_size]);
			break;
		case (int)obj_type::monster_wave:
			set_size(coords_[monster_wave_size]);
			set_hp(3);
			set_wave_angle(0.0f);
			set_wave_angle_numer(0.0f);
			break;
		}

		int rx = rand(coords_[monster_min_pos].x, coords_[monster_max_pos].x);
		int ry = coords_[monster_min_pos].y;
		set_now_pos_and_body({ static_cast<float>(rx), static_cast<float>(ry) });

		rx = rand(coords_[monster_min_pos].x, coords_[monster_max_pos].x);
		ry = coords_[monster_max_pos].y;
		set_arrival_vec({ static_cast<float>(rx), static_cast<float>(ry) });
	}

	void monster::move(const float delta_time) {
		if (in_screen()) {
			switch (get_obj_type())
			{
			case (int)obj_type::monster_straight:
			case (int)obj_type::monster_circle:
			case (int)obj_type::monster_homing: {
				set_speed(150.0f);
				//이동 방향 벡터는 첨에 스폰 시 생성
				math::vec2f v{
					get_body().origin - get_arrival_vec()
				};
				//내적->거리->단위화
				set_arrival_vec(arrival_vec_.normalize());
				set_now_pos_and_body(get_now_pos() + get_arrival_vec() * delta_time * get_speed());
				break;
			}

			case (int)obj_type::monster_arround: {
				//플레이어 따라오다가 y축 저점에서 그냥 아래로 사라지게 하자.
				if (get_now_pos().y < coords[back_max_size].y * 2 / 3) {
					//회전각
					if (get_obj_type() == (int)obj_type::monster_arround) {
						set_homing_angle(math::atan2(get_vec_for_player().x, get_vec_for_player().y));
						//rad to degree
						set_homing_degree(rad_to_deg(get_homing_angle()));
						if (get_homing_degree() < 0) {
							set_homing_degree(get_homing_degree() + 360);
						}
					}
					//이동
					set_speed(200.0f);
					set_vec_for_player(get_vec_for_player().normalize());
					set_now_pos_and_body(get_now_pos() + get_vec_for_player() * delta_time * get_speed());
					//아래 행렬 이동->곱과 같음
					/*matrix m;
					m.translation(vec_for_player_.x * delta_time * speed, vec_for_player_.y * delta_time * speed);
					m.mul(now_pos_);
					set_now_pos_and_body({ m.get_mf()[0][0], m.get_mf()[0][1] });*/
				}
				else {
					set_speed(250.0f);
					set_now_pos_and_body({ get_now_pos().x, get_now_pos().y + delta_time * get_speed() });
					set_homing_angle(0.0f);
					set_homing_degree(0.0f);
				}
				break;
			}

			case (int)obj_type::monster_wave: {
				if (get_now_pos() != coords_[monster_boss_pos]) {
					set_speed(150.0f);
					set_now_pos_and_body({ get_now_pos().x, get_now_pos().y + delta_time * get_speed() });
				}
				break;
			}//case
			}//switch
		}
	}

	//뷸렛 쏘기
	void monster::shoot(const float delta_time) {
		if (in_screen()
			&& get_state() == (int)obj_state::idle) {
			switch (get_obj_type()) {

			case (int)obj_type::monster_straight: {
				//직선탄
				constexpr float frame_straight = 1.5f;
				set_delay_shoot(get_delay_shoot() + delta_time);
				plane::shoot_per_delay(frame_straight);
				break;
			}

			case (int)obj_type::monster_circle: {
				//원형탄
				constexpr float frame_circle = 3.0f;
				set_delay_shoot(get_delay_shoot() + delta_time);
				if (get_delay_shoot() >= frame_circle) {
					float circle_angle = 0;
					constexpr int bullet_circle_cnt = 10;
					for (int i = 0; i != bullet_circle_cnt; i++) {

						circle_angle += math::pi() * 2 / static_cast<float>(bullet_circle_cnt);

						for (auto& bullet_obj : get_bullets()) {
							if (bullet_obj->get_state() == (int)obj_state::die) {
								bullet_obj->spawn_from(get_obj_type(), get_body());
								bullet_obj->set_circle_angle(circle_angle);
								break;
							}
						}
					}
				}
				set_delay_shoot((float)math::fmod(get_delay_shoot(), frame_circle));
				break;
			}

			case (int)obj_type::monster_homing: {
				//유도탄
				constexpr float frame_homing = 2.0f;
				set_delay_shoot(get_delay_shoot() + delta_time);
				plane::shoot_per_delay(frame_homing);
				break;
			}

			case (int)obj_type::monster_arround: {
				//어라운드
				constexpr float frame_arround = 1.5f;
				set_delay_shoot(get_delay_shoot() + delta_time);
				plane::shoot_per_delay(frame_arround);
				break;
			}

			case (int)obj_type::monster_wave: {
				//웨이브
				constexpr float frame_wave = 0.3f;
				set_delay_shoot(get_delay_shoot() + delta_time);
				if (get_delay_shoot() >= frame_wave) {

					set_wave_angle_numer(get_wave_angle_numer() + 1.0f);

					set_wave_angle(
						(get_wave_angle_numer() == 0) ?
						get_wave_angle() :
						get_wave_angle() + math::pi() * 2 / get_wave_angle_numer()
					);

					for (auto& bullet_obj : get_bullets()) {
						if (bullet_obj->get_state() == (int)obj_state::die) {
							bullet_obj->spawn_from(get_obj_type(), get_body());
							bullet_obj->set_circle_angle(get_wave_angle());
							break;
						}
					}
					set_wave_angle((float)math::fmod(get_wave_angle(), 20.0f));
					set_wave_angle_numer((float)math::fmod(get_wave_angle(), 20.0f));


				}
				set_delay_shoot((float)math::fmod(get_delay_shoot(), frame_wave));
				break;

			}//case

			}//switch


			plane::shoot(delta_time);
		}
	}


	void monster::hit_from(const std::shared_ptr<unit> other, const float delta_time) {
		//other이 player를 가리키고 nullptr이 아니면
		if (std::shared_ptr<player> other_player = std::dynamic_pointer_cast<player>(other)) {
			//히트
			unit::hit_from(other, delta_time);
			other_player->add_score(get_my_score());
		}
		else {
			assert(false);
		}
	}

	void monster::destroy(const float delta_time) {
		//공통
		unit::destroy(delta_time);
		spawn();
	}
	

	void monster::render(win32gdi::device_context_dynamic& dest_dc) {
		if (in_screen() 
			&& get_state() == (int)obj_state::idle) {

			//몸체
			if (get_state() != (int)obj_state::die) {
				switch (get_obj_type()) {
				case (int)obj_type::monster_straight:
				case (int)obj_type::monster_circle:
				case (int)obj_type::monster_homing:
				case (int)obj_type::monster_wave: {
					frame_image::get().render_transparent(
						dest_dc,
						get_now_pos(),
						get_frame_x() + get_frame_y(),
						get_obj_type()
					);
					break;
				}
				case (int)obj_type::monster_arround: {
					//회전할 때 src_pos 밖의 이미지가 자꾸 침범해서 간격을 만듦
					math::vec2i src_pos;
					src_pos.x = 
						(get_size().x + get_size().x / 2) * decltype(src_pos.x)(get_homing_degree());

					frame_image::get().render_plg_transparent(
						dest_dc,
						get_now_pos(),
						src_pos,
						get_obj_type()
					);
					break;
				}//case

				}//switch
			}//if
		}//if

		//충돌범위 렌더
		unit::render(dest_dc);

		//몬스터 뷸렛
		plane::render(dest_dc);
	}

	const math::vec2f& monster::get_arrival_vec() const {
		return arrival_vec_;
	}
	const math::vec2f& monster::get_vec_for_player() const {
		return vec_for_player_;
	}
	float monster::get_homing_angle() const {
		return homing_angle_;
	}
	float monster::get_homing_degree() const {
		return homing_degree_;
	}
	float monster::get_wave_angle() const {
		return wave_angle_;
	}
	float monster::get_wave_angle_numer() const {
		return wave_angle_numer_;
	}

	void monster::set_arrival_vec(const math::vec2f& vec) {
		arrival_vec_ = vec;
	}
	void monster::set_vec_for_player(const math::vec2f& v) {
		vec_for_player_ = v;
	}
	void monster::set_homing_angle(const float angle) {
		homing_angle_ = angle;
	}
	void monster::set_homing_degree(const float degree) {
		homing_degree_ = degree;
	}
	void monster::set_wave_angle(const float angle) {
		wave_angle_ = angle;
	}
	void monster::set_wave_angle_numer(const float angle) {
		wave_angle_numer_ = angle;
	}
	void monster::set_delay(const float delay) {
		delay_shoot_ = delay;
		delay_destroy_ = delay;
	}
}