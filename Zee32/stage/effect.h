#pragma once
#include "unit.h"
#include "frame_image.h"

namespace zee {
	class effect : public unit {
	public:
		effect() noexcept = default;
		virtual ~effect() noexcept = default;

		virtual void load_image() override;

		virtual void init() override;

		virtual void destroy(const float delta_time) override;
		virtual void render(win32gdi::device_context_dynamic& dest_dc) override;

		void set_delay(const float delay);

	private:
		//얘느 ㄴ적당한듯?
		enum const_var_ {
			effect_bomb_size,
			effect_bomb_default_frame,
			effect_bomb_final_frame,
		};
		const std::vector<math::vec2i> coords_{
			{32, 32},		//effect_bomb_size
			{0, 0},			//effect_bomb_default_frame
			{96, 0},		//effect_bomb_final_frame
		};

		float delay_bomb_;
	};
}