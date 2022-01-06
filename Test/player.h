#pragma once
//자 상속은 답이 없어요
//무조건 포함시켜야해요
#include "unit.h"
//초보들이 주로 하는 실수.
//찾기 존나 오래걸림
class player : public unit {
public:
	player();
	//
	int get_a();
	void add_score(int new_score) {
		my_score_ += new_score;
	}

protected:
	virtual void on_dead() override;

private:
	int my_score_ = 0;
};