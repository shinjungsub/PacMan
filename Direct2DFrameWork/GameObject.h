#pragma once
/*****************************************************************
## GameObject ##
���� GameObject�� �޶��� �� :
RECT _rc -> _mainRect
+ _callbackList;�߰�
+ _reserveMessageList;�߰�
*****************************************************************/
class GameObject : public GameNode
{
protected:
	//unordered_map�� �ؽ� ���̺�� �̷���� �ִ� map���μ� �ؽ��Լ��� ���� Ű���� �迭�� 
	//�ε����� �ٲٴ� ����� ����ϹǷ� �ſ� ���� Ž���ӵ��� ������.�ֻ��� ��� �ð����⵵ ����� ���� 
	//��� ����� ������ map�� �����ϴ�. �̷��� �ִ� ������ �˾Ƶ���

	//first���� �����ų �Լ��� Ű��, second���� �����ų �Լ��� ��Ƶд�
	//function<void(struct TagMessage)>�� �Լ��� �������ִ� ���� �����μ� �ʾȿ� ����ִ� �ڷ����� ������ ���� �ǹ̸� ������
	//��ȯ���� void�̰� ���ڷδ� struct TagMessage�� �޴� �Լ� 

	//������ ���� �ʿ� �Լ��� ��Ƶδ� ������ �� ��Ȱ�� ��ü���� ����� ���� ������ �޼������ ����� �����ϱ� �����̴�. 
	//���� ��� � ��ü���� GameObject�� ��ӹ��� Enemy��� ��ü�� Attack�̶�� �Լ��� ȣ���ϰ� ���� �� �츮�� 
	//Enemy* enemy = (Enemy*)ObjectManager->FindObject("sdas); enemy->Attack();
	//�� ���� �Լ��� ����ȯ�� �� �Ŀ� ���������� ȣ�����־�� �߾���
	//�� �ڵ��� �������� ã�ƿ� GameObject*�� EnemyŬ������ �ƴ� ���� �ڵ��� ���������� ��ƴٴ� ���̴�. 

	//�޼��� ����� ������ EnemyŬ���������� �̸� �ڽ��� callbackList�� "Attack"�̶�� Ű���� �Բ� Attack()�Լ��� ��Ƶд�. 
	//�ܺ��� ��ü������ EnemyŬ�������� �����ߴٰ� �˷��ְ� ���� �� enemy->SendMessage(TagMessage("Attack")); �� ���� 
	//�Լ��� ȣ�����ָ� �ȴ�. �̷��� �Ǹ� �÷��̾� Ŭ���������� EnemyŬ������ ����� �� �ʿ䰡 �������� Enemy*�� �ٿ� ĳ������ �� �ʿ䵵
	//��������.
	typedef unordered_map < string, function<void(struct TagMessage)>> CallbackHashmap;
	typedef unordered_map<string, function<void(struct TagMessage)>>::iterator CallbackHashmapIter;

protected:
	Image* _image;						//�̹���
	string _name;						//�̸�
	Pivot::Enum _pivot;					//�Ǻ�
	Direction::Enum _direction;			//�̵� ����
	Direction::Enum _intendedDirection;	//������ ����
	POINT _position;					//��ǥ
	POINT _mainSize;					//���η�Ʈ������
	RECT _mainRect;						//���� ��Ʈ
	bool _isChangeDirection;			//���� ���氡�� ����
	bool _isActive;						//Ȱ�� ����
	bool _isStart;
private:	//�Ʒ��� �������� �ڽ� ��ü���� �Ժη� �ǵ�� �ȵǱ� ������ private�� ���´�. 
	bool _isLive;			//��ü ���� ����(�ش� �Ұ��� ���� ObjectManager���� ��ü�� �����Ѵ�) 

	CallbackHashmap _callbackList;	//�޼��� ��ſ� ����� ��(�̰��� �̸� Ű���� �Բ� �Լ��� ��Ƶд�) 
	//���� �޼��� ����Ʈ 
	//���� ���� �޼����� delayTime�� �����Ǿ� ������ �ٷ� �Լ��� �����Ű�� �ʰ� �̰����� ������. 
	//GameObject�� ������Ʈ������ �ش� �޼����� ������ Ÿ���� ����ϴٰ� �ش� �Լ��� ���� �����ش�. 
	vector<struct TagMessage> _reserveMessageList;
public:
	//������ �����ε� 
	//��Ȳ�� �°� �ڽİ�ü���� ���ϴ� �����ڸ� ����ϸ� �ȴ�. 
	GameObject();
	GameObject(string name, POINT pos, POINT mainSize, Pivot::Enum pivot);
	GameObject(const RECT rect);
	virtual ~GameObject();
	//���� �����Լ��� �������� ���� ������ GameObjectŬ������ Release,Update������ ����Ǿ�� �� ������� �ֱ� �����̴�.
	void Init() override;
	void Release() override;
	void Update() override;
	void Render() override;
	//Ȱ��ȭ�� �� ����Ǵ� �Լ� (���ӿ�����Ʈ�� ��ӹ��� ��ü���� ���� Ȱ��ȭ �Ǵ� ������ �����ؾ� �� ������ �ִٸ� �ش� �Լ��� 
	//�������̵��ؼ� ������ �ۼ��ϸ� �ȴ�. 
	virtual void Enable() {}
	//��Ȱ��ȭ�� �� ����Ǵ� �Լ�
	virtual void Disable() {}
	//�ش� ��ü�� �޼����� ������.
	void SendCallbackMessage(const struct TagMessage message);
	//�ش� ��ü�� �̸� �޼����� �޾��� �� ������ �Լ��� ���
	void AddCallbackMessage(const string name, const function<void(struct TagMessage)> func);
	//���η�Ʈ�� �Ű������� ���� ��Ʈ�� �浹 �� �о�� �Լ�
	bool MainRectCollision(RECT rc);

	virtual RECT GetCollisionRect()const { return this->_mainRect; }
public:
	//�����ڸ� �����ڵ�
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
	//�Ǻ��� ���� ��Ʈ�� ������Ʈ �ȴ�
	void UpdateMainRect();
};