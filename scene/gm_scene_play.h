#include "gm_scene_base.h"
#include "../Enemy.h"
#include "../Player.h"

//このゲーム内で表示する最大の敵の数
const int MaxEnemyNum = 5;
/// <summary>
/// ゲームのインゲームシーンのマネージャーをするクラス
/// 基本的な構造は先生が開発したもので、一部機能に追加および修正を実施
/// </summary>
class GmCamera;
class ScenePlay : public SceneBase {
public :
	ScenePlay(){}
	~ScenePlay();

	GmCamera* camera_ = nullptr;

	float jump_vel_ = 0;
	dxe::Mesh* ctrl_box_ = nullptr;

	std::list<dxe::Mesh*> field_boxs_;

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

private:
	//各壁オブジェクトのサイズ
	tnl::Vector3 size1 = { 40, 40, 40 };
	tnl::Vector3 size2 = { 50, 50, 50 };

	//キー入力を停止させる変数
	bool KeyLock = false;
	//フィールドに配置する敵の数
	int SetEnemyNum = 3;

	int Nowlevel = 1;//現在の階層
	tnl::Vector3 StairPos = {0,0,0};//階段の位置
	tnl::Vector3 StairSize = { 50, 50, 50 };

	//ゲーム内で表示するオブジェクトの宣言
	Player* player;
	Enemy* enemy[MaxEnemyNum];

	//フィールドに配置する壊せるブロックの総数
	int SetBlock = 25;
	void KeyControl(tnl::Vector3 &move);
	void CreateMap();
	void GameOver();
	void StageClear();
};

