#pragma once
#include "unit.h"
//�̷��� ���� �и� ���Խ��״µ� �𸥴ٰ� ����?��
//�����̰� ��������. �𸦸��� ���µ�
//�̷����� �׳� ������~ ���漱�� Ȥ�� ������ Ÿ���������� ī���ľߵǴ� ����.
//��ȯ ������ ����?����
class player;

class monster : public unit {
public:
	monster();
	//�� �̷��ٰ� ġ�� monster�� player�� �˾ƾ� �� ���� ���ݾƿ�?��
	// �ΰ��� ���̽���
	// ���� ��Ȳ������
	// unit���� �޾Ƽ� dynamic_cast�� �Ẹ����?
	// Ȥ�� ���漱���� �ϴ���.
	//�䷱������ ~ ����?
	virtual void hit_from(unit& other) override;
	void func2(player& ohter_player);
	void func(player& ohter_player);
	//�̷��� �Ź� ������ �����ݾƿ�? ��
	//�׳� ������ ���漱�� ����� �ۼ��ϴ°� �Ϲ�����.
	//Ȥ�� �� �ڵ� ��Ģ�� ���� �ٸ��� �Ź� ������ ������ Ÿ�������� �ϴ���

	int get_a();

private:
	int my_score_;
};

