#pragma once
#include "frame_image.h"
#include "unit.h"

namespace zee {
	class projectile : public unit {
	public:
		projectile() noexcept = default;
		virtual ~projectile() noexcept = default;

		virtual void init(const int obj_state) override;
		virtual void spawn_from(const std::shared_ptr<unit> other);	//projectile ���� ����(����Ʈ, ������)
		virtual void spawn_from(const int obj_type, const shape::circlef& obj_body);	//projectile ���� ����(�淿)

	protected:

	};
}