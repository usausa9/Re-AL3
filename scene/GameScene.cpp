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

#pragma region ビュー変換行列
	// カメラ視点,注視点座標を設定
	viewProjection_.eye = { 0,10,-20 };
	viewProjection_.target = worldTransforms_[PartId::kCenter].translation_;

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
#pragma endregion

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	// 軸方向表示が参照するビュープロジェクションを指定する (アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	for (int i = 0; i < kNumPartId; i++) {
		worldTransforms_[i].Initialize();
	}

	worldTransforms_[PartId::kHead].translation_ = { 0, 0, 2.0f };
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kCenter];

	worldTransforms_[PartId::kLeft].translation_ = { -2.0f, 0, 0 };
	worldTransforms_[PartId::kLeft].parent_ = &worldTransforms_[PartId::kCenter];

	worldTransforms_[PartId::kRight].translation_ = { 2.0f, 0, 0 };
	worldTransforms_[PartId::kRight].parent_ = &worldTransforms_[PartId::kCenter];

	worldTransforms_[object2].translation_ = { -20.0f, 0, 20.0f };
	worldTransforms_[object3].translation_ = { 0, 0, 20.0f };
	worldTransforms_[object4].translation_ = { 20.0f, 0, 20.0f };
}

void GameScene::Update() {
#pragma region ワールドトランスフォーム
	if (input_->TriggerKey(DIK_Q) == true)	{
		if (moveMode == 0) { moveMode = 1; }
		else { moveMode = 0; }
	}

	// 画面上での上下左右移動
if (moveMode == 0) {
	const float kMoveSpeed = 0.5f;

	// 正面ベクトルの生成
	Vector3 frontVec;
	Vector3 target = viewProjection_.target;

	// 正面ベクトルの計算
	frontVec = target -= viewProjection_.eye;
	
	// 正面ベクトルの正規化
	MathUtility::Vector3Normalize(frontVec);	

	if (input_->PushKey(DIK_W)) {
		worldTransforms_[PartId::kCenter].translation_.x += frontVec.x * kMoveSpeed;
		worldTransforms_[PartId::kCenter].translation_.z += frontVec.z * kMoveSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		worldTransforms_[PartId::kCenter].translation_.x += -frontVec.x * kMoveSpeed;
		worldTransforms_[PartId::kCenter].translation_.z += -frontVec.z * kMoveSpeed;
	}

	// 右ベクトル・仮ベクトルの宣言
	Vector3 rightVec;
	Vector3 tentativeVec = { 0,1,0 };

	// 右ベクトルを作成
	rightVec = MathUtility::Vector3Cross(tentativeVec, frontVec);

	// 右ベクトルの正規化
	MathUtility::Vector3Normalize(tentativeVec);

	if (input_->PushKey(DIK_D)) {
		worldTransforms_[PartId::kCenter].translation_.x += rightVec.x * kMoveSpeed;
		worldTransforms_[PartId::kCenter].translation_.z += rightVec.z * kMoveSpeed;
	}
	if (input_->PushKey(DIK_A)) {
		worldTransforms_[PartId::kCenter].translation_.x += -rightVec.x * kMoveSpeed;
		worldTransforms_[PartId::kCenter].translation_.z += -rightVec.z * kMoveSpeed;
	}
}
	// バイオ歩き
if (moveMode == 1) {
	XMFloat3 move = { 0.0f, 0.0f, 0.0f };

	// 回転速度
	const float kRotSpeed = 0.05f;

	if (input_->PushKey(DIK_A)) {
		move = { 0, -kRotSpeed, 0 };
	}
	else if (input_->PushKey(DIK_D)) {
		move = { 0, kRotSpeed, 0 };
	}

	worldTransforms_[PartId::kCenter].rotation_.y += move.y;

	XMFloat3 frontVec = { 0.0f, 0.0f, -1.0f };	// 単位ベクトル(逆)
	XMFloat3 resultVec = { 0.0f, 0.0f, 0.0f };	// 結果補間用ベクトル

	resultVec.x = (cos(worldTransforms_[PartId::kCenter].rotation_.y) * frontVec.x +
				   sin(worldTransforms_[PartId::kCenter].rotation_.y) * frontVec.z);
	resultVec.z = (-sinf(worldTransforms_[PartId::kCenter].rotation_.y) * frontVec.x +
				   cosf(worldTransforms_[PartId::kCenter].rotation_.y) * frontVec.z);
}
	// デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(	"center:(%f,%f,%f)", worldTransforms_[PartId::kCenter].translation_.x,
		worldTransforms_[PartId::kCenter].translation_.y,
		worldTransforms_[PartId::kCenter].translation_.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf("moveMode(Change:Q):%d", moveMode);
	
	for (int i = 0; i < kNumPartId; i++) {
		matrix.UpdateMatrix(worldTransforms_[i]);
	}
#pragma endregion

#pragma region ビュープロジェクション
	const float kCameraSpeed = 0.1f;

	if (input_->PushKey(DIK_UP)) {
		viewProjection_.eye.z += kCameraSpeed;
		viewProjection_.target.z += kCameraSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		viewProjection_.eye.z -= kCameraSpeed;
		viewProjection_.target.z -= kCameraSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		viewProjection_.eye.x += kCameraSpeed;
		viewProjection_.target.x += kCameraSpeed;
	}
	if (input_->PushKey(DIK_LEFT)) {
		viewProjection_.eye.x -= kCameraSpeed;
		viewProjection_.target.x -= kCameraSpeed;
	}

	viewProjection_.UpdateMatrix();
#pragma endregion
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// 3Dオブジェクト描画処理
	for (int i = 0; i < kNumPartId; i++) {
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}