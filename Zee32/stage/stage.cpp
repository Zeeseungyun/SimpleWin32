#include "stage.h"

namespace zee {
	using namespace math;

	void stage::on_app_started() {
		application_delegates::on_client_size_changed().add_sp(shared_from_this(), &stage::on_resize);
		tick_manager::get().add(shared_from_this());
		on_resize(application::get().config().client_size);
		application::get().window_handle<HWND>();	//������ �ڵ� ��������

		//�̹��� �ε�
		//���
		background_image::get().load_background_image(coords[back_max_size]
			, TEXT("assets/game_background_loop_vertical.bmp"));
		background_image::get().load_background_image(coords[back_scroll_max_size]
			, TEXT("assets/game_background_stop.bmp"));

		//����
		//�÷��̾�
		std::shared_ptr<player> player_obj = std::make_shared<player>();
		player_obj->load_image();
		//����
		std::shared_ptr<monster> monster_obj = std::make_shared<monster>();
		monster_obj->load_image();
		//�淿
		std::shared_ptr<bullet> bullet_obj = std::make_shared<bullet>();
		bullet_obj->load_image();

		//����Ʈ
		std::shared_ptr<effect> bomb_obj = std::make_shared<effect>();
		bomb_obj->load_image();
		//������
		std::shared_ptr<item> item_obj = std::make_shared<item>();
		item_obj->load_image();

		//���� �ʱ�ȭ
		init_game();

		//��� �׽�Ʈ
		matrix2f m2;
		matrix3f m3;
		//��
		//plg_blt ����
		//��ġ���
		m2.set_vec({ { 0,1 }, { 3,4 } });
		m2.transposed();
		m2.transposed();
		m3.set_vec({ { 0,1,2 }, { 3,4,5 }, {6,7,8} });
		m3.transposed();
		m3.transposed();
		//��Ľ�
		m2.set_vec({{2,3}, {4,6}});
		float a2 = m2.determinant();
		m3.set_vec({ {2,3,4}, {5,6,7}, {8,9,10} });
		float a3 = m3.determinant();
		//�׵����
		m2.set_vec({ {1,0}, {0,1} });
		bool b2 = m2.is_identity();
		m3.set_vec({ {1,0,0}, {0,1,0}, {0,0,1} });
		bool b3 = m3.is_identity();
		//�����
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
			background_src_pos_ = coords[back_scroll_default_unit_pos];	//y�� ������ġ ����
			break;
		}

		//���� ����
		std::shared_ptr<player> spawned_player = std::make_shared<player>();
		players_.push_back(spawned_player);
		//���� �ʱ�ȭ
		players_.front()->init();

		//���� ����
		//for (int i = 0; i != 1; i++) {	//�׽�Ʈ
		for (int i = 0; i != monster_spawn_num; i++) {
			std::shared_ptr<monster> spawned_monster = std::make_shared<monster>();
			monsters_.push_back(spawned_monster);
		}
		//���� �ʱ�ȭ
		for (auto& monster_obj : monsters_) {
			monster_obj->init();
		}
	}

	void stage::tick(float delta_time) {
		//�ð� ƽ
		game_time_ += delta_time;

		//��� ƽ
		switch (background_type) {
		case loop: {
			const float speed = 120.0f;
			switch (background_direction) {
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
			break;
		}//case loop

		case scroll: {
			const float background_speed = 10.0f;
			if (players_.front()->get_is_dir_key_pressed()) {
				switch (players_.front()->get_direction()) {
					//��� ���� �̹���
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

		//�÷��̾� ƽ
		for (auto& player_obj : players_) {
			player_obj->move(delta_time);
			player_obj->shoot(delta_time);
			player_obj->destroy(delta_time);

			//������ vs �÷��̾� �浹
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
			//�� ƽ
			mon_obj->move(delta_time);
			mon_obj->shoot(delta_time);
			mon_obj->destroy(delta_time);

			//�� ȣ��
			if (mon_obj->get_obj_type() 
				== (int)unit::obj_type::monster_arround) 
			{
				//�÷��̾���� ���� ����
				math::vec2f v_mon_for_player{
					players_.front()->get_body().origin - mon_obj->get_body().origin
				};
				mon_obj->set_vec_for_player(v_mon_for_player);
			}

			//�� ȣ��ź(����ź)
			for (auto& bullet_obj : mon_obj->get_bullets()) {
				//ȣ�� ����
				if (bullet_obj->get_obj_type() 
					== (int)unit::obj_type::monster_bullet_homing) 
				{
					//�÷��̾���� ���� ����: normalize�� ���⼭ ����� ��
					math::vec2f v_bullet_for_player{
						players_.front()->get_body().origin - bullet_obj->get_body().origin
					};
					v_bullet_for_player = v_bullet_for_player.normalize();
					bullet_obj->set_vec_for_player(v_bullet_for_player);
				}

				//�� �淿(Ư���� �淿��) vs �÷��̾� �淿 �浹 ƽ
				if (bullet_obj->get_obj_type() == (int)unit::obj_type::monster_bullet_homing) {

					for (auto& bullet_player_obj : players_.front()->get_bullets())
					{
						if (shape::intersect(bullet_player_obj->get_body(), bullet_obj->get_body())
							!= shape::collide_type::none)
						{
							//���� ����Ʈ: �ǰ� ���� ���� ���� ��ġ ���
							spawn_bomb(bullet_obj);

							//�ǰ�
							//�� �淿�� �÷��̾� �淿���� �¾Ҵٰ� �Ǵ� (�÷��̾� �淿�� ���ݷ¸�ŭ ��������)
							//�÷��̾� �淿�� �� �淿���� ~
							bullet_obj->hit_from(bullet_player_obj, delta_time);
							bullet_player_obj->hit_from(bullet_obj, delta_time);
							bullet_obj->set_now_pos_and_body(coords[back_destroy_zone]);
							bullet_player_obj->set_now_pos_and_body(coords[back_destroy_zone]);
						}//if
					}//for
				}//if
			}


			//�÷��̾� �淿 vs �� �浹 ƽ
			for (auto& bullet_obj : players_.front()->get_bullets()) {
				if (shape::intersect(mon_obj->get_body(), bullet_obj->get_body()) 
					!= shape::collide_type::none) 
				{
					//���� ����Ʈ: �ǰ� ���� ���� ���� ��ġ ���
					spawn_bomb(mon_obj);
					//������
					spawn_item(mon_obj);

					//�ǰ�
					//���� �淿�� �ƴ� �÷��̾�� �¾Ҵٰ� �Ǵ�(���� �ֱ� ����)(�÷��̾��� ���ݷ¸�ŭ ��������)
					mon_obj->hit_from(players_.front(), delta_time);
					bullet_obj->hit_from(mon_obj, delta_time);
				}
			}

			//�� �淿 vs �÷��̾� �浹 ƽ
			for (auto& bullet_obj : mon_obj->get_bullets()) {
				if (shape::intersect(players_.front()->get_body(), 
					bullet_obj->get_body())
					!= shape::collide_type::none) 
				{
					//���� ����Ʈ: �ǰ� ���� ���� ���� ��ġ ���
					spawn_bomb(players_.front());

					//�ǰ�
					//�÷��̾�� �� �淿���� �¾Ҵٰ� �Ǵ� (�淿�� ���ݷ¸�ŭ ��������)
					players_.front()->hit_from(bullet_obj, delta_time);
					bullet_obj->hit_from(players_.front(), delta_time);
					bullet_obj->set_now_pos_and_body(coords[back_destroy_zone]);
				}
			}

			//�� vs �÷��̾� �浹 ƽ
			if (shape::intersect(mon_obj->get_body(), 
				players_.front()->get_now_pos())
				!= shape::collide_type::none) 
			{
				//���� ����Ʈ: �ǰ� ���� ���� ���� ��ġ ���
				spawn_bomb(players_.front());

				//�ǰ� (������ ���ݷ¸�ŭ ��������)
				players_.front()->hit_from(mon_obj, delta_time);

			}
		}//for (auto& mon_obj : monsters_)


		//����Ʈ ƽ
		for (auto& bomb_obj : bombs_) {
			bomb_obj->destroy(delta_time);
		}

		//����Ʈ ����
		for (int i = 0; i != bombs_.size(); ) {
			if (!(bombs_[i]->in_screen()) || bombs_[i]->get_state() == (int)unit::obj_state::die) {
				bombs_.erase(bombs_.begin() + i);
			}
			else {
				i++;
			}
		}

		//������ ����
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
				//��� ���� �̹���
				//direction �̹��� ���� 0: ��->��.. 1: ��->��.. 2: ��->��.. 3: ��->��
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

		//����
		for (auto& player_obj : players_) {
			player_obj->render(back_buffer_);
		}
		//����
		for (auto& mon_obj : monsters_) {
			mon_obj->render(back_buffer_);
		}

		//�淿: ���ְ� ���� render�� ������

		//����Ʈ
		for (auto& bomb_obj : bombs_) {
			bomb_obj->render(back_buffer_);
		}

		//������
		for (auto& item_obj : items_) {
			item_obj->render(back_buffer_);
		}


		//�����ڽ�
		shape::rectf box_info(830, 700, 1030, 880);
		back_buffer_.rectangle(box_info);
		back_buffer_.print_text({ 850, 720 }, TEXT("������: "));
		back_buffer_.print_text({ 920, 720 }, to_tstring(to_wstring(g_fps)));
		back_buffer_.print_text({ 850, 740 }, TEXT("�ð�: "));
		back_buffer_.print_text({ 920, 740 }, to_tstring(to_wstring(static_cast<int>(game_time_))));
		back_buffer_.print_text({ 850, 760 }, TEXT("����: "));
		back_buffer_.print_text({ 920, 760 }, to_tstring(to_wstring(players_.front()->get_my_score())));
		back_buffer_.print_text({ 850, 800 }, TEXT("�浹����:    Tab"));
		back_buffer_.print_text({ 850, 820 }, TEXT("������:        R"));

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