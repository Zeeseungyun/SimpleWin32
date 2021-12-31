#include "monster.h"
#include "stage.h"

namespace zee {
	monster::monster() noexcept :
		size_()
		, now_pos_()
		, body_()
		, frame_x_()
		, frame_y_()
		, angle_()
		, shoot_type_()
		, state_()	{
	}
	monster::~monster() noexcept {
	}

	void monster::init() {
		if (state_ == (int)obj_state::idle) {

			set_size(coords[monster_1_size]);

			int random_shoot_type = rand(0, (int)obj_shoot_type::max - 1);
			shoot_type_ = random_shoot_type;

			int rx = rand(coords[monster_min_pos].x, coords[monster_max_pos].x);
			int ry = coords[monster_min_pos].y;
			set_now_pos_and_body({ (float)rx, (float)ry });

			rx = rand(coords[monster_min_pos].x, coords[monster_max_pos].x);
			ry = coords[monster_max_pos].y;
			set_arrival_point({ (float)rx, (float)ry });
		}
	}

	//app ���� �� stage���� ȣ��
	void monster::init_bullet(const int& shoot_type) {

		std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();
		spawned_bullet->set_obj((int)obj_type::monster_1);
		spawned_bullet->set_max_move_size(coords[back_loop_max_size]);
		switch (shoot_type)
		{
		case (int)obj_shoot_type::follow:
			spawned_bullet->set_size(coords[monster_bullet_follow_size]);
			break;
		default:
			spawned_bullet->set_size(coords[monster_bullet_size]);
			break;
		}
		spawned_bullet->set_now_pos_and_body(coords[back_destroy_zone]);
		spawned_bullet->set_frame_size(coords[monster_bullet_frame]);
		spawned_bullet->set_obj((int)obj_type::monster_1);
		spawned_bullet->set_move_type(shoot_type);
		spawned_bullet->set_spawn_state(false);
		bullets_.push_back(spawned_bullet);
	}

	void monster::tick(float delta_time) {
		move(delta_time);
		rotate(delta_time);
		shoot(delta_time);
		hit_to_idle(delta_time);
		destroy(delta_time);
	}

	const bool monster::in_screen() const {
		return now_pos_.x > coords[back_min_size].x && now_pos_.x < coords[back_loop_max_size].x
			&& now_pos_.y > coords[back_min_size].y && now_pos_.y < coords[back_loop_max_size].y;
	}

	void monster::move(const float& delta_time) {
		const float speed = 2.0f;
		//�̵� ���� ���ʹ� ÷�� ���� �� ����
		//����ȭ ���� �Ÿ� ���ϱ�
		float dist = sqrtf(arrival_point_.x * arrival_point_.x + arrival_point_.y * arrival_point_.y);
		//����ȭ
		arrival_point_ /= dist;
		set_now_pos_and_body(now_pos_ + arrival_point_ * speed);
	}

	void monster::rotate(const float& delta_time) {
		angle_ += 0.1f;
		if (angle_ >= math::pi() * 2) {
			angle_ = 0.0f;
		}
	}

	void monster::shoot(const float& delta_time) {
		static float delay = 0.0f;
		const float frame = 3.0f;
		delay += delta_time;
		if (delay >= frame) {
			switch (shoot_type_)
			{
			case (int)obj_shoot_type::straight:
				for (auto& bullet_obj : bullets_) {
					if (!bullet_obj->get_spawn_state()) {
						bullet_obj->set_spawn_state(true);
						bullet_obj->set_state((int)obj_state::idle);
						bullet_obj->set_now_pos_and_body(
							{ now_pos_.x + size_.x / 2 - coords[monster_bullet_size].x / 2
							, now_pos_.y + size_.y / 2 }
						);
						break;
					}
				}
			case (int)obj_shoot_type::follow:
				for (auto& bullet_obj : bullets_) {
					if (!bullet_obj->get_spawn_state()) {
						bullet_obj->set_spawn_state(true);
						bullet_obj->set_now_pos_and_body(
							{ now_pos_.x + size_.x / 2 - coords[monster_bullet_follow_size].x / 2
							, now_pos_.y + size_.y / 2 }
						);
						break;
					}
				}
				break;
			case (int)obj_shoot_type::circle:
				float circle_angle = 0;
				const int bullet_cnt = 10;

				for (int i = 0; i < bullet_cnt; ) {
					for (auto& bullet_obj : bullets_) {
						if (!bullet_obj->get_spawn_state()) {
							bullet_obj->set_spawn_state(true);
							bullet_obj->set_now_pos_and_body(
								{ now_pos_.x + size_.x / 2 - coords[monster_bullet_size].x/ 2
								, now_pos_.y + size_.y / 2 }
							);
							circle_angle += math::pi() * 2 / (float)bullet_cnt;
							bullet_obj->set_circle_angle(circle_angle);
							i++;
							break;
						}
					}

				}
				break;
			}
		}

		delay = (float)math::fmod(delay, frame);

		for (auto& bullet_obj : bullets_) {
			bullet_obj->tick(delta_time);
		}
	}

	//bomb �̹��� ��� ���� �ð�
	void monster::hit_to_idle(const float& delta_time) {
		if (state_ == (int)obj_state::hit) {
			static float delay = 0.0f;
			const float frame = 0.8f;
			delay += delta_time;
			if (delay >= frame) {
				state_ = (int)obj_state::idle;
			}

			delay = (float)math::fmod(delay, frame);
		}
	}

	void monster::destroy(const float& delta_time) {
		if (state_ == (int)obj_state::hit || !(in_screen())) {

			//���� ����Ʈ
			static float delay = 0.0f;
			const float speed = 7.0f;
			const float frame = 1.0f;
			const float frame_final = 3.0f;
			delay += delta_time * speed;

			if (delay >= frame) {
				frame_x_ = { coords[effect_bomb_size].x * (int)delay, 0 };
			}
			if (delay >= frame_final) {
				state_ = (int)obj_state::idle;
			}
			delay = (float)math::fmod(delay, frame_final);
			frame_x_.x %= coords[effect_bomb_final_frame].x;

			//���� ��ġ ���
			bomb_point_ = now_pos_;
			set_now_pos_and_body(coords[back_destroy_zone]);

			//�ڱ� �Ѿ� ���ֱ�
			for (auto& bullet_obj : bullets_) {
				bullet_obj->set_now_pos_and_body(coords[back_destroy_zone]);
			}

			init();
		}
	}

	void monster::render(win32gdi::device_context_dynamic& dest_dc) {
		if (in_screen()) {
			frame_image::get().render_destdc_to_backbuffer(dest_dc);
			//ȸ�� �׽�Ʈ
			/*frame_image::get().render_plg(dest_dc, body_.origin, angle_, (int)obj_type::monster);
			frame_image::get().render_transparent_backbuffer_to_destdc(dest_dc, {});*/

			//��ü
			if (state_ == (int)obj_state::idle) {
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
				case (int)obj_shoot_type::follow:
					frame_image::get().render_transparent(
						dest_dc
						, now_pos_
						, {}
						, (int)obj_type::monster_3
					);
					break;
				}
			}
			else if (state_ == (int)obj_state::hit) {
 				frame_image::get().render_transparent(
					dest_dc
					, bomb_point_
					, frame_x_ + frame_y_
					, (int)obj_type::bomb
				);
			}

			//�Ѿ�
			for (auto& bullet_obj : bullets_) {
				bullet_obj->render(dest_dc);
			}

			//�浹���� �׽�Ʈ
			shape::circlef circle{ body_.origin, body_.radius };
			if (key_state::is_toggle_on(keys::tab)) {
				dest_dc.circle(circle);
			}
		}
	}

	const math::vec2f monster::get_now_pos() const {
		return now_pos_;
	}
	const shape::circlef monster::get_body() const {
		return body_;
	}
	const math::vec2i& monster::get_frame_x() const {
		return frame_x_;
	}
	const math::vec2i& monster::get_frame_y() const {
		return frame_y_;
	}
	const float& monster::get_angle() const {
		return angle_;
	}
	const int& monster::get_shoot_type() const {
		return shoot_type_;
	}
	const int& monster::get_state() const {
		return state_;
	}
	const math::vec2f& monster::get_bomb_point() const {
		return bomb_point_;
	}
	const math::vec2f& monster::get_arrival_point() const {
		return arrival_point_;
	}
	const std::vector<std::shared_ptr<bullet>> monster::get_bullets() const {
		return bullets_;
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
	void monster::set_shoot_type(const int& i) {
		shoot_type_ = i;
	}
	void monster::set_state(const int& i) {
		state_ = i;
	}
	void monster::set_bomb_point(const math::vec2f& point) {
		bomb_point_ = point;
	}
	void monster::set_arrival_point(const math::vec2f& point) {
		arrival_point_ = point;
	}
}