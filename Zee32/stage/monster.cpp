#include "monster.h"
#include "stage.h"

namespace zee {
	monster::monster() noexcept :
		size_()
		, now_pos_()
		, body_()
		, frame_x_()
		, frame_y_()
		, shoot_type_()
		, hp_()
		, homing_angle_()
		, delay_straight_()
		, delay_circle_()
		, delay_homing_()
		, delay_arround_()
		, delay_wave_()
		, delay_destroy_() {
	}
	monster::~monster() noexcept {
	}

	void monster::init() {

		int random_shoot_type = rand(0, (int)obj_shoot_type::max - 1);


		//테스트	
		//random_shoot_type;
		//(int)obj_shoot_type::;
		shoot_type_ = random_shoot_type;


		switch (shoot_type_)
		{
		case (int)obj_shoot_type::straight:
			set_size(coords[monster_1_size]);
			break;
		case (int)obj_shoot_type::circle:
			set_size(coords[monster_2_size]);
			break;
		case (int)obj_shoot_type::homing:
			set_size(coords[monster_3_size]);
			break;
		case (int)obj_shoot_type::arround:
			set_size(coords[monster_4_size]);
			break;
		case (int)obj_shoot_type::wave:
			set_size(coords[monster_5_size]);
			break;
		}

		int rx = rand(coords[monster_min_pos].x, coords[monster_max_pos].x);
		int ry = coords[monster_min_pos].y;
		set_now_pos_and_body({ (float)rx, (float)ry });

		rx = rand(coords[monster_min_pos].x, coords[monster_max_pos].x);
		ry = coords[monster_max_pos].y;
		set_arrival_vec({ (float)rx, (float)ry });

		set_hp((int)obj_state::idle);
		
		set_delay(0.0f);
	}

	//stage에서 호출
	void monster::init_bullet(const int& shoot_type) {

		std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();
		switch (shoot_type)
		{
		case (int)obj_shoot_type::straight:
			//직선탄
			spawned_bullet->set_size(coords[monster_bullet_straight_size]);
			spawned_bullet->set_obj((int)obj_type::monster_1);
			break;
		case (int)obj_shoot_type::circle:
			//원형탄
			spawned_bullet->set_size(coords[monster_bullet_circle_size]);
			spawned_bullet->set_obj((int)obj_type::monster_2);
			break;
		case (int)obj_shoot_type::homing:
			//유도탄
			spawned_bullet->set_size(coords[monster_bullet_homing_size]);
			spawned_bullet->set_obj((int)obj_type::monster_3);
			break;
		case (int)obj_shoot_type::arround:
			//어라운드
			spawned_bullet->set_size(coords[monster_bullet_arround_size]);
			spawned_bullet->set_obj((int)obj_type::monster_4);
			break;
		case (int)obj_shoot_type::wave:
			//웨이브
			spawned_bullet->set_size(coords[monster_bullet_wave_size]);
			spawned_bullet->set_obj((int)obj_type::monster_5);
			break;
		}
		spawned_bullet->set_now_pos_and_body(
			{ now_pos_.x + size_.x / 2 - spawned_bullet->get_size().x / 2
			, now_pos_.y + size_.y / 2 }
		);
		spawned_bullet->set_max_move_size(coords[back_max_size]);
		spawned_bullet->set_frame_size(coords[monster_bullet_frame]);
		spawned_bullet->set_move_type(shoot_type);
		bullets_.push_back(spawned_bullet);
	}

	void monster::tick(float delta_time) {
		move(delta_time);
		shoot(delta_time);
		destroy(delta_time);
	}

	const bool monster::in_screen() const {
		return now_pos_.x > coords[back_min_size].x && now_pos_.x < coords[back_max_size].x
			&& now_pos_.y > coords[back_min_size].y && now_pos_.y < coords[back_max_size].y;
	}

	void monster::move(const float delta_time) {
		float speed;
		float dist;
		switch (shoot_type_) 
		{
		case (int)obj_shoot_type::straight:
		case (int)obj_shoot_type::circle:
		case (int)obj_shoot_type::homing:
			speed = 100.0f;
			//이동 방향 벡터는 첨에 스폰 시 생성
			//단위화 위해 거리 구하기
			dist = sqrtf(arrival_vec_.x * arrival_vec_.x + arrival_vec_.y * arrival_vec_.y);
			//단위화
			arrival_vec_ /= dist;
			set_now_pos_and_body(now_pos_ + arrival_vec_ * delta_time * speed);
			break;
		case (int)obj_shoot_type::arround: {
			//플레이어 적당히 따라오다가 y축 저점에서 도착지로 노선 변경
			if (now_pos_.y < coords[back_max_size].y * 2 / 3) {
				//플레이어 호밍
				speed = 100.0f;
				dist = sqrtf(vec_for_player_.x * vec_for_player_.x + vec_for_player_.y * vec_for_player_.y);
				vec_for_player_ /= dist;
				set_now_pos_and_body(now_pos_ + vec_for_player_ * delta_time * speed);

				//회전각
				if (shoot_type_ == (int)obj_shoot_type::arround) {
					homing_angle_ = math::atan2(vec_for_player_.x, vec_for_player_.y);
				}
			}
			else {
				speed = 100.0f;
				dist = sqrtf(arrival_vec_.x * arrival_vec_.x + arrival_vec_.y * arrival_vec_.y);
				arrival_vec_ /= dist;
				set_now_pos_and_body(now_pos_ + arrival_vec_ * delta_time * speed);

				//회전각
				if (shoot_type_ == (int)obj_shoot_type::arround) {
					homing_angle_ = math::atan2(arrival_vec_.x, arrival_vec_.y);
				}
			}
			break;
		}
		case (int)obj_shoot_type::wave: {
			if (now_pos_ != coords[monster_boss_pos]) {
				speed = 100.0f;
				static math::vec2f v = { 0.0f, 1.0f };
				set_now_pos_and_body({ now_pos_.x, now_pos_.y + v.y * delta_time * speed });
			}
			break;
		}//case
		}//switch
	}

	void monster::shoot(const float delta_time) {
		switch (shoot_type_)
		{
		case (int)obj_shoot_type::straight: {
			//직선탄
			const float frame_straight = 1.5f;
			delay_straight_ += delta_time;
			if (delay_straight_ >= frame_straight) {
				init_bullet((int)obj_shoot_type::straight);
			}
			delay_straight_ = (float)math::fmod(delay_straight_, frame_straight);
			break;
		}
		case (int)obj_shoot_type::circle: {
			//원형탄
			const float frame_circle = 3.0f;
			delay_circle_ += delta_time;
			if (delay_circle_ >= frame_circle) {
				float circle_angle = 0;
				int bullent_circle_cnt = 10;

				for (int i = 0; i != bullent_circle_cnt; i++) {
					init_bullet((int)obj_shoot_type::circle);
					circle_angle += math::pi() * 2 / (float)bullent_circle_cnt;
					bullets_.back()->set_circle_angle(circle_angle);
				}
			}
			delay_circle_ = (float)math::fmod(delay_circle_, frame_circle);
			break;
		}
		case (int)obj_shoot_type::homing: {
			//유도탄
			const float frame_homing = 2.0f;
			delay_homing_ += delta_time;
			if (delay_homing_ >= frame_homing) {
				init_bullet((int)obj_shoot_type::homing);
			}
			delay_homing_ = (float)math::fmod(delay_homing_, frame_homing);
			break;
		}
		case (int)obj_shoot_type::arround: {
			//어라운드
			const float frame_arround = 1.5f;
			delay_arround_ += delta_time;
			if (delay_arround_ >= frame_arround) {
				init_bullet((int)obj_shoot_type::arround);
			}
			delay_arround_ = (float)math::fmod(delay_arround_, frame_arround);
			break;
		}
		case (int)obj_shoot_type::wave: {
			//웨이브
			const float frame_wave = 3.0f;
			delay_wave_ += delta_time;
			if (delay_wave_ >= frame_wave) {
				float circle_angle = 0;
				int bullent_circle_cnt = 20;

				for (int i = 0; i != bullent_circle_cnt; i++) {
					init_bullet((int)obj_shoot_type::wave);
					circle_angle += math::pi() * 2 / (float)bullent_circle_cnt;
					bullets_.back()->set_circle_angle(circle_angle);
				}
			}
			delay_wave_ = (float)math::fmod(delay_wave_, frame_wave);
			break;
		}//case
		}//switch


		//뷸렛 틱
		for (auto& bullet_obj : bullets_) {
			bullet_obj->tick(delta_time);
		}
	}

	void monster::destroy(const float delta_time) {
		if (!(in_screen())) {
			hp_ = (int)obj_state::die;
		}
		if (hp_ == (int)obj_state::die) {

			set_now_pos_and_body(coords[back_destroy_zone]);

			//일정시간 후 스폰
			const float speed = 2.0f;
			const float frame = 3.0f;
			delay_destroy_ += delta_time * speed;

			if (delay_destroy_ >= frame) {
				init();
			}

		}

		//뷸렛 제거
		for (int i = 0; i != bullets_.size(); ) {
			if (!(bullets_[i]->in_screen())) {
				bullets_.erase(bullets_.begin() + i);
			}
			else {
				i++;
			}
		}
	}

	void monster::render(win32gdi::device_context_dynamic& dest_dc) {
		if (in_screen()) {
			frame_image::get().render_destdc_to_backbuffer(dest_dc);

			//몸체
			if (hp_ == (int)obj_state::idle) {
				switch (shoot_type_)
				{
				case (int)obj_shoot_type::straight:
					frame_image::get().render_transparent(
						dest_dc
						, now_pos_
						, {}
						, (int)obj_type::monster_1
					);
					break;
				case (int)obj_shoot_type::circle:
					frame_image::get().render_transparent(
						dest_dc
						, now_pos_
						, {}
						, (int)obj_type::monster_2
					);
					break;
				case (int)obj_shoot_type::homing:
					frame_image::get().render_transparent(
						dest_dc
						, now_pos_
						, {}
						, (int)obj_type::monster_3
					);
					break;
				case (int)obj_shoot_type::arround:
					frame_image::get().render_plg(
						dest_dc
						, body_.origin
						, homing_angle_
						, (int)obj_type::monster_4
					);
					frame_image::get().render_transparent_backbuffer_to_destdc(dest_dc, {});
					break;
				case (int)obj_shoot_type::wave:
					frame_image::get().render_transparent(
						dest_dc
						, now_pos_
						, {}
						, (int)obj_type::monster_5
					);
					break;
				}
			}
		}

		//몬스터가 스크린 밖에 있을 때도 계속 노출
		//뷸렛 렌더
		for (auto& bullet_obj : bullets_) {
			bullet_obj->render(dest_dc);
		}

		//충돌범위 테스트
		shape::circlef circle{ body_.origin, body_.radius };
		if (key_state::is_toggle_on(keys::tab)) {
			dest_dc.circle(circle);
		}
	}

	const math::vec2f& monster::get_now_pos() const {
		return now_pos_;
	}
	const shape::circlef& monster::get_body() const {
		return body_;
	}
	const math::vec2i& monster::get_frame_x() const {
		return frame_x_;
	}
	const math::vec2i& monster::get_frame_y() const {
		return frame_y_;
	}
	const int monster::get_shoot_type() const {
		return shoot_type_;
	}
	const math::vec2f& monster::get_arrival_vec() const {
		return arrival_vec_;
	}
	const int monster::get_hp() const {
		return hp_;
	}
	const std::vector<std::shared_ptr<bullet>> monster::get_bullets() const {
		return bullets_;
	}
	const math::vec2f& monster::get_vec_for_player() const {
		return vec_for_player_;
	}
	void monster::set_now_pos_and_body(const math::vec2f& point) {
		now_pos_ = point;
		set_body(now_pos_ + size_ / 2, (float)math::min(size_.x, size_.y) / 2);
	}
	void monster::set_body(const math::vec2f& origin, const float& r) {
		body_.origin = origin;
		body_.radius = r;
	}
	void monster::set_size(const math::vec2i& size) {
		size_ = size;
	}
	void monster::set_shoot_type(const int i) {
		shoot_type_ = i;
	}
	void monster::set_arrival_vec(const math::vec2f& vec) {
		arrival_vec_ = vec;
	}
	void monster::set_hp(const int hp) {
		hp_ = hp;
	}
	void monster::set_vec_for_player(const math::vec2f& v) {
		vec_for_player_ = v;
	}
	void monster::set_delay(const float delay) {
		delay_straight_ = delay;
		delay_circle_ = delay;
		delay_homing_ = delay;
		delay_arround_ = delay;
		delay_wave_ = delay;
		delay_destroy_ = delay;
	}
}