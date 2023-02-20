#pragma once
class CreatureBase {
public:
	CreatureBase(){ NowHp = MaxHp; }
	virtual ~CreatureBase(){}

	//Play��ʂŉ�Update
	virtual void Update(){}
protected:
	//Mesh�̑傫��
	float mesh_x = -250;
	float mesh_z = 250;

	//HP�֌W
	int MaxHp = 10;
	int NowHp;

	//Position�֌W
	int PosX = 0;
	int PosZ = 0;

	//����������ł��邩�ǂ����𔻒肷��t���O�@
	bool DeathFlag = false;
	
	virtual void Attack(){}
	virtual void Move(){}
	virtual void Dead(){}

	//�ʒu���Z�b�g����
	void SetPos(const int x, const int y);

	//Mesh��Set
	virtual void SetMesh(int x, int z){}
};