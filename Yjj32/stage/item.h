#pragma once
#include "frame_image.h"
#include "projectile.h"


namespace yjj {

	class player;

	class item final : public projectile {
	public:
		item() noexcept = default;
		virtual ~item() noexcept = default;

		virtual void load_image() final;

		virtual void init() final;
		virtual void spawn_from(const std::shared_ptr<unit> other) final;
		virtual void hit_from(const std::shared_ptr<unit> other, const float delta_time) final;
		virtual void destroy(const float delta_time) final;

		virtual void render(win32gdi::device_context_dynamic& dest_dc) final;

	private:
		enum const_var_ {
			item_size,
			destroy_item_zone,
		};
		const std::vector<math::vec2i> coords_item_{
			{32, 32},		//item_size
			{3000, 3000},	//destroy_item_zone
		};
	};
}