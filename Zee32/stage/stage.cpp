#include "stage.h"

namespace zee {
	using namespace math;

	stage::stage() noexcept {}
	stage::~stage() noexcept {}

	void stage::on_app_started() {
		application_delegates::on_client_size_changed().add_sp(shared_from_this(), &stage::on_resize);
		tick_manager::get().add(shared_from_this());
		on_resize(application::get().config().client_size);
		application::get().window_handle<HWND>();	//������ �ڵ� ��������
		
		game_init();

		//�׽�Ʈ��
		matrix m;
		//��Ľ�
		m.set_m2f({ {2,3}, {4,6} });
		m.determinant();
		//�׵����
		m.set_m3f({ {1, 0, 0}, {0,1,0}, {0,0,1} });
		if (m.is_identity()) {
			ZEE_LOG(warning, TEXT("�׵���� ��"), TEXT("-"));
		}
		else {
			ZEE_LOG(warning, TEXT("�׵���� �ƴϴϴϴ�"), TEXT("-"));
		}
		//�����
		std::vector<math::vec2f> vv = { {2,3}, {4,5} };
		m.inverse(vv);
		ZEE_LOG(warning, TEXT("�ߴ��� ��°�"), TEXT("-"));
	}
	void stage::game_init() {
		std::shared_ptr<unit> spawned_unit = std::make_shared<unit>();
		std::shared_ptr<monster> spawned_monster = std::make_shared<monster>();

		switch (kind_of_background)	{
		case loop:
			background_image::get().load_background_image({ 800, 1200 }, TEXT("assets/game_background_loop_vertical.bmp"));
			//kind_of_background scroll ��� ������ ������ ��� �ִ� ��� �ҽ� �����͸� loop������ �״�� Ȱ�� (loop ��常 �ϸ� stage�� ��� ���� ��)
			spawned_unit->set_background_src_pos({ 0, 0 });
			spawned_unit->set_background_src_size({ 800, 1200 });	//���� �� ������ ���� �ʿ�
			break;
		case scroll:
			background_image::get().load_background_image({ 1152, 2048 }, TEXT("assets/game_background_stop.bmp"));
			//kind_of_background scroll ��忡�� ���� ��ġ�� ���� ��� �ҽ��� ��ġ�� �ٸ�. �Ͽ� ���� unit�� ��� �ְ� ��
			spawned_unit->set_background_src_pos({ 0, 1100 });	//y�� ������ġ ����
			break;
		}
		//������ �̹���
		frame_image::get().load_frame_image({ 1152, 2048 }, { 64, 64 }, TEXT("assets/walk.bmp"));

		//���� ����
		spawned_unit->set_size({ 64, 64 });
		spawned_unit->set_max_move_size({ 705, 770 });
		spawned_unit->set_now_pos({ 350, 650 });
		units_.push_back(spawned_unit);
		//���� ����
		spawned_monster->set_rotate_point({ 0.0f, 0.0f });
		spawned_monster->set_rotate_origin({ 0.0f, 0.0f });
		monsters_.push_back(spawned_monster);
	}

	void stage::on_resize(const math::vec2i& client_size) {
		if (back_buffer_.is_valid()) {
			back_buffer_.resize(client_size);
		}
		else {
			back_buffer_.create_empty_image(client_size);
		}
	}

	void stage::tick(float delta_time) {
		for (auto& unit_obj : units_) {
			unit_obj->tick(delta_time);
		}
		for (auto& mon_obj : monsters_) {
			mon_obj->tick(delta_time);
		}
	}

	void stage::render(win32gdi::device_context_base& dest_dc) {
		if (back_buffer_.is_valid()) {
			switch (kind_of_background)	{
			case loop:
				background_image::get().render_loop(back_buffer_, units_[0]->get_background_src_pos(), 2);
				break;

			case scroll:
				background_image::get().render(back_buffer_, units_[0]->get_background_src_pos());
				break;
			}

			for (auto& unit_obj : units_) {
				unit_obj->render(back_buffer_);
			}

			for (auto& mon_obj : monsters_) {
				mon_obj->render(back_buffer_);
			}

			back_buffer_.bit_blt(dest_dc, {});
		}
	}
}