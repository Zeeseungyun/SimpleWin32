#pragma once
#include <vector>
#include "frame_image.h"
#include "bullet.h"

namespace zee {
	class plane : public unit {
	public:
		plane() noexcept = default;
		virtual ~plane() noexcept = default;

		virtual void init() override;
		//spawn(): plane 전용 풀링
		virtual void spawn();

		virtual void shoot(const float delta_time);
		virtual void shoot_per_delay(const float frame);

		virtual void render(win32gdi::device_context_dynamic& dest_dc) override;


		const float get_delay_shoot() const;
		const std::vector<std::shared_ptr<bullet>> get_bullets() const;

		virtual void set_delay(const float delay);
		void set_delay_shoot(const float delay);


	protected:
		float delay_shoot_;
		float delay_destroy_;

		std::vector<std::shared_ptr<bullet>> bullets_;
	};
}