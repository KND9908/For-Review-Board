#include "Enemy.h"

void Enemy::Attack() {

}

void Enemy::Move() {
	//Aster‚ÅƒvƒŒƒCƒ„[‚ÌŒ³‚Ö
}

void Enemy::Dead() {

}

void Enemy :: Update() {

}

void Enemy::SetMesh(int x, int z) {
	EnemyMesh = dxe::Mesh::CreateBoxMV(Size);
	EnemyMesh->setTexture(dxe::Texture::CreateFromFile("graphics/red_box.bmp"));
	SetPos(mesh_x + x * Size, mesh_z - z * Size);
	EnemyMesh->pos_ = { (float)PosX, 0, (float)PosZ };
}