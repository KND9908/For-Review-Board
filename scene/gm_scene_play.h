#include "gm_scene_base.h"
#include "../Enemy.h"
#include "../Player.h"

//���̃Q�[�����ŕ\������ő�̓G�̐�
const int MaxEnemyNum = 5;
/// <summary>
/// �Q�[���̃C���Q�[���V�[���̃}�l�[�W���[������N���X
/// ��{�I�ȍ\���͐搶���J���������̂ŁA�ꕔ�@�\�ɒǉ�����яC�������{
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
	//�e�ǃI�u�W�F�N�g�̃T�C�Y
	tnl::Vector3 size1 = { 40, 40, 40 };
	tnl::Vector3 size2 = { 50, 50, 50 };

	//�L�[���͂��~������ϐ�
	bool KeyLock = false;
	//�t�B�[���h�ɔz�u����G�̐�
	int SetEnemyNum = 3;

	int Nowlevel = 1;//���݂̊K�w
	tnl::Vector3 StairPos = {0,0,0};//�K�i�̈ʒu
	tnl::Vector3 StairSize = { 50, 50, 50 };

	//�Q�[�����ŕ\������I�u�W�F�N�g�̐錾
	Player* player;
	Enemy* enemy[MaxEnemyNum];

	//�t�B�[���h�ɔz�u����󂹂�u���b�N�̑���
	int SetBlock = 25;
	void KeyControl(tnl::Vector3 &move);
	void CreateMap();
	void GameOver();
	void StageClear();
};

