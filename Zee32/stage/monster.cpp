#include "monster.h"
#include "player.h"

namespace zee {
	using namespace math;

	void monster::load_image() {
		frame_image::get().load_frame_image(unit::coords[back_max_size], coords_[monster_straight_size]
			, TEXT("assets/monster_straight.bmp"), (int)obj_type::monster_straight);
		frame_image::get().load_frame_image(unit::coords[back_max_size], coords_[monster_circle_size]
			, TEXT("assets/monster_circle.bmp"), (int)obj_type::monster_circle);
		frame_image::get().load_frame_image(unit::coords[back_max_size], coords_[monster_homing_size]
			, TEXT("assets/monster_homing.bmp"), (int)obj_type::monster_homing);
		frame_image::get().load_frame_image(unit::coords[back_max_size], coords_[monster_arround_size]
			, TEXT("assets/monster_arround.bmp"), (int)obj_type::monster_arround);
		frame_image::get().load_frame_image(unit::coords[back_max_size], coords_[monster_wave_size]
			, TEXT("assets/monster_wave.bmp"), (int)obj_type::monster_wave);
	}

	void monster::init() {

		int random_shoot_type = rand(1, 5);	//monster_num
		switch (random_shoot_type)
		{
		case 1: obj_type_ = (int)obj_type::monster_straight;
			break;
		case 2: obj_type_ = (int)obj_type::monster_circle;
			break;
		case 3: obj_type_ = (int)obj_type::monster_homing;
			break;
		case 4: obj_type_ = (int)obj_type::monster_arround;
			break;
		case 5: obj_type_ = (int)obj_type::monster_wave;
			break;
		}


		//테스트	--------------------------------------------------------
		//obj_type_ = (int)obj_shoot_type::;



		switch (obj_type_)
		{
		case (int)obj_type::monster_straight:
			set_size(coords_[monster_straight_size]);
			set_hp(1);
			break;
		case (int)obj_type::monster_circle:
			set_size(coords_[monster_circle_size]);
			set_hp(1);
			break;
		case (int)obj_type::monster_homing:
			set_size(coords_[monster_homing_size]);
			set_hp(1);
			break;
		case (int)obj_type::monster_arround:
			set_size(coords_[monster_arround_size]);
			set_hp(1);
			break;
		case (int)obj_type::monster_wave:
			set_size(coords_[monster_wave_size]);
			set_hp(3);
			break;
		}

		int rx = rand(coords_[monster_min_pos].x, coords_[monster_max_pos].x);
		int ry = coords_[monster_min_pos].y;
		set_now_pos_and_body({ (float)rx, (float)ry });

		rx = rand(coords_[monster_min_pos].x, coords_[monster_max_pos].x);
		ry = coords_[monster_max_pos].y;
		set_arrival_vec({ (float)rx, (float)ry });

		set_atk(1);
		set_state((int)obj_state::idle);
		set_delay(0.0f);
		set_my_score(1);
	}

	void monster::move(const float delta_time) {
		float speed;
		switch (obj_type_) 
		{
		case (int)obj_type::monster_straight:
		case (int)obj_type::monster_circle:
		case (int)obj_type::monster_homing: {
			speed = 100.0f;
			//이동 방향 벡터는 첨에 스폰 시 생성
			math::vec2f v{
				body_.origin - arrival_vec_
			};
			//내적->거리->단위화
			arrival_vec_ =  arrival_vec_.normalize();
			set_now_pos_and_body(now_pos_ + arrival_vec_ * delta_time * speed);
			break;
		}
		case (int)obj_type::monster_arround: {
			//플레이어 따라오다가 y축 저점에서 그냥 아래로 사라지게 하자.
			if (now_pos_.y < unit::coords[back_max_size].y * 2 / 3) {
				//회전각
				if (obj_type_ == (int)obj_type::monster_arround) {
					homing_angle_ = math::atan2(vec_for_player_.x, vec_for_player_.y);
				}
				//이동
				speed = 150.0f;
				vec_for_player_ = vec_for_player_.normalize();
				set_now_pos_and_body(now_pos_ + vec_for_player_ * delta_time * speed);
				//아래 행렬 이동->곱과 같음
				/*matrix m;
				m.translation(vec_for_player_.x * delta_time * speed, vec_for_player_.y * delta_time * speed);
				m.mul(now_pos_);
				set_now_pos_and_body({ m.get_mf()[0][0], m.get_mf()[0][1] });*/
			}
			else {
				speed = 200.0f;
				set_now_pos_and_body({ now_pos_.x, now_pos_.y + delta_time * speed });
				homing_angle_ = 0.0f;
			}
			break;
		}
		case (int)obj_type::monster_wave: {
			if (now_pos_ != coords_[monster_boss_pos]) {
				speed = 100.0f;
				set_now_pos_and_body({ now_pos_.x, now_pos_.y + delta_time * speed });
			}
			break;
		}//case
		}//switch
	}

	void monster::shoot(const float delta_time) {
		switch (obj_type_)
		{
		case (int)obj_type::monster_straight: {
			//직선탄
			const float frame_straight = 1.5f;
			delay_shoot += delta_time;
			if (delay_shoot >= frame_straight) {
				std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();
				spawned_bullet->init_bullet(obj_type_, now_pos_, size_);
				bullets_.push_back(spawned_bullet);
			}
			delay_shoot = (float)math::fmod(delay_shoot, frame_straight);
			break;
		}
		case (int)obj_type::monster_circle: {
			//원형탄
			const float frame_circle = 3.0f;
			delay_shoot += delta_time;
			if (delay_shoot >= frame_circle) {
				float circle_angle = 0;
				const int bullent_circle_cnt = 10;
				for (int i = 0; i != bullent_circle_cnt; i++) {
					circle_angle += math::pi() * 2 / (float)bullent_circle_cnt;

					std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();
					spawned_bullet->set_circle_angle(circle_angle);
					spawned_bullet->init_bullet(obj_type_, now_pos_, size_);
					bullets_.push_back(spawned_bullet);
				}
			}
			delay_shoot = (float)math::fmod(delay_shoot, frame_circle);
			break;
		}
		case (int)obj_type::monster_homing: {
			//유도탄
			const float frame_homing = 2.0f;
			delay_shoot += delta_time;
			if (delay_shoot >= frame_homing) {
				std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();
				spawned_bullet->init_bullet(obj_type_, now_pos_, size_);
				bullets_.push_back(spawned_bullet);
			}
			delay_shoot = (float)math::fmod(delay_shoot, frame_homing);
			break;
		}
		case (int)obj_type::monster_arround: {
			//어라운드
			const float frame_arround = 1.5f;
			delay_shoot += delta_time;
			if (delay_shoot >= frame_arround) {
				std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();
				spawned_bullet->init_bullet(obj_type_, now_pos_, size_);
				bullets_.push_back(spawned_bullet);
			}
			delay_shoot = (float)math::fmod(delay_shoot, frame_arround);
			break;
		}
		case (int)obj_type::monster_wave: {
			//웨이브
			const float frame_wave = 3.0f;
			delay_shoot += delta_time;
			if (delay_shoot >= frame_wave) {
				float circle_angle = 0;
				int bullent_circle_cnt = 50;

				//얘여
				for (int i = 0; i != bullent_circle_cnt; i++) {
					circle_angle += math::pi() * 2 / (float)bullent_circle_cnt;

					std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();
					spawned_bullet->set_circle_angle(circle_angle);
					spawned_bullet->init_bullet(obj_type_, now_pos_, size_);
					bullets_.push_back(spawned_bullet);
				}
			}
			delay_shoot = (float)math::fmod(delay_shoot, frame_wave);
			break;
		}//case
		}//switch

		//뷸렛 틱
		for (auto& bullet_obj : bullets_) {
			bullet_obj->move(delta_time);
			bullet_obj->destroy(delta_time);
		}
	}

	void monster::hit_from(const std::shared_ptr<unit> other, const float delta_time) {
		//other이 player를 가리키고 nullptr이 아니면
		if (std::shared_ptr<player> other_player = std::dynamic_pointer_cast<player>(other)) {
			unit::hit_from(other, delta_time);
			other_player->add_score(my_score_);
		}
	}

	void monster::destroy(const float delta_time) {
		unit::destroy(delta_time);
		if (hp_ <= 0) {
 			set_now_pos_and_body(unit::coords[back_destroy_zone]);

			//일정시간 후 스폰
			const float speed = 2.0f;
			const float frame = 3.0f;
			delay_destroy_ += delta_time * speed;

			if (delay_destroy_ >= frame) {
				init();
			}
		}

		//뷸렛이 밖에 있으면 제거 (추후 다른 함수로 빼기)
		for (int i = 0; i != bullets_.size(); ) {
			if (!(bullets_[i]->in_screen())) {
				bullets_.erase(bullets_.begin() + i);
			}
			else {
				i++;
			}
		}
	}
	//어디임 총알 스폰?
	void monster::render(win32gdi::device_context_dynamic& dest_dc) {
		if (in_screen()) {
			//백버퍼 관리가 이상하게 되고있는건데


			//몸체
			if (state_ != (int)obj_state::die) {
				switch (obj_type_)
				{
				case (int)obj_type::monster_straight:
				case (int)obj_type::monster_circle:
				case (int)obj_type::monster_homing:
				case (int)obj_type::monster_wave: {
					frame_image::get().render_transparent(
						dest_dc,
						now_pos_,
						{},
						obj_type_
					);
					break;
				}
				case (int)obj_type::monster_arround: {
					frame_image::get().render_plg(
						dest_dc,
						body_.origin,
						homing_angle_,
						obj_type_
					);
					frame_image::get().render_transparent_backbuffer_to_destdc(dest_dc, {}, obj_type_);
					break;
				}//case

				}//switch
			}//if
		}//if


		//몬스터가 스크린 밖에 있을 때도 계속 노출될 것들
		//뷸렛 렌더
		for (auto& bullet_obj : bullets_) {
			bullet_obj->render(dest_dc);
		}

		//충돌범위 테스트
		unit::render(dest_dc);
	}

	const math::vec2f& monster::get_arrival_vec() const {
		return arrival_vec_;
	}
	const std::vector<std::shared_ptr<bullet>> monster::get_bullets() const {
		return bullets_;
	}
	const math::vec2f& monster::get_vec_for_player() const {
		return vec_for_player_;
	}

	void monster::set_arrival_vec(const math::vec2f& vec) {
		arrival_vec_ = vec;
	}
	void monster::set_vec_for_player(const math::vec2f& v) {
		vec_for_player_ = v;
	}
	void monster::set_delay(const float delay) {
		delay_shoot = delay;
		delay_destroy_ = delay;
	}
}