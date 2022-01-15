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
		application::get().window_handle<HWND>();	//������ �ڵ� ��������

		//�̹��� �ε�
		//���
		if (background_type == loop) {
			background_image::get().load_background_image(coords[back_max_size]
				, TEXT("assets/game_background_loop_vertical.bmp"));
		}
		else if (background_type == scroll) {
			background_image::get().load_background_image(coords_stage[back_scroll_max_size]
				, TEXT("assets/game_background_stop.bmp"));
		}

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
		std::shared_ptr<effect> effect_obj = std::make_shared<effect>();
		effect_obj->load_image();
		//������
		std::shared_ptr<item> item_obj = std::make_shared<item>();
		item_obj->load_image();


		//���� �ʱ�ȭ
		init_game();


		//��� �׽�Ʈ
		/*
		matrix2f m2;
		matrix3f m3;
		//��ġ���
		m2.set_vec({ { 0,1 }, { 3,4 } });
		m2.transposed();
		m2.transposed();
		m3.set_vec({ { 0,1,2 }, { 3,4,5 }, {6,7,8} });
		m3.transposed();
		m3.transposed();
		//��Ľ�
		m2.set_vec({ {2,3}, {4,6} });
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
		//��
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

		//���
		switch (background_type) {
		//@[[likely]] likely and unlikely attributes: c++ 20. �ش� ���̺��� ����� ���ɼ��� ���ٴ� ��Ʈ�� ������.
		[[likely]] case loop:
			set_background_src_pos({});
			set_background_src_size(coords_stage[back_loop_max_size]);
			break;
		case scroll:
			set_background_src_size(coords_stage[back_scroll_default_unit_pos]);	//y�� ������ġ ����
			break;
		}


		///obj �ʱ�ȭ

		//�÷��̾�
		std::shared_ptr<player> spawned_player = std::make_shared<player>();
		spawned_player->init();
		players_.push_back(spawned_player);


		//����
		for (int i = 0; i != monster_spawn_num; i++) {
			std::shared_ptr<monster> spawned_monster = std::make_shared<monster>();
			spawned_monster->init();
			monsters_.push_back(spawned_monster);
		}


		//����Ʈ
		for (int i = 0; i != effect_spawn_num; i++) {
			std::shared_ptr<effect> spawned_effect = std::make_shared<effect>();
			spawned_effect->init();
			effects_.push_back(spawned_effect);
		}


		//������
		for (int i = 0; i != item_spawn_num; i++) {
			std::shared_ptr<item> spawned_item = std::make_shared<item>();
			spawned_item->init();
			items_.push_back(spawned_item);
		}
	}

	void stage::tick(float delta_time) {

		//�ð� ƽ
		set_game_time(get_game_time() + delta_time);

		//��� ƽ
		switch (background_type) {
		[[likely]] case loop: {
			const float speed = 120.0f;
			switch (background_direction) {
				//��� ���� �̹���
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
					//��� ���� �̹���
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
		{//�̷������� �������� ���μ�
			simple_stat<> stat;//������ ȣ��Ǹ� ���⼭ �ʱ�ȭ���ϰ�.
			//���� ������ ���⼭ �� �Լ� ȣ���ϸ� �ð��� �����ŵ翩? ��������
			ZEE_LOG(normal, TEXT("tick check"), TEXT("player tick : %f"), stat.mili_sec());
		}
		*/

		///////
		//�÷��̾� ƽ
		///////

		//�̵�
		get_players()[0]->move(delta_time);

		//�߻�
		get_players()[0]->shoot(delta_time);

		//�浹: ������ vs �÷��̾�
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

		//�÷��̾� ����Ǵ�
		if (get_players()[0]->get_hp() == 0 || !(get_players()[0]->in_screen())) {
			get_players()[0]->destroy(delta_time);
		}



		//�÷��̾� �淿 ����Ǵ�
		for (auto& bullet_player_obj : get_players()[0]->get_bullets()) {
			if (bullet_player_obj->get_hp() == 0 || !(bullet_player_obj->in_screen())) {
				bullet_player_obj->destroy(delta_time);
			}
		}



		///////
		//�� ƽ
		///////
		for (auto& mon_obj : get_monsters()) {
			//����, �� ����� Ÿ���̶�� �÷��̾� ���� ��ġ ����
			if (mon_obj->get_obj_type()
				== (int)obj_type::monster_arround)
			{
				//�÷��̾���� ���� ����
				math::vec2f v_mon_for_player{
					get_players()[0]->get_body().origin - mon_obj->get_body().origin
				};
				mon_obj->set_vec_for_player(v_mon_for_player);
			}


			//�� �̵�
			mon_obj->move(delta_time);

			//�� �߻�
			mon_obj->shoot(delta_time);


			//�浹: �� vs �÷��̾�
			if (shape::intersect(
				mon_obj->get_body(),
				get_players()[0]->get_body())
				!= shape::collide_type::none)
			{
				//����Ʈ: �ǰ� ���� ���� ���� ��ġ ���
				for (auto& effect_obj : get_effects()) {
					if (effect_obj->get_state() == (int)obj_state::die) {
						effect_obj->spawn_from(get_players()[0]);
						break;
					}
				}

				//�ǰ�
				get_players()[0]->hit_from(mon_obj, delta_time);
			}



			//�� �淿
			for (auto& bullet_monster_obj : mon_obj->get_bullets()) {
				//�� ȣ�� �淿(����ź) Ÿ��
				//ȣ�� �淿�� �÷��̾ ����
				if (bullet_monster_obj->get_obj_type()
					== (int)obj_type::monster_bullet_homing)
				{
					//�÷��̾���� ���� ����: normalize�� ���⼭ ����� ��
					math::vec2f v_bullet_for_player{
						get_players()[0]->get_body().origin - bullet_monster_obj->get_body().origin
					};
					v_bullet_for_player = v_bullet_for_player.normalize();
					bullet_monster_obj->set_vec_for_player(v_bullet_for_player);
				}


				//�浹: ȣ�� �淿 vs �÷��̾� �淿
				if (bullet_monster_obj->get_obj_type() == (int)obj_type::monster_bullet_homing) {

					for (auto& bullet_player_obj : get_players()[0]->get_bullets())
					{
						if (shape::intersect(
							bullet_player_obj->get_body(),
							bullet_monster_obj->get_body())
							!= shape::collide_type::none)
						{
 							//����Ʈ: �ǰ� ���� ���� ���� ��ġ ���
 							for (auto& effect_obj : get_effects()) {
								if (effect_obj->get_state() == (int)obj_state::die) {
									effect_obj->spawn_from(bullet_monster_obj);
									break;
								}
							}

							//�ǰ�
							//�� �淿�� �÷��̾� �淿���� �¾Ҵٰ� �Ǵ� (�÷��̾� �淿�� ���ݷ¸�ŭ ��������)
							//�÷��̾� �淿�� �� �淿���� �¾Ҵٰ� �Ǵ� (�� �淿�� ���ݷ¸�ŭ ��������)
							bullet_monster_obj->hit_from(bullet_player_obj, delta_time);
							bullet_player_obj->hit_from(bullet_monster_obj, delta_time);

						}//if
					}//for
				}//if
				

				//�浹: �� �淿 vs �÷��̾�
				if (shape::intersect(
					get_players()[0]->get_body(),
					bullet_monster_obj->get_body())
					!= shape::collide_type::none)
				{
					//����Ʈ: �ǰ� ���� ���� ���� ��ġ ���
					for (auto& effect_obj : get_effects()) {
						if (effect_obj->get_state() == (int)obj_state::die) {
							effect_obj->spawn_from(get_players()[0]);
							break;
						}
					}

					//�ǰ�
					//�÷��̾�� �� �淿���� �¾Ҵٰ� �Ǵ� (�淿�� ���ݷ¸�ŭ ��������)
					get_players()[0]->hit_from(bullet_monster_obj, delta_time);
					bullet_monster_obj->hit_from(get_players()[0], delta_time);
				}



				//�� �淿 ����Ǵ�
				if (bullet_monster_obj->get_hp() == 0 || !(bullet_monster_obj->in_screen())) {
					bullet_monster_obj->destroy(delta_time);
				}
			}


			//�浹: �÷��̾� �淿 vs ��
			for (auto& bullet_player_obj : get_players()[0]->get_bullets()) {
				if (shape::intersect(
					mon_obj->get_body(),
					bullet_player_obj->get_body())
					!= shape::collide_type::none)
				{
					//����Ʈ: �ǰ� ���� ���� ���� ��ġ ���
					for (auto& effect_obj : get_effects()) {
						if (effect_obj->get_state() == (int)obj_state::die) {
							effect_obj->spawn_from(bullet_player_obj);
							break;
						}
					}

					//�ǰ�
					//���� �淿�� �ƴ� �÷��̾�� �¾Ҵٰ� �Ǵ�(���� �ֱ� ����)(�÷��̾��� ���ݷ¸�ŭ ��������)
					mon_obj->hit_from(get_players()[0], delta_time);
					bullet_player_obj->hit_from(mon_obj, delta_time);
				}
			}



			//���� ����Ǵ�
			if (mon_obj->get_hp() == 0 || !(mon_obj->in_screen())) {

				//������: �ǰ� ���� ���� ���� ��ġ ���
				for (auto& item_obj : get_items()) {
					if (item_obj->get_state() == (int)obj_state::die) {
						item_obj->spawn_from(mon_obj);
						break;
					}
				}

				//���
				mon_obj->destroy(delta_time);
			}

		}//for (auto& mon_obj : monsters_)


		//����Ʈ ���: �����ð� �� �Ǹ�.
		for (auto& effect_obj : get_effects()) {
			if (effect_obj->get_state() == (int)obj_state::idle) {
				effect_obj->destroy(delta_time);
			}
		}

		//����
		if (get_players()[0]->get_high_score() < get_players()[0]->get_my_score()) {
			get_players()[0]->set_high_score(get_players()[0]->get_my_score());
		}



		//������: ���� ����
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
			
			//���
			switch (background_type) {
			[[likely]] case loop:
				//��� ���� �̹���
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

		//�÷��̾�
		for (auto& player_obj : get_players()) {
			player_obj->render(back_buffer_);
		}
		
		get_monsters()[0]->render(back_buffer_);
		
		//����
		for (auto& mon_obj : get_monsters()) {
			mon_obj->render(back_buffer_);
		}
		
		//������ ����
		for (auto& item_obj : get_items()) {
			item_obj->render(back_buffer_);
		}

		//����Ʈ ����
		for (auto& bomb_obj : get_effects()) {
			bomb_obj->render(back_buffer_);
		}


		//�����ڽ�
		shape::rectf box(700, 0, 1200, 1100);
		back_buffer_.rectangle(box);
		shape::rectf box_info(830, 700, 1030, 900);
		back_buffer_.rectangle(box_info);
		back_buffer_.print_text({ 850, 720 }, TEXT("������: "));
		back_buffer_.print_text({ 930, 720 }, to_tstring(to_wstring(g_fps)));
		back_buffer_.print_text({ 850, 740 }, TEXT("�ð�: "));
		back_buffer_.print_text({ 930, 740 }, to_tstring(to_wstring(static_cast<int>(get_game_time()))));
		back_buffer_.print_text({ 850, 760 }, TEXT("����: "));
		back_buffer_.print_text({ 930, 760 }, to_tstring(to_wstring(get_players()[0]->get_my_score())));
		back_buffer_.print_text({ 850, 780 }, TEXT("�ְ�����: "));
		back_buffer_.print_text({ 930, 780 }, to_tstring(to_wstring(get_players()[0]->get_high_score())));
		back_buffer_.print_text({ 850, 820 }, TEXT("�浹����:    Tab"));
		back_buffer_.print_text({ 850, 840 }, TEXT("������:        R"));

		
		//�� ���� �׸���
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