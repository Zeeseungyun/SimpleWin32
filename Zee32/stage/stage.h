#pragma once
#include "../win32helper/win32helper.h"
#include "../application/application.h"
#include "../application/application_delegates.h"
#include <memory>
#include <interfaces/tickable.h>
#include "../application/key_state.h"
#include "../application/tick_manager.h"
#include "../../ZeeCore/public/math/matrix.h"
#include "../../ZeeCore/public/shape/intersect.h"
#include "background_image.h"
#include "frame_image.h"

/*
1. ��� ���� ���� (�� �� ���ڰ� �غ���!)
2. �̵� - > �浹 - > ��ó�� -> ����
3. ���� ����
4. Ǯ��


* TODO:: ���漱��Ȱ���� �ȵ���. 
* �ش� ������Ͽ��� player, monster, item, bullet ����� ����.
* �׸��� unit�� ���漱���� �������.
*/

/*
* 1. enum�� ġ���. 2�� ���� obj_type, background_type
* 2. ��� Ȱ�� �غ���. (*������ ����)
* 3. �������� ���������� �ִ��� �Ű�Ἥ �ۼ��Ѵ�.
* 4. ��Ʈ���� �ٽ�!
*
*/

/*
* �����ս��� �������� Ȯ���ϴ� �����
* �ڵ�� ���� �Ǵ��ϱ⵵ �ϴµ� 
* �׷��� ������ �����
* �ð��� ��ϴ�
* ��� �ð��� ���� ���� ��� �Գ� �̰� ����
* �� �װ� ����;��
*/

namespace zee {
	class unit;
	class player;
	class monster;
	class bullet;
	class effect;
	class item;

	//���� enum
	enum background_type {
		loop, scroll
	};
	//@param background_type ==> background ���� ����
	constexpr int background_type = loop;
	//@param background_direction_ ==> background loop ���� ���� 0~3
	constexpr int background_direction = 2;
	//constexpr: �ڵ� �ζ��� ó�� (c++ 14�̻�) (�𸮾󿡼� extern const T&, extern const T*)

	class stage : public interfaces::tickable, public std::enable_shared_from_this<stage> {
	public:
		enum const_var {
			back_destroy_zone,
			back_min_size,
			back_max_size,
			back_loop_max_size,
			back_scroll_max_size,
			back_scroll_default_unit_pos,
			back_scroll_max,
			back_scroll_unit_max_move,

			monster_spawn_num = 3,
		};
		//�̰� �̹� �ǵ찳 �����ϱ�..
		//�帧�� �ľ��ϱ� ���� 
		//�̵� -> �浹ó�� -> ����
		//������ �ϴ°� �����ű�.
		const std::vector<math::vec2i> coords{
			{2048, 2048},	//back_destroy_zone
			{-150, -200},	//back_min_size
			{720, 1000},	//back_max_size
			{720, 1280},	//back_loop_max_size
			{1152, 2048},	//back_scroll_max_size
			{0, 1100},		//back_scroll_default_unit_pos
			{390, 1152},	//back_scroll_max
			{705, 770},		//back_scroll_unit_max_move
		};

		stage() noexcept = default;
		virtual ~stage() noexcept = default;

		void on_app_started();
		void on_resize(const math::vec2i& client_size);

		void init_game();

		void tick(float delta_time) override;
		void spawn_bomb(std::shared_ptr<unit> other);
		void spawn_item(std::shared_ptr<unit> other);
		void render(win32gdi::device_context_base& dest_dc, const float g_fps);

		const math::vec2f get_background_src_pos() const;
		const math::vec2f get_background_src_size() const;
		void set_background_src_pos(const math::vec2f& src_pos);
		void set_background_src_size(const math::vec2f& size);
		//���� frame image�� ���� ���� ����? ��
		//�׷�? �ʿ� ������? ��
	private:
		win32gdi::device_context_dynamic back_buffer_;
		math::vec2f background_src_pos_;
		math::vec2i background_src_size_;
		std::vector<std::shared_ptr<player>> players_;
		std::vector<std::shared_ptr<monster>> monsters_;
		std::vector<std::shared_ptr<effect>> bombs_;
		std::vector<std::shared_ptr<item>> items_;
		float game_time_;
	};
}

