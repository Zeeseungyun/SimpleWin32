#pragma once
//�� ����� ���� �����
//������ ���Խ��Ѿ��ؿ�
#include "unit.h"
//�ʺ����� �ַ� �ϴ� �Ǽ�.
//ã�� ���� �����ɸ�
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