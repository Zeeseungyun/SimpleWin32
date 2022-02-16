#include "game_var.h"
#include <memory>

namespace yjj {

	game_var& game_var::get() noexcept {
		static std::unique_ptr<game_var> inst_;
		if (!inst_) {
			inst_ = std::unique_ptr<game_var>(new game_var);
		}
		return *inst_;
	}


	void game_var::add_bullet_monster(const int num) {
		bullet_monster_total_ += num;
	}
	void game_var::add_bullet_player(const int num) {
		bullet_player_total_ += num;
	}

	void game_var::init_bullet_monster() {
		bullet_monster_total_ = 0;
	}
	void game_var::init_bullet_player() {
		bullet_player_total_ = 0;
	}


	int game_var::bullet_monster_total() const {
		return bullet_monster_total_;
	}
	int game_var::bullet_player_total() const {
		return bullet_player_total_;
	}
}