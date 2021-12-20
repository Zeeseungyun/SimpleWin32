#pragma once
#include "../win32helper/win32helper.h"
#include "../application/application_delegates.h"
#include <memory>
#include <interfaces/tickable.h>
#include "../application/application.h"
#include "../application/key_state.h"
#include "../application/tick_manager.h"
#include "frame_image.h"
#include "background_image.h"
#include "unit.h"

namespace zee {
	using namespace win32gdi;

	enum e_kind_of_background {
		loop, stop
	};
	//@param kind_of_background ==> background ���� �����Ϸ��� ���⼭ ����
	static const int kind_of_background = loop;

	class stage : public interfaces::tickable, public std::enable_shared_from_this<stage> {
	public:
		stage() noexcept;
		virtual ~stage() noexcept;

		void on_app_started();
		void game_init();
		void on_resize(const math::vec2i& client_size);
		void tick(float delta_time) override;
		void render(device_context_base& dest_dc);

	private:
		device_context_dynamic back_buffer;
		unit unit_;
		math::vec2i background_src_pos;
	};
}

