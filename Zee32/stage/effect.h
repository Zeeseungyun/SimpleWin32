#pragma once
#include "frame_image.h"
#include "projectile.h"

namespace zee {
	class effect final : public projectile {
	public:
		effect() noexcept = default;
		virtual ~effect() noexcept = default;

		virtual void load_image() final;

		virtual void init() final;
		virtual void spawn_from(const std::shared_ptr<unit> other) final;
		virtual void destroy(const float delta_time) final;

		virtual void render(win32gdi::device_context_dynamic& dest_dc) final;

		float get_delay_bomb() const;
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