#pragma once
#include "unit.h"
//이러면 굳이 분명 포함시켰는데 모른다고 하져?넨
//정신이가 나간거임. 모를리가 없는데
//이럴때도 그냥 무조건~ 전방선언 혹은 정교한 타입지정으로 카바쳐야되는 거임.
//순환 참조라 ㅇㅇ?ㅇㅎ
class player;

class monster : public unit {
public:
	monster();
	//머 이렇다고 치고 monster가 player를 알아야 할 때도 있잖아요?네
	// 두가지 케이스져
	// 지금 상황에서는
	// unit으로 받아서 dynamic_cast를 써보던가?
	// 혹은 전방선언을 하던가.
	//요런식으로 ~ ㅇㅋ?
	virtual void hit_from(unit& other) override;
	void func2(player& ohter_player);
	void func(player& ohter_player);
	//이렇게 매번 붙히기 귀찮잖아여? 네
	//그냥 위에다 전방선언 목록을 작성하는게 일반적임.
	//혹은 뭐 코딩 규칙에 따라 다른데 매번 저렇게 정교한 타입지정을 하던지

	int get_a();

private:
	int my_score_;
};

