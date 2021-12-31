#include "stage.h"

namespace zee {
	using namespace math;

	stage::stage() noexcept {
	}
	stage::~stage() noexcept {
	}

	void stage::on_app_started() {
		application_delegates::on_client_size_changed().add_sp(shared_from_this(), &stage::on_resize);
		tick_manager::get().add(shared_from_this());
		on_resize(application::get().config().client_size);
		application::get().window_handle<HWND>();	//������ �ڵ� ��������

		//�̹��� �ε�
		//���
		background_image::get().load_background_image(coords[back_loop_max_size]
			, TEXT("assets/game_background_loop_vertical.bmp"));
		background_image::get().load_background_image(coords[back_scroll_max_size]
			, TEXT("assets/game_background_stop.bmp"));
		//����
		frame_image::get().load_frame_image(coords[back_loop_max_size], coords[unit_size]
			, TEXT("assets/player.bmp"), (int)obj_type::unit);
		frame_image::get().load_frame_image(coords[back_loop_max_size], coords[monster_1_size]
			, TEXT("assets/monster_1.bmp"), (int)obj_type::monster_1);
		frame_image::get().load_frame_image(coords[back_loop_max_size], coords[monster_2_size]
			, TEXT("assets/monster_2.bmp"), (int)obj_type::monster_2);
		frame_image::get().load_frame_image(coords[back_loop_max_size], coords[monster_2_size]
			, TEXT("assets/monster_3.bmp"), (int)obj_type::monster_3);
		//�淿
		frame_image::get().load_frame_image(coords[back_loop_max_size], coords[unit_bullet_straight_size]
			, TEXT("assets/unit_bullet.bmp"), (int)obj_type::unit_bullet_straight);
		frame_image::get().load_frame_image(coords[back_loop_max_size], coords[monster_bullet_straight_size]
			, TEXT("assets/monster_bullet.bmp"), (int)obj_type::monster_bullet_straight);
		frame_image::get().load_frame_image(coords[back_loop_max_size], coords[monster_bullet_circle_size]
			, TEXT("assets/monster_bullet_circle.bmp"), (int)obj_type::monster_bullet_circle);
		frame_image::get().load_frame_image(coords[back_loop_max_size], coords[monster_bullet_follow_size]
			, TEXT("assets/monster_bullet_follow.bmp"), (int)obj_type::monster_bullet_follow);
		//����
		frame_image::get().load_frame_image(coords[back_loop_max_size], coords[effect_bomb_size]
			, TEXT("assets/bomb.bmp"), (int)obj_type::bomb);

		//���� �ʱ�ȭ
		init_game();

		//��� �׽�Ʈ
		/*matrix m;
		//��Ľ�
		m.set_m2f({ {2,3}, {4,6} });
		m.determinant();
		//�׵����
		m.set_m3f({ {1, 0, 0}, {0,1,0}, {0,0,1} });
		if (m.is_identity()) {
			ZEE_LOG(normal, TEXT("�׵���� ��"), TEXT("-"));
		}
		else {
			ZEE_LOG(warning, TEXT("�׵���� �ƴϴϴϴ�"), TEXT("-"));
		}
		//�����
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
			background_src_pos_ = coords[back_scroll_default_unit_pos];	//y�� ������ġ ����
			break;
		}
		
		//���� ����
		std::shared_ptr<unit> spawned_unit = std::make_shared<unit>();
		units_.push_back(spawned_unit);
		//���� �ʱ�ȭ
		for (auto& unit_obj : units_) {
			unit_obj->init();
			//���� �淿 �ʱ�ȭ
			for (int i = 0; i != unit_bullet_max_num; i++) {
				unit_obj->init_bullet(unit_obj->get_shoot_type());
			}
		}

		//���� ����
		for (int i = 0; i != monster_spawn_num; i++) {
			std::shared_ptr<monster> spawned_monster = std::make_shared<monster>();
			monsters_.push_back(spawned_monster);
		}
		//���� �ʱ�ȭ
		for (auto& monster_obj : monsters_) {
			monster_obj->init();
			//���� �淿 �ʱ�ȭ
			for (int i = 0; i != monster_bullet_max_num; i++) {
				monster_obj->init_bullet(monster_obj->get_shoot_type());
			}
		}

		//����Ʈ
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
		//��� ƽ
		switch (background_type_) {
		case loop: {
			const float speed = 120.0f;
			switch (background_direction_) {
			//��� ���� �̹���
			//direction �̹��� ���� 0: ��->��.. 1: ��->��.. 2: ��->��.. 3: ��->��
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
			break;	//loop ��
		}

		case scroll: {
			const int background_speed = 10;
			if (units_[0]->get_is_dir_key_pressed()) {
				switch (units_[0]->get_direction()) {
				//��� ���� �̹���
				case 0:
					if (units_[0]->get_now_pos().y > 0 && background_src_pos_.y > 0) {
						background_src_pos_.y -= background_speed;
					}
					break;
				case 1:
					if (units_[0]->get_now_pos().x > 0 && background_src_pos_.x > 0) {
						background_src_pos_.x -= background_speed;
					}
					break;
				case 2:
					if (units_[0]->get_now_pos().y < coords[back_scroll_unit_max_move].y 
						&& background_src_pos_.y < coords[back_scroll_max].y) {
						background_src_pos_.y += background_speed;
					}
					break;
				case 3:
					if (units_[0]->get_now_pos().x < coords[back_scroll_unit_max_move].x 
						&& background_src_pos_.x < coords[back_scroll_max].x) {
						background_src_pos_.x += background_speed;
					}
					break;
				}
			}
			break;
		}//case
		}//switch (kind_of_background)

		//���� ƽ
		for (auto& unit_obj : units_) {
			unit_obj->tick(delta_time);
		}
		for (auto& mon_obj : monsters_) {
			//�� ƽ
			mon_obj->tick(delta_time);


			//�� ����ź ���� ����
			for (auto& bullet_obj : mon_obj->get_bullets()) {
				if (bullet_obj->in_screen()
					&& bullet_obj->get_hp()
					&& bullet_obj->get_move_type() == (int)obj_shoot_type::follow) {
					//�̵� ���� ����
					math::vec2f v{
						units_[0]->get_body().origin - bullet_obj->get_body().origin
					};
					//����ȭ ���� �Ÿ� ���ϱ�
					float dist = sqrtf(v.x * v.x + v.y * v.y);
					//����ȭ
					v /= dist;
					bullet_obj->set_vec_for_player(v);
				}
			}

			//�÷��̾� �Ѿ� vs �� �浹 ƽ
			for (auto& bullet_obj : units_[0]->get_bullets()) {
				if (bullet_obj->in_screen()
					&& shape::intersect(mon_obj->get_body(), bullet_obj->get_body()) != shape::collide_type::none) {

					//���� ����Ʈ ��ġ ����
					for (auto& bomb_obj : bombs_) {
						if (bomb_obj->hp_ == (int)obj_state::die) {
							bomb_obj->hp_ = (int)obj_state::idle;
							bomb_obj->dest_pos_ = mon_obj->get_now_pos();
							bomb_obj->src_pos_ = {0, 0};
							break;
						}
					}

					//���ó��
					mon_obj->set_hp((int)obj_state::die);
					bullet_obj->set_hp((int)obj_state::die);
				}
			}

			//���� ����Ʈ ƽ
			static float delay = 0.0f;
			const float speed = 2.0f;
			const float frame = 3.0f;
			delay += delta_time * speed;
			
			for (auto& bomb_obj : bombs_) {
				if (bomb_obj->hp_ == (int)obj_state::idle) {
					if (delay >= frame) {
						delay = (float)math::fmod(delay, frame);
						bomb_obj->hp_ = (int)obj_state::die;
					}
					bomb_obj->src_pos_.x = coords[effect_bomb_size].x * (int)delay;
					break;
				}
			}

			//�� �Ѿ� vs �÷��̾� �浹 ƽ
			for (auto& bullet_obj : mon_obj->get_bullets()) {
				if (bullet_obj->in_screen()
					&& shape::intersect(units_[0]->get_body(), bullet_obj->get_body()) != shape::collide_type::none) {

					units_[0]->set_hp((int)obj_state::die);
					bullet_obj->set_hp((int)obj_state::die);
				}
			}

			//�� vs �÷��̾� �浹 ƽ
			if (shape::intersect(mon_obj->get_body(), units_[0]->get_now_pos())	!= shape::collide_type::none) {

				units_[0]->set_hp((int)obj_state::die);
			}
		}
	}

	void stage::render(win32gdi::device_context_base& dest_dc, const float& g_fps) {
		if (back_buffer_.is_valid()) {
			switch (background_type_)	{
			case loop:
				//��� ���� �̹���
				//direction �̹��� ���� 0: ��->��.. 1: ��->��.. 2: ��->��.. 3: ��->��
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

		//����
		for (auto& unit_obj : units_) {
			unit_obj->render(back_buffer_);
		}
		//����
		for (auto& mon_obj : monsters_) {
			mon_obj->render(back_buffer_);
		}

		//����
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