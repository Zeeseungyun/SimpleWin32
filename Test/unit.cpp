#include "unit.h"

unit::unit()
	: hp_(0)
	, atk_(0)
{
}

void unit::init(int new_hp, int new_atk) {
	hp_ = new_hp;
	atk_ = new_atk;
}

void unit::hit_from(unit& other) {
	hp_ -= other.atk();
	if (hp_ <= 0) {

	}
}

void unit::on_dead() {
	//todo:: dead..
}
