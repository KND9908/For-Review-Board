#pragma once
#pragma once
#include "CreatureBase.h"
#include "../dxlib_ext/dxlib_ext_mesh.h"

class Player :public CreatureBase {
public:
	Player() {}
	Player(int posx, int posy) {}
	virtual ~Player() {}
	virtual void Update();

	dxe::Mesh* PlayerMesh = nullptr;

	virtual void SetMesh(int x, int z) override;
protected:
	const int Size = 40;

	virtual void Attack() override;
	virtual void Move() override;
	virtual void Dead() override;
};