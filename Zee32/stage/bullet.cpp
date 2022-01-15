#include "bullet.h"

namespace zee {
	using namespace math;

	void bullet::load_image() {
		frame_image::get().load_frame_image(coords_[player_bullet_straight_size]
			, TEXT("assets/unit_bullet.bmp"), (int)obj_type::player_bullet_straight);
		frame_image::get().load_frame_image(coords_[monster_bullet_straight_size]
			, TEXT("assets/monster_bullet_straight.bmp"), (int)obj_type::monster_bullet_straight);
		frame_image::get().load_frame_image(coords_[monster_bullet_circle_size]
			, TEXT("assets/monster_bullet_circle.bmp"), (int)obj_type::monster_bullet_circle);
		frame_image::get().load_frame_image(coords_[monster_bullet_arround_size]
			, TEXT("assets/monster_bullet_arround.bmp"), (int)obj_type::monster_bullet_arround);
		frame_image::get().load_frame_image(coords_[monster_bullet_wave_size]
			, TEXT("assets/monster_bullet_wave.bmp"), (int)obj_type::monster_bullet_wave);


		//회전
		frame_image::get().load_plg_image(coords_[monster_bullet_homing_size]
			, TEXT("assets/monster_bullet_homing.bmp"), (int)obj_type::monster_bullet_homing);

		//회전: 모든 각도의 이미지 미리 찍기
		for (float degree = 0.0f; degree != 360; degree++) {
			//회전할 때 src_pos 밖의 이미지가 자꾸 침범해서 간격을 만듦
			math::vec2f point;
			point.x =
				degree < 1.0f ?
				coords_[monster_bullet_homing_size].x / 2 :
				coords_[monster_bullet_homing_size].x / 2 + degree * (coords_[monster_bullet_homing_size].x + coords_[monster_bullet_homing_size].x / 2);

			point.y = static_cast<float>(coords_[monster_bullet_homing_size].y / 2);

			frame_image::get().render_plg(
				point,
				math::deg_to_rad(degree),
				(int)obj_type::monster_bullet_homing
			);
		}
	}

	void bullet::init() {
		projectile::init();
		set_atk(1);
		//size_, frame_size_는 스폰될 때 결정된다. 
		//몬스터의 타입을 가져오더라도, 몬스터가 무슨 탄을 쏘느냐에 따라 사이즈가 달라질 수 있기 때문.
	}

	void bullet::spawn_from(const int obj_type, const shape::circlef& obj_body) {

		//공통 
		projectile::spawn_from(obj_type, obj_body);
		set_subj_type(obj_type);


		//사이즈
		//플레이어
		if (obj_type == (int)obj_type::player_straight) {
			set_size(coords_[player_bullet_straight_size]);
			set_frame_size(coords_[player_bullet_frame]);
			set_obj_type((int)obj_type::player_bullet_straight);
		}
		//적
		else {
			//적 탄별
			switch (obj_type)
			{
			case (int)obj_type::monster_straight:
				//직선탄
				set_size(coords_[monster_bullet_straight_size]);
				set_obj_type((int)obj_type::monster_bullet_straight);
				break;
			case (int)obj_type::monster_circle:
				//원형탄
				set_size(coords_[monster_bullet_circle_size]);
				set_obj_type((int)obj_type::monster_bullet_circle);
				break;
			case (int)obj_type::monster_homing:
				//유도탄
				set_size(coords_[monster_bullet_homing_size]);
				set_obj_type((int)obj_type::monster_bullet_homing);
				break;
			case (int)obj_type::monster_arround:
				//어라운드
				set_size(coords_[monster_bullet_arround_size]);
				set_obj_type((int)obj_type::monster_bullet_arround);
				break;
			case (int)obj_type::monster_wave:
				//웨이브
				set_size(coords_[monster_bullet_wave_size]);
				set_obj_type((int)obj_type::monster_bullet_wave);
				break;
			}
			//적 공통
			set_frame_size(coords_[monster_bullet_frame]);
		}


		//생성 위치
		switch (obj_type)
		{
		case (int)obj_type::player_straight:
			set_now_pos_and_body(
				{ obj_body.origin.x - get_size().x / 2,
				obj_body.origin.y - obj_body.radius / 2 - get_size().y / 2 }
			);
			break;
		case (int)obj_type::monster_straight:
		case (int)obj_type::monster_circle:
		case (int)obj_type::monster_homing:
		case (int)obj_type::monster_arround:
		case (int)obj_type::monster_wave:
			set_now_pos_and_body(
				{ obj_body.origin.x - get_size().x / 2,
				obj_body.origin.y + obj_body.radius / 2 - get_size().y / 2 }
			);
			break;
		}

	}


	void bullet::move(const float delta_time) {
		float speed;
		switch (get_subj_type())
		{
		case (int)obj_type::player_straight: {
			speed = 500.0f;
			set_now_pos_and_body({ get_now_pos().x, get_now_pos().y - delta_time * speed});
			break;
		}
		case (int)obj_type::monster_straight: {
			speed = 200.0f;
			set_now_pos_and_body({ get_now_pos().x, get_now_pos().y + delta_time * speed });
			break;
		}
		case (int)obj_type::monster_circle: {
			speed = 150.0f;
			//이동 행렬 -> 곱
			matrix2f m;
			m.translation(get_circle_angle(), delta_time * speed, delta_time * speed);
			m.mul(get_now_pos());
			set_now_pos_and_body({ m.get_m()[0][0], m.get_m()[0][1] });
			break;
		}
		case (int)obj_type::monster_homing: {
			speed = 150.0f;
			//유도탄 회전각
			set_homing_angle(math::atan2(get_vec_for_player().x, get_vec_for_player().y));
			//회전각
			set_homing_degree(rad_to_deg(get_homing_angle()));
			if (get_homing_degree() < 0) {
				set_homing_degree(get_homing_degree() + 360);
			}
			//이동
			//stage tick 에서 얻은 vec_for_player_
			set_now_pos_and_body(get_now_pos() + get_vec_for_player() * delta_time * speed);

			//아래 행렬 이동->곱과 같음
			/*matrix m;
			m.translation(vec_for_player_.x * delta_time * speed, vec_for_player_.y * delta_time * speed);
			m.mul(now_pos_);
			set_now_pos_and_body({ m.get_mf()[0][0], m.get_mf()[0][1] });*/
			break;
		}
		case (int)obj_type::monster_arround: {
			speed = 550.0f;
			set_now_pos_and_body({ get_now_pos().x, get_now_pos().y + delta_time * speed });
			break;
		}
		case (int)obj_type::monster_wave: {
			speed = 200.0f;
			matrix2f m;
			//이동 행렬 -> 곱
			m.translation(get_circle_angle(), delta_time * speed, delta_time * speed);
			m.mul(get_now_pos());
			set_now_pos_and_body({ m.get_m()[0][0], m.get_m()[0][1] });
			break;
		}//case
		}//switch
	}

	void bullet::hit_from(const std::shared_ptr<unit> other, const float delta_time) {
		unit::hit_from(other, delta_time);
	}


	void bullet::render(win32gdi::device_context_dynamic& dest_dc) {
		if (in_screen() && get_state() == (int)obj_state::idle) {
			switch (get_obj_type())
			{
			case (int)obj_type::player_bullet_straight:
			case (int)obj_type::monster_bullet_straight:
			case (int)obj_type::monster_bullet_circle:
			case (int)obj_type::monster_bullet_arround:
			case (int)obj_type::monster_bullet_wave: {
				frame_image::get().render_transparent(
					dest_dc,
					get_now_pos(),
					{},
					get_obj_type()
				);
				break;
			}

			case (int)obj_type::monster_bullet_homing: {
				//유도탄
				//회전할 때 src_pos 밖의 이미지가 자꾸 침범해서 간격을 만듦
				math::vec2i src_pos;
				src_pos.x =
					(get_size().x + get_size().x / 2) * static_cast<int>(get_homing_degree());
				
				frame_image::get().render_plg_transparent(
					dest_dc,
					get_now_pos(),
					src_pos,
					get_obj_type()
				);

			}//case

			}//switch

		}

		//충돌범위 테스트
		unit::render(dest_dc);
	}

	int bullet::get_subj_type() const {
		return subj_type_;
	}
	float bullet::get_homing_angle() const {
		return homing_angle_;
	}
	float bullet::get_homing_degree() const {
		return homing_degree_;
	}
	float bullet::get_circle_angle() const {
		return circle_angle_;
	}
	const math::vec2f& bullet::get_vec_for_player() const {
		return vec_for_player_;
	}

	void bullet::set_subj_type(const int& obj) {
		subj_type_ = obj;
	}
	void bullet::set_homing_angle(const float angle) {
		homing_angle_ = angle;
	}
	void bullet::set_homing_degree(const float degree) {
		homing_degree_ = degree;
	}
	void bullet::set_circle_angle(const float angle) {
		circle_angle_ = angle;
	}
	void bullet::set_vec_for_player(const math::vec2f& v) {
		vec_for_player_ = v;
	}
}