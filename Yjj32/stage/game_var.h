#pragma once
namespace yjj {
	class game_var {
	public:
		// singleton
		static game_var& get() noexcept;

		game_var() = default;
		game_var(const game_var&) = delete;
		game_var& operator=(const game_var&) = delete;
		virtual ~game_var() = default;

		void add_bullet_monster(const int num);
		void add_bullet_player(const int num);
		void init_bullet_monster();
		void init_bullet_player();

		int bullet_monster_total() const;
		int bullet_player_total() const;

	private:

		int bullet_monster_total_ = 0;
		int bullet_player_total_ = 0;
	};
}