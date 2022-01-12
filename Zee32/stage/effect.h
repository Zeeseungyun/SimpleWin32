#pragma once
#include "projectile.h"
#include "frame_image.h"

namespace zee {
	class effect : public projectile {
	public:
		effect() noexcept = default;
		virtual ~effect() noexcept = default;

		virtual void load_image() override;

		virtual void init(const int obj_state) override;
		virtual void spawn_from(const std::shared_ptr<unit> other) override;
		virtual void destroy(const float delta_time) override;

		virtual void render(win32gdi::device_context_dynamic& dest_dc) override;

		const float get_delay_bomb() const;
		void set_delay_bomb(const float delay);

	private:
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