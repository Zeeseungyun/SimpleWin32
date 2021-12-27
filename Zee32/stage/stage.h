#pragma once
#include "../win32helper/win32helper.h"
#include "../application/application_delegates.h"
#include <memory>
#include <interfaces/tickable.h>
#include "../application/application.h"
#include "../application/key_state.h"
#include "../application/tick_manager.h"
#include "background_image.h"
#include "unit.h"
#include "monster.h"
#include "bullet.h"
#include "../../ZeeCore/public/math/matrix.h"//테스트용
#include "../../ZeeCore/public/shape/intersect.h"

namespace zee {
	enum kind_of_background {
		loop, scroll
	};
	//@param kind_of_background ==> background 종류 변경
	static const int kind_of_background = loop;
	//@param background_direction_ ==> background loop 방향 조정 0~3
	static const int background_direction_ = 2;

	enum class frame_image_index {
		player,
		bullet,
		monster
	};

	class stage : public interfaces::tickable, public std::enable_shared_from_this<stage> {
	public:
		stage() noexcept;
		virtual ~stage() noexcept;

		void on_app_started();
		void game_init();
		void on_resize(const math::vec2i& client_size);
		void tick(float delta_time) override;
		void render(win32gdi::device_context_base& dest_dc);

		const math::vec2f get_background_src_pos() const;
		const math::vec2f get_background_src_size() const;
		void set_background_src_pos(const math::vec2f& src_pos);
		void set_background_src_size(const math::vec2f& size);

	private:
		win32gdi::device_context_dynamic back_buffer_;
		std::vector<std::shared_ptr<unit>> units_;
		std::vector<std::shared_ptr<monster>> monsters_;

		math::vec2f background_src_pos_;
		math::vec2i background_src_size_;
	};
}

