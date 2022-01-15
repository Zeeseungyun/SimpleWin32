#pragma once
#include "frame_image.h"
#include "unit.h"

namespace zee {
	//특징: die 상태로 게임시작, 어떤 obj로부터 스폰되며 살아남.
	class projectile : public unit {
	public:
		projectile() noexcept = default;
		virtual ~projectile() noexcept = default;

		virtual void init() override;
		//spawn_from(other): effect, item 전용 풀링
		virtual void spawn_from(const std::shared_ptr<unit> other);
		//spawn_from(obj_type, obj_body): bullet 전용 풀링
		virtual void spawn_from(const int obj_type, const shape::circlef& obj_body);

		virtual void render(win32gdi::device_context_dynamic& dest_dc) override;
	};
}