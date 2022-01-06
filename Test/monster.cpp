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
	//근데이거 &로 해서 dynamic_cast를 실패하게되면 예외던질꺼에여 아마
	//그래서 대부분 다이나믹 캐스팅은 포인터로 해여
	//nullptr로 체크할라고

	if (player* other_player = dynamic_cast<player*>(&other)) {//이런식으로~
		//레퍼런스도 다이나믹 캐스팅이 댐니다 ㅇㅋ? ㅇㅎ
		other_player->add_score(my_score_);
		//뭐 이런식으로 스코어를 줄수있겠쥬? 네 몬스터마다 점수있따 이럴수있으니까
		//그럴떄 아예 몬스터 헤더에선 플레이어 자체를 안보이게 하는게 더 좋긴 함니다
		//근데 필요할떄가 있긴있어여
		//아무리 뗄래야 뗄수없는 상황이 생긴단말이져
	}
}

void monster::func(class player& ohter_player) {
	ohter_player.add_score(10);
}

void monster::func2(player& ohter_player) {
}