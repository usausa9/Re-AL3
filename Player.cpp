#include "Player.h"
#include "input.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	// NULL�`�F�b�N
	assert(model);
	assert(textureHandle);

	// �t�@�C�������w�肵�ăe�N�X�`����ǂݍ���
	model_ = model;
	textureHandle_ = textureHandle;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();

}

void Player::Update()
{
	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };


}

void Player::Draw()
{

}