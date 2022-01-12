#pragma once
#include <vector>
#include "frame_image.h"
#include "bullet.h"

namespace zee {
	class plane : public unit {
	public:
		plane() noexcept = default;
		virtual ~plane() noexcept = default;

		virtual void init(const int obj_state) override;
		virtual void spawn();	//plane 전용 스폰


	protected:
		float delay_shoot_;
		float delay_destroy_;
	};
}