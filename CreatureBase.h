#pragma once
class CreatureBase {
public:
	CreatureBase(){ NowHp = MaxHp; }
	virtual ~CreatureBase(){}

	//Play画面で回すUpdate
	virtual void Update(){}
protected:
	//Meshの大きさ
	float mesh_x = -250;
	float mesh_z = 250;

	//HP関係
	int MaxHp = 10;
	int NowHp;

	//Position関係
	int PosX = 0;
	int PosZ = 0;

	//自分が死んでいるかどうかを判定するフラグ　
	bool DeathFlag = false;
	
	virtual void Attack(){}
	virtual void Move(){}
	virtual void Dead(){}

	//位置をセットする
	void SetPos(const int x, const int y);

	//MeshをSet
	virtual void SetMesh(int x, int z){}
};