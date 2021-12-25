#include "stage.h"

namespace zee {
	using namespace math;

	stage::stage() noexcept {}
	stage::~stage() noexcept {}

	void stage::on_app_started() {
		application_delegates::on_client_size_changed().add_sp(shared_from_this(), &stage::on_resize);
		tick_manager::get().add(shared_from_this());
		on_resize(application::get().config().client_size);
		application::get().window_handle<HWND>();	//윈도우 핸들 가져오기
		
		game_init();

		//테스트용
		matrix m;
		//행렬식
		m.set_m2f({ {2,3}, {4,6} });
		m.determinant();
		//항등행렬
		m.set_m3f({ {1, 0, 0}, {0,1,0}, {0,0,1} });
		if (m.is_identity()) {
			ZEE_LOG(warning, TEXT("항등행렬 임"), TEXT("-"));
		}
		else {
			ZEE_LOG(warning, TEXT("항등행렬 아니니니니"), TEXT("-"));
		}
		//역행렬
		std::vector<math::vec2f> vv = { {2,3}, {4,5} };
		m.inverse(vv);
		ZEE_LOG(warning, TEXT("중단점 찍는곳"), TEXT("-"));
	}
	void stage::game_init() {
		std::shared_ptr<unit> spawned_unit = std::make_shared<unit>();
		std::shared_ptr<monster> spawned_monster = std::make_shared<monster>();

		switch (kind_of_background)	{
		case loop:
			background_image::get().load_background_image({ 800, 1200 }, TEXT("assets/game_background_loop_vertical.bmp"));
			//kind_of_background scroll 모드 때문에 유닛이 들고 있는 배경 소스 데이터를 loop에서도 그대로 활용 (loop 모드만 하면 stage가 들고 있을 듯)
			spawned_unit->set_background_src_pos({ 0, 0 });
			spawned_unit->set_background_src_size({ 800, 1200 });	//루프 때 나머지 연산 필요
			break;
		case scroll:
			background_image::get().load_background_image({ 1152, 2048 }, TEXT("assets/game_background_stop.bmp"));
			//kind_of_background scroll 모드에선 유닛 위치에 따라 배경 소스의 위치가 다름. 하여 메인 unit이 들고 있게 함
			spawned_unit->set_background_src_pos({ 0, 1100 });	//y축 시작위치 설정
			break;
		}
		//프레임 이미지
		frame_image::get().load_frame_image({ 1152, 2048 }, { 64, 64 }, TEXT("assets/walk.bmp"));

		//유닛 세팅
		spawned_unit->set_size({ 64, 64 });
		spawned_unit->set_max_move_size({ 705, 770 });
		spawned_unit->set_now_pos({ 350, 650 });
		units_.push_back(spawned_unit);
		//몬스터 세팅
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