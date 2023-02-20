#pragma once
#include "CreatureBase.h"
#include "../dxlib_ext/dxlib_ext_mesh.h"

class Enemy :public CreatureBase {
public:
	Enemy(){}
	Enemy(int posx,int posy){}
	virtual ~Enemy(){}
	virtual void Update();

	dxe::Mesh* EnemyMesh = nullptr;

	virtual void SetMesh(int x, int z) override;
protected:
	const int Size = 50;

	virtual void Attack() override;
	virtual void Move() override;
	virtual void Dead() override;
};