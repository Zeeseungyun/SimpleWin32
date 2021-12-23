#pragma once
#include "../win32helper/win32helper.h"
#include "../application/key_state.h"
#include <interfaces/tickable.h>
#include <vector>
#include "frame_image.h"

namespace zee {
	//@param background_direction_ ==> background loop 방향 조정
	static const int background_direction_ = 2;

	class unit : public interfaces::tickable {
	public:
		unit() noexcept;
		virtual ~unit() noexcept;
		virtual void tick(float delta_time) override;
		void set_frame_direction(const float& delta_time);
		void move(const float& delta_time);
		void render(win32gdi::device_context_dynamic& dest_dc);

		const math::vec2i& get_frame_x() const;
		const math::vec2i& get_frame_y() const;
		const int& get_direction() const;
		const math::vec2i& get_now_pos() const;
		const bool get_is_pressed() const;
		const math::vec2f& get_background_src_pos() const;
		void set_size(const math::vec2i& size);
		void set_max_move_size(const math::vec2i& size);
		void set_now_pos(const math::vec2f& now_pos);
		//@param background_src_pos : kind_of_background scroll 모드에서 유닛의 이동에 따라 배경 위치가 달라져야 하기 때문에 유닛이 들고 있음.
		void set_background_src_pos(const math::vec2f& src_pos);
		void set_background_src_size(const math::vec2f& size);

	private:
		math::vec2i size_;
		math::vec2i max_move_size_;
		math::vec2i frame_x_;
		math::vec2i frame_y_;
		int direction_;
		math::vec2f now_pos_;
		bool is_pressed;
		float frame_per_time_;
		math::vec2f background_src_pos_;
		math::vec2i background_src_size_;
	};
}
