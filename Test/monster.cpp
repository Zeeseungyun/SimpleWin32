#include "monster.h"
#include "player.h"
monster::monster() : my_score_(0)
{
	a = 20;
}
int monster::get_a() {
	return a;
}

void monster::hit_from(unit& other) {
	//�ٵ��̰� &�� �ؼ� dynamic_cast�� �����ϰԵǸ� ���ܴ��������� �Ƹ�
	//�׷��� ��κ� ���̳��� ĳ������ �����ͷ� �ؿ�
	//nullptr�� üũ�Ҷ��

	if (player* other_player = dynamic_cast<player*>(&other)) {//�̷�������~
		//���۷����� ���̳��� ĳ������ ��ϴ� ����? ����
		other_player->add_score(my_score_);
		//�� �̷������� ���ھ �ټ��ְ���? �� ���͸��� �����ֵ� �̷��������ϱ�
		//�׷��� �ƿ� ���� ������� �÷��̾� ��ü�� �Ⱥ��̰� �ϴ°� �� ���� �Դϴ�
		//�ٵ� �ʿ��ҋ��� �ֱ��־
		//�ƹ��� ������ �������� ��Ȳ�� ����ܸ�����
	}
}

void monster::func(class player& ohter_player) {
	ohter_player.add_score(10);
}

void monster::func2(player& ohter_player) {
}