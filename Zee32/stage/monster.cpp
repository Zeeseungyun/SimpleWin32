#include "monster.h"
#include "player.h"
#include "item.h"

namespace zee {
	using namespace math;

	void monster::load_image() {
		frame_image::get().load_frame_image(coords_[monster_straight_size]
			, TEXT("assets/monster_straight.bmp"), (int)obj_type::monster_straight);
		frame_image::get().load_frame_image(coords_[monster_circle_size]
			, TEXT("assets/monster_circle.bmp"), (int)obj_type::monster_circle);
		frame_image::get().load_frame_image(coords_[monster_homing_size]
			, TEXT("assets/monster_homing.bmp"), (int)obj_type::monster_homing);
		frame_image::get().load_frame_image(coords_[monster_arround_size]
			, TEXT("assets/monster_arround.bmp"), (int)obj_type::monster_arround);
		frame_image::get().load_frame_image(coords_[monster_wave_size]
			, TEXT("assets/monster_wave.bmp"), (int)obj_type::monster_wave);
	}

	void monster::init(const int obj_state) {
		//���ʹ� ���� �������� init�Լ��� spawn�Լ��� ���� �̻� ����.
		monster::spawn();
		set_my_score(1);

		//���� �淿
		for (int i = 0; i != monster_bullet_max_num; i++) {
			std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();
			spawned_bullet->init((int)obj_state::idle);
			bullets_.push_back(spawned_bullet);
		}
	}

	void monster::spawn() {
		plane::init((int)obj_state::idle);

		int random_shoot_type = rand(1, 5);	//monster_num
		switch (random_shoot_type)
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

		//�׽�Ʈ	--------------------------------------------------------
		//obj_type_ = (int)obj_shoot_type::;

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
			break;
		}

		int rx = rand(coords_[monster_min_pos].x, coords_[monster_max_pos].x);
		int ry = coords_[monster_min_pos].y;
		set_now_pos_and_body({ (float)rx, (float)ry });

		rx = rand(coords_[monster_min_pos].x, coords_[monster_max_pos].x);
		ry = coords_[monster_max_pos].y;
		set_arrival_vec({ (float)rx, (float)ry });

		set_delay(0.0f);
	}

	void monster::move(const float delta_time) {
		if (in_screen()) {
			float speed;
			switch (get_obj_type())
			{
			case (int)obj_type::monster_straight:
			case (int)obj_type::monster_circle:
			case (int)obj_type::monster_homing: {
				speed = 150.0f;
				//�̵� ���� ���ʹ� ÷�� ���� �� ����
				math::vec2f v{
					get_body().origin - get_arrival_vec()
				};
				//����->�Ÿ�->����ȭ
				set_arrival_vec(arrival_vec_.normalize());
				set_now_pos_and_body(get_now_pos() + get_arrival_vec() * delta_time * speed);
				break;
			}
			case (int)obj_type::monster_arround: {
				//�÷��̾� ������ٰ� y�� �������� �׳� �Ʒ��� ������� ����.
				if (get_now_pos().y < coords[back_max_size].y * 2 / 3) {
					//ȸ����
					if (get_obj_type() == (int)obj_type::monster_arround) {
						set_homing_angle(math::atan2(get_vec_for_player().x, get_vec_for_player().y));
					}
					//�̵�
					speed = 200.0f;
					set_vec_for_player(get_vec_for_player().normalize());
					set_now_pos_and_body(get_now_pos() + get_vec_for_player() * delta_time * speed);
					//�Ʒ� ��� �̵�->���� ����
					/*matrix m;
					m.translation(vec_for_player_.x * delta_time * speed, vec_for_player_.y * delta_time * speed);
					m.mul(now_pos_);
					set_now_pos_and_body({ m.get_mf()[0][0], m.get_mf()[0][1] });*/
				}
				else {
					speed = 250.0f;
					set_now_pos_and_body({ get_now_pos().x, get_now_pos().y + delta_time * speed });
					set_homing_angle(0.0f);
				}
				break;
			}
			case (int)obj_type::monster_wave: {
				if (get_now_pos() != coords_[monster_boss_pos]) {
					speed = 150.0f;
					set_now_pos_and_body({ get_now_pos().x, get_now_pos().y + delta_time * speed });
				}
				break;
			}//case
			}//switch
		}
	}

	//�淿 ���
	void monster::shoot(const float delta_time) {
		if (in_screen()) {
			switch (get_obj_type())
			{
			case (int)obj_type::monster_straight: {
				//����ź
				const float frame_straight = 1.5f;
				set_delay_shoot(get_delay_shoot() + delta_time);
				if (get_delay_shoot() >= frame_straight) {
					for (auto& bullet_obj : get_bullets()) {
						if (bullet_obj->get_state() == (int)obj_state::die) {
							bullet_obj->spawn_from(get_obj_type(), get_body());
							break;
						}
					}
				}
				set_delay_shoot((float)math::fmod(get_delay_shoot(), frame_straight));
				break;
			}
			case (int)obj_type::monster_circle: {
				//����ź
				const float frame_circle = 3.0f;
				set_delay_shoot(get_delay_shoot() + delta_time);
				if (get_delay_shoot() >= frame_circle) {
					float circle_angle = 0;
					const int bullent_circle_cnt = 10;
					for (int i = 0; i != bullent_circle_cnt; i++) {

						circle_angle += math::pi() * 2 / (float)bullent_circle_cnt;

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
				//����ź
				const float frame_homing = 2.0f;
				set_delay_shoot(get_delay_shoot() + delta_time);
				if (get_delay_shoot() >= frame_homing) {
					for (auto& bullet_obj : get_bullets()) {
						if (bullet_obj->get_state() == (int)obj_state::die) {
							bullet_obj->spawn_from(get_obj_type(), get_body());
							break;
						}
					}
				}
				set_delay_shoot((float)math::fmod(get_delay_shoot(), frame_homing));
				break;
			}
			case (int)obj_type::monster_arround: {
				//�����
				const float frame_arround = 1.5f;
				set_delay_shoot(get_delay_shoot() + delta_time);
				if (get_delay_shoot() >= frame_arround) {
					for (auto& bullet_obj : get_bullets()) {
						if (bullet_obj->get_state() == (int)obj_state::die) {
							bullet_obj->spawn_from(get_obj_type(), get_body());
							break;
						}
					}
				}
				set_delay_shoot((float)math::fmod(get_delay_shoot(), frame_arround));
				break;
			}
			case (int)obj_type::monster_wave: {
				//���̺�
				const float frame_wave = 3.0f;
				set_delay_shoot(get_delay_shoot() + delta_time);
				if (get_delay_shoot() >= frame_wave) {
					float circle_angle = 0;
					int bullent_circle_cnt = 30;

					//�꿩
					for (int i = 0; i != bullent_circle_cnt; i++) {

						circle_angle += math::pi() * 2 / (float)bullent_circle_cnt;

						for (auto& bullet_obj : get_bullets()) {
							if (bullet_obj->get_state() == (int)obj_state::idle) {
								bullet_obj->spawn_from(get_obj_type(), get_body());
								bullet_obj->set_circle_angle(circle_angle);
								break;
							}
						}
					}
				}
				set_delay_shoot((float)math::fmod(get_delay_shoot(), frame_wave));
				break;
			}//case
			}//switch

			//�淿 ƽ
			for (auto& bullet_obj : get_bullets()) {
				bullet_obj->move(delta_time);
				bullet_obj->destroy(delta_time);
			}
		}
	}

	void monster::hit_from(const std::shared_ptr<unit> other, const float delta_time) {
		//other�� player�� ����Ű�� nullptr�� �ƴϸ�
		if (std::shared_ptr<player> other_player = std::dynamic_pointer_cast<player>(other)) {
			//��Ʈ
			unit::hit_from(other, delta_time);
			other_player->add_score(get_my_score());
		}
	}

	void monster::destroy(const float delta_time) {
		//����
		plane::destroy(delta_time);
		spawn();
	}
	

	void monster::render(win32gdi::device_context_dynamic& dest_dc) {
		if (in_screen()) {

			//��ü
			if (get_state() != (int)obj_state::die) {
				switch (get_obj_type())
				{
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
					frame_image::get().render_plg(
						dest_dc,
						get_body().origin,
						get_homing_angle(),
						get_obj_type()
					);
					break;
				}//case

				}//switch
			}//if
		}//if


		//�÷��̾� �淿
		for (auto& bullet_obj : get_bullets()) {
			bullet_obj->render(dest_dc);
		}

		//�÷��� ����
		plane::render(dest_dc);
	}

	const math::vec2f& monster::get_arrival_vec() const {
		return arrival_vec_;
	}
	const math::vec2f& monster::get_vec_for_player() const {
		return vec_for_player_;
	}
	const float monster::get_homing_angle() const {
		return homing_angle_;
	}
	const float monster::get_delay_shoot() const {
		return delay_shoot_;
	}
	const float monster::get_delay_destroy() const {
		return delay_destroy_;
	}
	const std::vector<std::shared_ptr<bullet>> monster::get_bullets() const {
		return bullets_;
	}

	void monster::set_arrival_vec(const math::vec2f& vec) {
		arrival_vec_ = vec;
	}
	void monster::set_vec_for_player(const math::vec2f& v) {
		vec_for_player_ = v;
	}
	void monster::set_delay(const float delay) {
		delay_shoot_ = delay;
		delay_destroy_ = delay;
	}
	void monster::set_homing_angle(const float angle) {
		homing_angle_ = angle;
	}
	void monster::set_delay_shoot(const float delay) {
		delay_shoot_ = delay;
	}
	void monster::set_delay_destroy(const float delay) {
		delay_destroy_ = delay;
	}
}