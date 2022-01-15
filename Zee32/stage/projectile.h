#pragma once
#include "frame_image.h"
#include "unit.h"

namespace zee {
	//Ư¡: die ���·� ���ӽ���, � obj�κ��� �����Ǹ� ��Ƴ�.
	class projectile : public unit {
	public:
		projectile() noexcept = default;
		virtual ~projectile() noexcept = default;

		virtual void init() override;
		//spawn_from(other): effect, item ���� Ǯ��
		virtual void spawn_from(const std::shared_ptr<unit> other);
		//spawn_from(obj_type, obj_body): bullet ���� Ǯ��
		virtual void spawn_from(const int obj_type, const shape::circlef& obj_body);

		virtual void render(win32gdi::device_context_dynamic& dest_dc) override;
	};
}