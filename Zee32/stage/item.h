#pragma once
#include "unit.h"
#include "frame_image.h"


namespace zee {
	//이래야 했다.
	class player;
	class item : public unit {
	public:
		item() noexcept = default;
		virtual ~item() noexcept = default;

		virtual void load_image() override;

		virtual void init() override;

		virtual void hit_from(const std::shared_ptr<unit> other, const float delta_time) override;
		virtual void destroy(const float delta_time) override;
		virtual void render(win32gdi::device_context_dynamic& dest_dc) override;

	private:
		enum const_var_ {
			item_size,
		};
		const std::vector<math::vec2i> coords_{
			{32, 32},		//item_size
		};
	};
}