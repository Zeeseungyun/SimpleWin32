#include "monster.h"
#include "player.h"

namespace yjj {
	using namespace math;

	void monster::load_image() {
		frame_image::get().load_frame_image(coords_monster_[monster_straight_size]
			, TEXT("assets/monster_straight.bmp"), (int)obj_type::monster_straight);
		frame_image::get().load_frame_image(coords_monster_[monster_circle_size]
			, TEXT("assets/monster_circle.bmp"), (int)obj_type::monster_circle);
		frame_image::get().load_frame_image(coords_monster_[monster_homing_size]
			, TEXT("assets/monster_homing.bmp"), (int)obj_type::monster_homing);
		frame_image::get().load_frame_image(coords_monster_[monster_wave_size]
			, TEXT("assets/monster_wave.bmp"), (int)obj_type::monster_wave);



		//ȸ��
		frame_image::get().load_plg_image(coords_monster_[monster_arround_size]
			, TEXT("assets/monster_arround.bmp"), (int)obj_type::monster_arround);


		//ȸ��: ��� ������ �̹��� �̸� ���
		for (float degree = 0.0f; degree != 360; degree++) {
			//ȸ���� �� src_pos ���� �̹����� �ڲ� ħ���ؼ� ������ ����
			math::vec2f point;
			point.x =
				degree < 1.0f ?
				coords_monster_[monster_arround_size].x / 2 :
				coords_monster_[monster_arround_size].x / 2 + degree * (coords_monster_[monster_arround_size].x + coords_monster_[monster_arround_size].x / 2);

			point.y = static_cast<float>(coords_monster_[monster_arround_size].y / 2);

			frame_image::get().render_plg(
				point,
				math::deg_to_rad(degree),
				(int)obj_type::monster_arround
			);
		}
	}

	void monster::init() {
		//���ʹ� ���� �������� init�Լ��� spawn�Լ��� ���� �̻� ����.
		monster::spawn();
		set_my_score(1);

		//���� �淿
		for (int i = 0; i != monster_bullet_max_num; i++) {
			std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();
			spawned_bullet->init();
			bullets_.push_back(spawned_bullet);
		}
	}

	void monster::spawn() {
		plane::init();
		set_now_pos_and_body(coords_monster_[destroy_monster_zone]);
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

		//�׽�Ʈ	--------------------------------------------------------
		//set_obj_type((int)obj_type::monster_circle);


		switch (get_obj_type())
		{
		case (int)obj_type::monster_straight:
			set_size(coords_monster_[monster_straight_size]);
			break;
		case (int)obj_type::monster_circle:
			set_size(coords_monster_[monster_circle_size]);
			break;
		case (int)obj_type::monster_homing:
			set_size(coords_monster_[monster_homing_size]);
			break;
		case (int)obj_type::monster_arround:
			set_size(coords_monster_[monster_arround_size]);
			break;
		case (int)obj_type::monster_wave:
			set_size(coords_monster_[monster_wave_size]);
			set_hp(3);
			set_wave_angle(0.0f);
			set_wave_angle_numer(0.0f);
			break;
		}

		int rx = rand(coords_monster_[monster_min_pos].x, coords_monster_[monster_max_pos].x);
		int ry = coords_monster_[monster_min_pos].y;
		set_now_pos_and_body({ static_cast<float>(rx), static_cast<float>(ry) });

		rx = rand(coords_monster_[monster_min_pos].x, coords_monster_[monster_max_pos].x);
		ry = coords_monster_[monster_max_pos].y;
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
				//�̵� ���� ���ʹ� ÷�� ���� �� ����
				math::vec2f v{
					get_body().origin - get_arrival_vec()
				};
				//����->�Ÿ�->����ȭ
				set_arrival_vec(arrival_vec_.normalize());
				set_now_pos_and_body(get_now_pos() + get_arrival_vec() * delta_time * get_speed());
				break;
			}

			case (int)obj_type::monster_arround: {
				//�÷��̾� ������ٰ� y�� �������� �׳� �Ʒ��� ������� ����.
				if (get_now_pos().y < coords[back_max_size].y * 2 / 3) {
					//ȸ����
					if (get_obj_type() == (int)obj_type::monster_arround) {
						set_homing_angle(math::atan2(get_vec_for_player().x, get_vec_for_player().y));
						//rad to degree
						set_homing_degree(rad_to_deg(get_homing_angle()));
						if (get_homing_degree() < 0) {
							set_homing_degree(get_homing_degree() + 360);
						}
					}
					//�̵�
					set_speed(200.0f);
					set_vec_for_player(get_vec_for_player().normalize());
					set_now_pos_and_body(get_now_pos() + get_vec_for_player() * delta_time * get_speed());
					//�Ʒ� ��� �̵�->���� ����
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
				if (get_now_pos() != coords_monster_[monster_boss_pos]) {
					set_speed(150.0f);
					set_now_pos_and_body({ get_now_pos().x, get_now_pos().y + delta_time * get_speed() });
				}
				break;
			}//case
			}//switch
		}
	}

	//�淿 ���
	void monster::shoot(const float delta_time) {
		if (in_screen()
			&& get_state() == (int)obj_state::idle) {
			switch (get_obj_type()) {

			case (int)obj_type::monster_straight: {
				//����ź
				constexpr float frame_straight = 1.5f;
				set_delay_shoot(get_delay_shoot() + delta_time);
				plane::shoot_per_delay(frame_straight);
				break;
			}

			case (int)obj_type::monster_circle: {
				//����ź
				constexpr float frame_circle = 3.0f;
				set_delay_shoot(get_delay_shoot() + delta_time);
				if (get_delay_shoot() >= frame_circle) {
					float circle_angle = 0;
					constexpr int bullet_circle_cnt = 15;
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
				//����ź
				constexpr float frame_homing = 2.0f;
				set_delay_shoot(get_delay_shoot() + delta_time);
				plane::shoot_per_delay(frame_homing);
				break;
			}

			case (int)obj_type::monster_arround: {
				//�����
				constexpr float frame_arround = 1.5f;
				set_delay_shoot(get_delay_shoot() + delta_time);
				plane::shoot_per_delay(frame_arround);
				break;
			}

			case (int)obj_type::monster_wave: {
				//���̺�
				constexpr float frame_wave = 0.4f;
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
		//other�� player�� ����Ű�� nullptr�� �ƴϸ�
		if (std::shared_ptr<player> other_player = std::dynamic_pointer_cast<player>(other)) {
			//��Ʈ
			unit::hit_from(other, delta_time);
			other_player->add_score(get_my_score());
		}
		else {
			assert(false);
		}
	}

	void monster::destroy(const float delta_time) {
		//����
		unit::destroy(delta_time);
		set_now_pos_and_body(coords_monster_[destroy_monster_zone]);
		spawn();
	}
	

	void monster::render(win32gdi::device_context_dynamic& dest_dc) {
		if (in_screen() 
			&& get_state() == (int)obj_state::idle) {

			//��ü
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
					//ȸ���� �� src_pos ���� �̹����� �ڲ� ħ���ؼ� ������ ����
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

		//�浹���� ����
		unit::render(dest_dc);

		//���� �淿
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