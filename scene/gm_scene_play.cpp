#include "../gm_manager.h"
#include "../gm_camera.h"
#include "../model/gm_anim_sprite3d.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"
#include <random>

//マップの大きさ（行と列）
const int MapHeight = 16;
const int MapWidth = 16;

//マップのランダム出力用変数
std::random_device rd;
std::mt19937 gen(rd());

ScenePlay::~ScenePlay() {
	delete camera_;
	delete ctrl_box_;
	for (auto box : field_boxs_) delete box;
}

void ScenePlay::initialzie() {

	camera_ = new GmCamera();
	camera_->pos_ = { 0, 150, -300 };

	//敵のインスタンス生成
	for (int i = 0; i < sizeof(enemy) / sizeof(enemy[0]); i++) {
		enemy[i] = new Enemy();
	}
	player = new Player();

	CreateMap();
}

void ScenePlay::update(float delta_time)
{
	GameManager* mgr = GameManager::GetInstance();

	tnl::Vector3 move = { 0, 0, 0 };

	KeyControl(move);

	//
	// 移動処理
	//
	move.normalize();
	move *= 3.0f;
	tnl::Vector3 prev_pos = ctrl_box_->pos_;
	ctrl_box_->pos_ += move;
	ctrl_box_->pos_.y += jump_vel_;
	if (ctrl_box_->pos_.y <= 0) {
		ctrl_box_->pos_.y = 0;
		jump_vel_ = 0;
	}
	else {
		jump_vel_ -= 0.98f;
	}

	// 
	// 衝突判定と補正
	//

	// 操作ボックスに近い順にソート
	field_boxs_.sort([&](const dxe::Mesh* l, const dxe::Mesh* r) {
		float ld = (ctrl_box_->pos_ - l->pos_).length();
	float rd = (ctrl_box_->pos_ - r->pos_).length();
	return ld < rd;
		});

	for (auto box : field_boxs_) {
		if (tnl::IsIntersectAABB(ctrl_box_->pos_, size1, box->pos_, size2)) {
			//オブジェクトとぶつかったときに先に進めないようにする
			int n = tnl::GetCorrectPositionIntersectAABB(prev_pos, size1, box->pos_, size2, ctrl_box_->pos_);
			// ボックスの上に補正されたらジャンプ力リセット
			//if (2 == n) jump_vel_ = 0;
		}
	}

	//敵とぶつかったら当たり判定をプレイヤーにコールする
	for (int i = 0; i < SetEnemyNum - 1; i ++ ) {
		if (tnl::IsIntersectAABB(ctrl_box_->pos_, size1, enemy[i]->EnemyMesh->pos_, size2)) {
			GameOver();
		}
	}

	//ゴールのポジションにたどり着いたらステージクリア
	if (tnl::IsIntersectAABB(ctrl_box_->pos_, size1, StairPos, StairSize)) {
		StageClear();
	}

	//プレイヤーの位置へカメラは合わせる
	camera_->target_ = ctrl_box_->pos_;

	//プレイヤーのライフが0になったらGameOverをコールする
}

void ScenePlay::render()
{
	camera_->update();

	ctrl_box_->render(camera_);

	for (auto box : field_boxs_) box->render(camera_);
	for (int i = 0; i < SetEnemyNum; i++ ) enemy[i]->EnemyMesh->render(camera_);

	DrawGridGround(camera_, 50, 20);
}

/// <summary>
/// キーボード操作
/// </summary>
void ScenePlay::KeyControl(tnl::Vector3& move) {
	//キー入力無効時は受け付けないようにする
	if (!KeyLock) {
		//
		// キーボード操作
		//
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) jump_vel_ = 10.0f;
		if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) move += camera_->left().xz();
		if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) move += camera_->right().xz();
		if (tnl::Input::IsKeyDown(eKeys::KB_UP)) move += camera_->front().xz();
		if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) move += camera_->back().xz();

		if (tnl::Input::IsKeyDown(eKeys::KB_A))	camera_->free_look_angle_xy_.y += tnl::ToRadian(1);
		if (tnl::Input::IsKeyDown(eKeys::KB_D)) camera_->free_look_angle_xy_.y -= tnl::ToRadian(1);
		if (tnl::Input::IsKeyDown(eKeys::KB_W))	camera_->free_look_angle_xy_.x += tnl::ToRadian(1);
		if (tnl::Input::IsKeyDown(eKeys::KB_S))	camera_->free_look_angle_xy_.x -= tnl::ToRadian(1);


		//
		// ジョイパッド操作
		//
		if (tnl::Input::IsPadDownTrigger(ePad::KEY_1)) jump_vel_ = 10.0f;
		tnl::Vector3 lstick = tnl::Input::GetLeftStick();
		if (lstick.length() > 0.1f) {
			move += camera_->front().xz() * -lstick.y;
			move += camera_->left().xz() * -lstick.x;
		}

		tnl::Vector3 rstick = tnl::Input::GetRightStick();
		if (rstick.length() > 0.1f) {
			camera_->free_look_angle_xy_.y += tnl::ToRadian(rstick.x);
			camera_->free_look_angle_xy_.x += tnl::ToRadian(rstick.y);
		}
	}
}


/// <summary>
/// マップの自動生成をする関数
/// </summary>
void ScenePlay::CreateMap() {
	// 使用するテクスチャを事前ロード
	Shared<dxe::Texture> texs[3];
	texs[0] = dxe::Texture::CreateFromFile("graphics/red_box.bmp");
	texs[1] = dxe::Texture::CreateFromFile("graphics/blue_box.bmp");
	texs[2] = dxe::Texture::CreateFromFile("graphics/purple_box.bmp");

	//Mapにオブジェクトを生成した場所を覚えさせる配列
	bool IsCreateMap[MapHeight][MapWidth] = { false };

	// ボックス３種類をあらかじめ作成
	dxe::Mesh* origin_boxs[3];
	for (int i = 0; i < 3; ++i) {
		origin_boxs[i] = dxe::Mesh::CreateBoxMV(50);
		origin_boxs[i]->setTexture(texs[i]);
	}
	//まずは周囲に壁を作る
	float sx = -250;
	float sz = 250;
	for (int i = 0; i < MapHeight; ++i) {
		for (int k = 0; k < MapWidth; ++k) {
			if (!IsCreateMap[i][k]) {
				// 横軸の壁
				// 横軸の壁
				if ((i == 0 || i == MapHeight - 1) || (k == 0 || k == MapWidth - 1)) {
					dxe::Mesh* mesh = origin_boxs[2]->createClone();	// ボックスをクローンして生成することで生成速度アップ
					IsCreateMap[i][k] = true;
					mesh->pos_ = { sx + k * 50, 0, sz - i * 50 };
					field_boxs_.emplace_back(mesh);
				}
			}
		}
	}
	//次にプレイヤーの位置を決める
	//ランダム位置の取得
	std::uniform_int_distribution<> heightseed(0, MapHeight - 1);
	int randomheight = heightseed(gen);
	std::uniform_int_distribution<> widthseed(0, MapHeight - 1);
	int randomwidth = widthseed(gen);

	while (IsCreateMap[randomheight][randomwidth]) {
		randomheight = heightseed(gen);
		randomwidth = widthseed(gen);
	}

	//ランダムの数値に合わせてプレイヤーの位置を設定し召喚
	player->SetMesh(randomheight, randomwidth);

	ctrl_box_ = dxe::Mesh::CreateBoxMV(40);
	ctrl_box_->setTexture(dxe::Texture::CreateFromFile("graphics/box.bmp"));
	ctrl_box_->pos_ = { sx + randomwidth * 50, 0, sz - randomheight * 50 };
	IsCreateMap[randomheight][randomwidth] = true;
	camera_->target_ = ctrl_box_->pos_;//プレイヤーの位置へカメラは合わせる

	//次に敵の位置を決める
	for (int i = 0; i < SetEnemyNum; i++) {
		while (IsCreateMap[randomheight][randomwidth]) {
			randomheight = heightseed(gen);
			randomwidth = widthseed(gen);
		}
		//ランダムの数値に合わせて敵の位置を設定し召喚
		enemy[i]->SetMesh(randomwidth, randomheight);
		IsCreateMap[randomheight][randomwidth] = true;
		field_boxs_.emplace_back(enemy[i]->EnemyMesh);
	}

	//次に指定セル内に一定個数の壊せる壁を作る
	for (int i = 0; i < SetBlock; i++) {
		while (IsCreateMap[randomheight][randomwidth]) {
			randomheight = heightseed(gen);
			randomwidth = widthseed(gen);
		}
		//ランダムの数値に合わせて岩の位置を設定し召喚
		dxe::Mesh* mesh = origin_boxs[2]->createClone();
		IsCreateMap[randomheight][randomwidth] = true;
		mesh->pos_ = { sx + randomwidth * 50, 0, sz - randomheight * 50 };
		IsCreateMap[randomheight][randomwidth] = true;
		field_boxs_.emplace_back(mesh);
	}

	//階段の位置を決定
}

void ScenePlay::GameOver() {
	//ゲームオーバー画面へ遷移

	//Rキーを押下したらリスタート
}

void ScenePlay::StageClear() {
	//次の階層へ移動
	Nowlevel++;
}