#pragma once
/*****************************************************************
## GameObject ##
기존 GameObject와 달라진 점 :
RECT _rc -> _mainRect
+ _callbackList;추가
+ _reserveMessageList;추가
*****************************************************************/
class GameObject : public GameNode
{
protected:
	//unordered_map은 해쉬 테이블로 이루어져 있는 map으로서 해싱함수를 통해 키값을 배열의 
	//인덱스로 바꾸는 방식을 사용하므로 매우 빠른 탐색속도를 가진다.최상의 경우 시간복잡도 상수를 가짐 
	//사용 방법은 기존의 map과 동일하다. 이런게 있다 정도로 알아두자

	//first에는 실행시킬 함수의 키값, second에는 실행시킬 함수를 담아둔다
	//function<void(struct TagMessage)>는 함수를 담을수있는 변수 형으로서 맵안에 담겨있는 자료형은 다음과 같은 의미를 가진다
	//반환형이 void이고 인자로는 struct TagMessage를 받는 함수 

	//다음과 같이 맵에 함수를 담아두는 이유는 더 원활한 객체간의 통신을 위해 간단한 메세지통신 기법을 구현하기 위함이다. 
	//예를 들어 어떤 객체에서 GameObject를 상속받은 Enemy라는 객체의 Attack이라는 함수를 호출하고 싶을 때 우리는 
	//Enemy* enemy = (Enemy*)ObjectManager->FindObject("sdas); enemy->Attack();
	//과 같은 함수를 형변환을 한 후에 직접적으로 호출해주어야 했었다
	//이 코드의 문제점은 찾아온 GameObject*가 Enemy클래스가 아닐 경우와 코드의 유지보수가 어렵다는 점이다. 

	//메세지 통신의 사용법은 Enemy클래스에서는 미리 자신의 callbackList에 "Attack"이라는 키값과 함께 Attack()함수를 담아둔다. 
	//외부의 객체에서는 Enemy클래스에게 공격했다고 알려주고 싶을 때 enemy->SendMessage(TagMessage("Attack")); 과 같이 
	//함수만 호출해주면 된다. 이렇게 되면 플레이어 클래스에서는 Enemy클래스의 헤더를 달 필요가 없어지고 Enemy*로 다운 캐스팅을 할 필요도
	//없어진다.
	typedef unordered_map < string, function<void(struct TagMessage)>> CallbackHashmap;
	typedef unordered_map<string, function<void(struct TagMessage)>>::iterator CallbackHashmapIter;

protected:
	Image* _image;						//이미지
	string _name;						//이름
	Pivot::Enum _pivot;					//피봇
	Direction::Enum _direction;			//이동 방향
	Direction::Enum _intendedDirection;	//예정된 방향
	POINT _position;					//좌표
	POINT _mainSize;					//메인렉트사이즈
	RECT _mainRect;						//메인 렉트
	bool _isChangeDirection;			//방향 변경가능 여부
	bool _isActive;						//활성 여부
	bool _isStart;
private:	//아래의 변수들은 자식 객체에서 함부로 건들면 안되기 때문에 private로 엮는다. 
	bool _isLive;			//객체 생존 여부(해당 불값을 끄면 ObjectManager에서 객체를 삭제한다) 

	CallbackHashmap _callbackList;	//메세지 통신에 사용할 맵(이곳에 미리 키값과 함께 함수를 담아둔다) 
	//예약 메세지 리스트 
	//만약 보낸 메세지의 delayTime이 설정되어 있으면 바로 함수를 실행시키지 않고 이곳으로 보낸다. 
	//GameObject의 업데이트에서는 해당 메세지의 딜레이 타임을 계산하다가 해당 함수를 실행 시켜준다. 
	vector<struct TagMessage> _reserveMessageList;
public:
	//생성자 오버로딩 
	//상황에 맞게 자식객체들은 원하는 생성자를 사용하면 된다. 
	GameObject();
	GameObject(string name, POINT pos, POINT mainSize, Pivot::Enum pivot);
	GameObject(const RECT rect);
	virtual ~GameObject();
	//순수 가상함수로 지정하지 않은 이유는 GameObject클래스의 Release,Update에서도 실행되어야 할 내용들이 있기 때문이다.
	void Init() override;
	void Release() override;
	void Update() override;
	void Render() override;
	//활성화될 때 실행되는 함수 (게임오브젝트를 상속받은 객체에서 만약 활성화 되는 순간에 실행해야 될 내용이 있다면 해당 함수를 
	//오버라이딩해서 내용을 작성하면 된다. 
	virtual void Enable() {}
	//비활성화될 때 실행되는 함수
	virtual void Disable() {}
	//해당 객체에 메세지를 보낸다.
	void SendCallbackMessage(const struct TagMessage message);
	//해당 객체에 미리 메세지를 받았을 때 실행할 함수를 등록
	void AddCallbackMessage(const string name, const function<void(struct TagMessage)> func);
	//메인렉트와 매개변수로 들어가는 렉트가 충돌 시 밀어내는 함수
	bool MainRectCollision(RECT rc);

	virtual RECT GetCollisionRect()const { return this->_mainRect; }
public:
	//접근자며 설정자들
	string GetName()const { return this->_name; }
	POINT GetPosition()const { return this->_position; }
	POINT GetMainSize()const { return this->_mainSize; }
	RECT GetMainRect()const { return this->_mainRect; }
	Direction::Enum GetDirection()const { return this->_direction; }
	Direction::Enum GetIntendedDirection()const { return this->_intendedDirection; }
	bool GetActive()const { return this->_isActive; }
	bool GetLive()const { return this->_isLive; }

	void SetName(const string name) { this->_name = name; }
	void SetPivot(const Pivot::Enum pivot);
	void SetIntendedDirection(const Direction::Enum intendedDirection) { _intendedDirection = intendedDirection; }
	void SetPosition(const POINT position);
	void SetMainSize(const POINT size);
	void SetIsChangeDirection(const bool isChangeDirection) { this->_isChangeDirection = isChangeDirection; }
	void SetActive(const bool b);
	void SetStart(const bool b) { _isStart = b; }
	void Destroy() { this->_isLive = false; }
	void SetMainRect(const RECT rect);
protected:
	//피봇에 따라서 렉트들 업데이트 된다
	void UpdateMainRect();
};