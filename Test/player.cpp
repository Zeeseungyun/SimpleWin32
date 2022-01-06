#include "player.h"

player::player()
{
	a = 10;
}

int player::get_a() {
	return a;
}

void player::on_dead() {
	//todo:: dead..
	unit::on_dead();
}
