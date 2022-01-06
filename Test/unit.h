#pragma once
/*
* 예시 끝임.
* 1. 컴파일 성능 향상
* 2. 가독성 향상
* 3. 순환 참조 방지
* 등등.. 장점 많슴
* 언리얼 코딩 표준에서도 장려함.
* ㅇㅋ?ㅇㅇㅇ 이제 다음번 당신의 코드 리팩토링 주제가
* 뭔지 알겠쮸? 넵상속도 좀 써보지
* 총알도 유닛이고
* 몬스터도 유닛이고
* 플레이어도 유닛인데
* 너무 아쉬움
* 그런 부분은
* 객체지향적으로 뽐낸 흔적이 거의 없음.
* 상속활용이 없어서 아쉽슴
* 글쿠만
* C언어의 꽃이 뭐임? 포인터
* C++에서 클래스의 꽃은? 상속이 아님 , 그럼 뭘까? 캡슐화 것도 아님. 다형성임. virtual.
* 다른건 어떻게든 불편하지 않게 대체가 되는데 
* C++의 꽃은 뭘까?
* 아님 ㅋㅋ 템플릿 ㅇㅇ 맞슴
* ㅋㅋㅋ
* 젤 난이도가 높슴
* 모~~~~~든게
* 다 포함되어ㅣㅇㅆ음
* 클래스건 머건
* virtual은 도무지 불편하게 대체됨.
* C언어에서 보자면
* ㅇㅇ
* 저 버츄얼로 잘갖고 놀면 코드가 정말 세련되어짐.
* 잘짜는 사람들 보면 virtual 참 찰지게 잘 씀.
* 다형적으로 뭔갈 구조를 만들면 정말 이쁨. ㅇㅎ
*/
static int a;
class unit {
public:
	unit();
	void init(int new_hp, int new_atk);
	
	int hp() const { return hp_; }
	int atk() const { return atk_; }

	virtual void hit_from(unit& other);

protected:
	virtual void on_dead();

protected:
	int hp_;
	int atk_;
};
