#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <cassert>
#include <random>

#define XM_PI 3.141592

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

	// 乱数シード生成器
	std::random_device seed_gen;
	// メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	// 乱数範囲の指定
	std::uniform_real_distribution<float> dist(0,640);
	// 乱数エンジンを渡し、指定範囲からランダムな数値を得る
	float value = dist(engine);

#pragma region ビュー変換行列
	// カメラ視点座標を設定
	viewProjection_.eye = { 0,10,-20 };

	// カメラ注視点座標を設定
	viewProjection_.target = worldTransforms_[PartId::kRoot].translation_;

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
#pragma endregion

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth,WinApp::kWindowHeight);

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	// 軸方向表示が参照するビュープロジェクションを指定する (アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//&debugCamera_->GetViewProjection()

	// ライン描画が参照するビュープロジェクションを指定する (アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	worldTransforms_[PartId::kRoot].Initialize();

	worldTransforms_[PartId::kHead].Initialize();
	worldTransforms_[PartId::kHead].translation_ = { 0, 0, 2.0f };
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kRoot];

	worldTransforms_[PartId::kArmL].Initialize();
	worldTransforms_[PartId::kArmL].translation_ = { -2.0f, 0, 0 };
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kRoot];

	worldTransforms_[PartId::kArmR].Initialize();
	worldTransforms_[PartId::kArmR].translation_ = { 2.0f, 0, 0 };
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kRoot];

	worldTransforms_[object2].Initialize();
	worldTransforms_[object2].translation_ = { -20.0f, 0, 20.0f };
	
	worldTransforms_[object3].Initialize();
	worldTransforms_[object3].translation_ = { 0, 0, 20.0f };

	worldTransforms_[object4].Initialize();
	worldTransforms_[object4].translation_ = { 20.0f, 0, 20.0f };
}

void GameScene::Update() {

	// ワールドトランスフォーム
	if (input_->TriggerKey(DIK_Q) == true)
{
	if (moveMode == 0)
	{
		moveMode = 1;
	}
	else { moveMode = 0; }
}

if (moveMode == 0)
{
	const float kMoveSpeed = 0.5f;

	// 正面ベクトルの生成
	Vector3 frontVec;
	Vector3 target = viewProjection_.target;

	// 正面ベクトルの計算
	frontVec = target -= viewProjection_.eye;
	
	// 正面ベクトルの正規化
	MathUtility::Vector3Normalize(frontVec);	

	if (input_->PushKey(DIK_W))
	{
		worldTransforms_[PartId::kRoot].translation_.x += frontVec.x * kMoveSpeed;
		worldTransforms_[PartId::kRoot].translation_.z += frontVec.z * kMoveSpeed;
	}
	if (input_->PushKey(DIK_S))
	{
		worldTransforms_[PartId::kRoot].translation_.x += -frontVec.x * kMoveSpeed;
		worldTransforms_[PartId::kRoot].translation_.z += -frontVec.z * kMoveSpeed;
	}

	// 右ベクトル・仮ベクトルの宣言
	Vector3 rightVec;
	Vector3 tentativeVec = { 0,1,0 };

	// 右ベクトルを作成
	rightVec = MathUtility::Vector3Cross(tentativeVec, frontVec);

	// 右ベクトルの正規化
	MathUtility::Vector3Normalize(tentativeVec);

	if (input_->PushKey(DIK_D))
	{
		worldTransforms_[PartId::kRoot].translation_.x += rightVec.x * kMoveSpeed;
		worldTransforms_[PartId::kRoot].translation_.z += rightVec.z * kMoveSpeed;
	}
	if (input_->PushKey(DIK_A))
	{
		worldTransforms_[PartId::kRoot].translation_.x += -rightVec.x * kMoveSpeed;
		worldTransforms_[PartId::kRoot].translation_.z += -rightVec.z * kMoveSpeed;
	}
}

if (moveMode == 1)
{
	XMFloat3 move = { 0.0f, 0.0f, 0.0f };

	// 回転速度
	const float kRotSpeed = 0.05f;

	if (input_->PushKey(DIK_A))
	{
		move = { 0, -kRotSpeed, 0 };
	}
	else if (input_->PushKey(DIK_D))
	{
		move = { 0, kRotSpeed, 0 };
	}

	worldTransforms_[PartId::kRoot].rotation_.y += move.y;

	XMFloat3 frontVec = { 0.0f, 0.0f, -1.0f };	// 単位ベクトル(逆)
	XMFloat3 resultVec = { 0.0f, 0.0f, 0.0f };	// 結果補間用ベクトル

	resultVec.x = (cos(worldTransforms_[PartId::kRoot].rotation_.y) * frontVec.x +
		sin(worldTransforms_[PartId::kRoot].rotation_.y) * frontVec.z);
	resultVec.z = (-sinf(worldTransforms_[PartId::kRoot].rotation_.y) * frontVec.x +
		cosf(worldTransforms_[PartId::kRoot].rotation_.y) * frontVec.z);
}

	// デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"kRoot:(%f,%f,%f)", worldTransforms_[PartId::kRoot].translation_.x,
		worldTransforms_[PartId::kRoot].translation_.y,
		worldTransforms_[PartId::kRoot].translation_.z,
		moveMode);

	debugText_->SetPos(50, 70);
	debugText_->Printf(
		"moveMode(Change:Q):%d", moveMode);
	

	for (int i = 0; i < kNumPartId; i++)
	{
		matrix.UpdateMatrix(worldTransforms_[i]);
	}

	// ビュープロジェクション
	const float kCameraSpeed = 0.1f;

	if (input_->PushKey(DIK_UP))
	{
		viewProjection_.eye.z += kCameraSpeed;
		viewProjection_.target.z += kCameraSpeed;
	}
	if (input_->PushKey(DIK_DOWN))
	{
		viewProjection_.eye.z -= kCameraSpeed;
		viewProjection_.target.z -= kCameraSpeed;
	}
	if (input_->PushKey(DIK_RIGHT))
	{
		viewProjection_.eye.x += kCameraSpeed;
		viewProjection_.target.x += kCameraSpeed;
	}
	if (input_->PushKey(DIK_LEFT))
	{
		viewProjection_.eye.x -= kCameraSpeed;
		viewProjection_.target.x -= kCameraSpeed;
	}

	viewProjection_.UpdateMatrix();

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
	
	for (int i = 0; i < kNumPartId; i++)
	{
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}

	//debugCamera_->GetViewProjection()

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

	//for (float i = 0; i < 21; i++)
	//{
	//	PrimitiveDrawer::GetInstance()->DrawLine3d({ -20, -20 + (i * 2), 0 }, {  20, -20 + (i * 2), 0 }, { 1,0,0,1 });
	//	PrimitiveDrawer::GetInstance()->DrawLine3d({ -20 + (i * 2), -20, 0 }, { -20 + (i * 2), 20, 0 }, { 0,1,0,1 });
	//	PrimitiveDrawer::GetInstance()->DrawLine3d({ -20 + (i * 2), 0, -20 }, { -20 + (i * 2) ,0 ,20, }, { 0,0,1,1 });
	//}
}
