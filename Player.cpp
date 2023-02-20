#include "Player.h"

void Player::Attack() {

}

void Player::Move() {

}

void Player::Dead() {

}

void Player::Update() {

}

void Player::SetMesh(int x, int z) {
	PlayerMesh = dxe::Mesh::CreateBoxMV(Size);
	PlayerMesh->setTexture(dxe::Texture::CreateFromFile("graphics/box.bmp"));
	SetPos(mesh_x + x * Size, mesh_z - z * Size);
	PlayerMesh->pos_ = { (float)PosX, 0, (float)PosZ };
}