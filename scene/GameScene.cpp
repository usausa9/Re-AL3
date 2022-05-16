#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	// 3Dモデルの生成
	model_ = Model::Create();


	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth,WinApp::kWindowHeight);

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	// 軸方向表示が参照するビュープロジェクションを指定する (アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	// ライン描画が参照するビュープロジェクションを指定する (アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

#pragma region スケーリング
	
	// X, Y, Z 方向のスケーリングを設定
	worldTransform_.scale_ = { 5.0f,1.0f,1.0f };

	// スケーリング行列を宣言
	Matrix4 matScale;

	// スケーリング倍率を行列に設定する ①
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;
	matScale.m[3][3] = 1.0f;

	// 単位行列を代入 ②
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	// 掛け算をして代入
	worldTransform_.matWorld_ *= matScale;

	// 行列の転送
	worldTransform_.TransferMatrix();
	
#pragma endregion 

#pragma region 回転

	//// X, Y, Z 軸周りの回転角を設定
	//worldTransform_.rotation_ = { 0.0f, 0.0f, 0.785398f };

	//// Z軸回転行列を宣言
	//Matrix4 matRotZ;

	//// 回転行列の各要素を設定する
	//matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	//matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	//matRotZ.m[0][2] = 0.0f;
	//matRotZ.m[0][3] = 0.0f;

	//matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	//matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
	//matRotZ.m[1][2] = 0.0f;
	//matRotZ.m[1][3] = 0.0f;

	//matRotZ.m[2][0] = 0.0f;
	//matRotZ.m[2][1] = 0.0f;
	//matRotZ.m[2][2] = 1.0f;
	//matRotZ.m[2][3] = 0.0f;

	//matRotZ.m[3][0] = 0.0f;
	//matRotZ.m[3][1] = 0.0f;
	//matRotZ.m[3][2] = 0.0f;
	//matRotZ.m[3][3] = 1.0f;

	//// 単位行列を代入 ②
	//worldTransform_.matWorld_.IdentityMatrix();
	//// 掛け算をして代入
	//worldTransform_.matWorld_ *= matRotZ;

	//// 行列の転送
	//worldTransform_.TransferMatrix();

	//// X, Y, Z 軸周りの回転角を設定
	//worldTransform_.rotation_ = { 0.785398f , 0.0f, 0.0f};

	//// Z軸回転行列を宣言
	//Matrix4 matRotX;

	//// 回転行列の各要素を設定する
	//matRotX.m[0][0] = 1.0f;
	//matRotX.m[0][1] = 0.0f;
	//matRotX.m[0][2] = 0.0f;
	//matRotX.m[0][3] = 0.0f;

	//matRotX.m[1][0] = 0.0f;
	//matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	//matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	//matRotX.m[1][3] = 0.0f;

	//matRotX.m[2][0] = 0.0f;
	//matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	//matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
	//matRotX.m[2][3] = 0.0f;

	//matRotX.m[3][0] = 0.0f;
	//matRotX.m[3][1] = 0.0f;
	//matRotX.m[3][2] = 0.0f;
	//matRotX.m[3][3] = 1.0f;

	//// 単位行列を代入 ②
	//worldTransform_.matWorld_.IdentityMatrix();
	//// 掛け算をして代入
	//worldTransform_.matWorld_ *= matRotX;

	//// 行列の転送
	//worldTransform_.TransferMatrix();

	//// X, Y, Z 軸周りの回転角を設定
	//worldTransform_.rotation_ = { 0.0f , 0.785398f, 0.0f };

	//// Z軸回転行列を宣言
	//Matrix4 matRotY;

	//// 回転行列の各要素を設定する
	//matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	//matRotY.m[0][1] = 0.0f;
	//matRotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	//matRotY.m[0][3] = 0.0f;

	//matRotY.m[1][0] = 0.0f;
	//matRotY.m[1][1] = 1.0f;
	//matRotY.m[1][2] = 0.0f;
	//matRotY.m[1][3] = 0.0f;

	//matRotY.m[2][0] = sin(worldTransform_.rotation_.y);
	//matRotY.m[2][1] = 0.0f;
	//matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
	//matRotY.m[2][3] = 0.0f;

	//matRotY.m[3][0] = 0.0f;
	//matRotY.m[3][1] = 0.0f;
	//matRotY.m[3][2] = 0.0f;
	//matRotY.m[3][3] = 1.0f;

	//// 単位行列を代入 ②
	//worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	//// 掛け算をして代入
	//worldTransform_.matWorld_ *= matRotY;

	//// 行列の転送
	//worldTransform_.TransferMatrix();

	// X, Y, Z 軸周りの回転角を設定
	worldTransform_.rotation_ = { 0.785398f, 0.785398f, 0.785398f };
	
	// 合成用回転行列を宣言
	Matrix4 matRot;

	// Z軸回転行列を宣言
	Matrix4 matRotZ;
	// X軸回転行列を宣言
	Matrix4 matRotX;
	// Y軸回転行列を宣言
	Matrix4 matRotY;

	// 回転行列の各要素を設定する
	matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	matRotZ.m[0][2] = 0.0f;
	matRotZ.m[0][3] = 0.0f;
	
	matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
	matRotZ.m[1][2] = 0.0f;
	matRotZ.m[1][3] = 0.0f;
	
	matRotZ.m[2][0] = 0.0f;
	matRotZ.m[2][1] = 0.0f;
	matRotZ.m[2][2] = 1.0f;
	matRotZ.m[2][3] = 0.0f;
	
	matRotZ.m[3][0] = 0.0f;
	matRotZ.m[3][1] = 0.0f;
	matRotZ.m[3][2] = 0.0f;
	matRotZ.m[3][3] = 1.0f;

	// 回転行列の各要素を設定する
	matRotX.m[0][0] = 1.0f;
	matRotX.m[0][1] = 0.0f;
	matRotX.m[0][2] = 0.0f;
	matRotX.m[0][3] = 0.0f;

	matRotX.m[1][0] = 0.0f;
	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	matRotX.m[1][3] = 0.0f;

	matRotX.m[2][0] = 0.0f;
	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
	matRotX.m[2][3] = 0.0f;

	matRotX.m[3][0] = 0.0f;
	matRotX.m[3][1] = 0.0f;
	matRotX.m[3][2] = 0.0f;
	matRotX.m[3][3] = 1.0f;

	// 回転行列の各要素を設定する
	matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	matRotY.m[0][1] = 0.0f;
	matRotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	matRotY.m[0][3] = 0.0f;

	matRotY.m[1][0] = 0.0f;
	matRotY.m[1][1] = 1.0f;
	matRotY.m[1][2] = 0.0f;
	matRotY.m[1][3] = 0.0f;

	matRotY.m[2][0] = sin(worldTransform_.rotation_.y);
	matRotY.m[2][1] = 0.0f;
	matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
	matRotY.m[2][3] = 0.0f;

	matRotY.m[3][0] = 0.0f;
	matRotY.m[3][1] = 0.0f;
	matRotY.m[3][2] = 0.0f;
	matRotY.m[3][3] = 1.0f;

	matRotZ *= matRotX;
	matRotZ *= matRotY;
	matRot = matRotZ;
	// 単位行列を代入 ②
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	// 掛け算をして代入
	worldTransform_.matWorld_ *= matRot;
	
	// 行列の転送
	worldTransform_.TransferMatrix();
#pragma endregion 

#pragma region 平行移動

	// X,Y,Z軸周りの平行移動を設定
	worldTransform_.translation_ = { 0,10,0 };
	
	//// 平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;

	// 単位行列を代入 
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	// 掛け算をして代入
	worldTransform_.matWorld_ *= matTrans;

	// 行列の転送
	worldTransform_.TransferMatrix();

#pragma endregion
}

void GameScene::Update() {

	debugCamera_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

	
#pragma endregion

	// ライン描画が参照するビュープロジェクションを指定する (アドレス渡し)

	for (float i = 0; i < 21; i++)
	{
		PrimitiveDrawer::GetInstance()->DrawLine3d({ -20, -20 + (i * 2), 0 }, {  20, -20 + (i * 2), 0 }, { 1,0,0,1 });
		PrimitiveDrawer::GetInstance()->DrawLine3d({ -20 + (i * 2), -20, 0 }, { -20 + (i * 2), 20, 0 }, { 0,1,0,1 });
		PrimitiveDrawer::GetInstance()->DrawLine3d({ -20 + (i * 2), 0, -20 }, { -20 + (i * 2) ,0 ,20, }, { 0,0,1,1 });
	}
}
