#include "Matrix.h"
#include "GameScene.h"

Matrix::Matrix()
{
	for (int i = 0; i < 4; i++) 
	{
		matResult.Scale.m[i][i] = 1;
		matResult. Rot .m[i][i] = 1;
	}

	matResult.Trans = MathUtility::Matrix4Identity();
}

// 単位行列
void Matrix::InitMatrix(Matrix4& m) {

	Matrix4 initMatrix;
	initMatrix.m[0][0] = 1.0f;
	initMatrix.m[1][1] = 1.0f;
	initMatrix.m[2][2] = 1.0f;
	initMatrix.m[3][3] = 1.0f;

	m = initMatrix;
}

// スケーリング
void Matrix::ScaleChange(WorldTransform& worldTransform, float m1, float m2, float m3, float m4) {
	// スケーリング 設定
	worldTransform.scale_ = { m1,m2,m3 };

	// スケーリング行列宣言
	matResult.Scale.m[0][0] = worldTransform.scale_.x;
	matResult.Scale.m[1][1] = worldTransform.scale_.y;
	matResult.Scale.m[2][2] = worldTransform.scale_.z;
	matResult.Scale.m[3][3] = m4;
}

// 回転
void Matrix::RotaChange(WorldTransform& worldTransform, float m1, float m2, float m3) {
	// 回転角 設定
	worldTransform.rotation_ = { m1,m2,m3 };

	// 回転行列 宣言
	Matrix4 matRotX, matRotY, matRotZ;

	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(worldTransform.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform.rotation_.x);
	matRotX.m[3][3] = 1;

	matRotY.m[0][0] = cos(worldTransform.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(worldTransform.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform.rotation_.y);
	matRotY.m[3][3] = 1;

	matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform.rotation_.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	// 回転行列 合成
	matResult.Rot = matRotZ;
	matResult.Rot *= matRotX;
	matResult.Rot *= matRotY;
}

// 平行移動
void Matrix::ChangeTranslation(WorldTransform& worldTransform, float tx, float ty, float tz)
{
	// 平行移動 設定
	worldTransform.translation_ = { tx,ty,tz };

	matResult.Trans.m[3][0] = worldTransform.translation_.x;
	matResult.Trans.m[3][1] = worldTransform.translation_.y;
	matResult.Trans.m[3][2] = worldTransform.translation_.z;
}


void Matrix::ScaleChange(WorldTransform worldTransform)
{
	matResult.Scale.m[0][0] = worldTransform.scale_.x;
	matResult.Scale.m[1][1] = worldTransform.scale_.y;
	matResult.Scale.m[2][2] = worldTransform.scale_.z;
	matResult.Scale.m[3][3] = 1;
}
void Matrix::RotaChange(WorldTransform worldTransform)
{
	// 回転行列 宣言
	Matrix4 matRotX, matRotY, matRotZ;

	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(worldTransform.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform.rotation_.x);
	matRotX.m[3][3] = 1;

	matRotY.m[0][0] = cos(worldTransform.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(worldTransform.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform.rotation_.y);
	matRotY.m[3][3] = 1;

	matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform.rotation_.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	// 回転行列 合成
	matResult.Rot = matRotZ;
	matResult.Rot *= matRotX;
	matResult.Rot *= matRotY;
}

void Matrix::ChangeTranslation(WorldTransform worldTransform)
{
	matResult.Trans.m[3][0] = worldTransform.translation_.x;
	matResult.Trans.m[3][1] = worldTransform.translation_.y;
	matResult.Trans.m[3][2] = worldTransform.translation_.z;
}

//組み合わせ
void Matrix::UpdataMatrix(WorldTransform& worldTransform)
{
	// 行列 合成
	ScaleChange(worldTransform);
	RotaChange(worldTransform);
	ChangeTranslation(worldTransform);

	InitMatrix(worldTransform.matWorld_);

	worldTransform.matWorld_ = matResult.Scale;
	worldTransform.matWorld_ *= matResult.Rot;
	worldTransform.matWorld_ *= matResult.Trans;

	if (worldTransform.parent_) {
		worldTransform.matWorld_ *= worldTransform.parent_->matWorld_;
	}

	//行列の転送
	worldTransform.TransferMatrix();
}
