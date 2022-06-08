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
	// viewProjection_.eye = { 0,0,-10 };

	// カメラ注視点座標を設定
	// viewProjection_.target = { 10,0,0 };

	// カメラ上方向ベクトルを設定(右上45度指定)
	// viewProjection_.up = { cosf(XM_PI / 4.0f), sinf(XM_PI / 4.0f), 0.0f };
#pragma endregion
#pragma region 射影変換行列
	// カメラ垂直方向視野角を設定
	/*viewProjection_.fovAngleY = 10.0f * (XM_PI / 180);*/

	// アスペクト比を設定(1.0fであると、画面比1:1のときに正常な大きさになる)
	// viewProjection_.aspectRatio = 1.0f;

	// ニアクリップ、ファークリップを設定
	/*viewProjection_.nearZ = 52.0f;
	viewProjection_.farZ = 53.0f;*/
	
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

	worldTransforms_[PartId::kSpine].Initialize();
	worldTransforms_[PartId::kSpine].translation_ = { 0, 4.5f, 0 };
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];

	worldTransforms_[PartId::kChest].Initialize();
	worldTransforms_[PartId::kChest].translation_ = { 0, 1.0f, 0 };
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
							 
	worldTransforms_[PartId::kHead].Initialize();
	worldTransforms_[PartId::kHead].translation_ = { 0, 3.0f, 0 };
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
							 
	worldTransforms_[PartId::kArmL].Initialize();
	worldTransforms_[PartId::kArmL].translation_ = { -3.0f, 0, 0 };
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
							 
	worldTransforms_[PartId::kArmR].Initialize();
	worldTransforms_[PartId::kArmR].translation_ = { 3.0f, 0, 0 };
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
				  			 
	worldTransforms_[PartId::kHip].Initialize();
	worldTransforms_[PartId::kHip].translation_ = { 0, -2.0f, 0 };
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
				  			 
	worldTransforms_[PartId::kLegL].Initialize();
	worldTransforms_[PartId::kLegL].translation_ = { -3.0f, -3.0f, 0 };
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
				  			 
	worldTransforms_[PartId::kLegR].Initialize();
	worldTransforms_[PartId::kLegR].translation_ = { 3.0f, -3.0f, 0 };
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];

	//for (WorldTransform& worldTransform : worldTransforms_) {
	//	// ワールドトランスフォームの初期化
	//	worldTransform.Initialize();
	//	//スケールチェンジ初期化
	//	matrix.ScaleChange(worldTransform, 1.0f, 1.0f, 1.0f, 1.0f);

	//	//乱数範囲の指定
	//	std::uniform_real_distribution<float> rotaDistX(0, XM_PI);
	//	std::uniform_real_distribution<float> rotaDistY(0, XM_PI);
	//	std::uniform_real_distribution<float> rotaDistZ(0, XM_PI);

	//	random.x = rotaDistX(engine);
	//	random.y = rotaDistY(engine);
	//	random.z = rotaDistZ(engine);

	//	//回転初期化
	//	matrix.RotaChange(worldTransform, random.x, random.y, random.z);

	//	//乱数範囲の指定
	//	std::uniform_real_distribution<float> transDistX(-10, 10);
	//	std::uniform_real_distribution<float> transDistY(-10, 10);
	//	std::uniform_real_distribution<float> transDistZ(-10, 10);

	//	random.x = transDistX(engine);
	//	random.y = transDistY(engine);
	//	random.z = transDistZ(engine);

	//	//平行移動
	//	matrix.ChangeTranslation(worldTransform, random.x, random.y, random.z);

	//	matrix.UpdateMatrix(worldTransform);
	//}
}

void GameScene::Update() {

#pragma region ビュー変換行列
//	// 視点移動処理
//	{
//		// 視点の移動ベクトル
//		Vector3 move = { 0,0,0 };
//
//		// 視点移動の速さ
//		const float kEyeSpeed = 0.2f;
//
//		// 押した方向で移動ベクトルを変更
//		if (input_->PushKey(DIK_W))
//		{
//			move.z += kEyeSpeed;
//		}
//		else if (input_->PushKey(DIK_S))
//		{
//			move.z -= kEyeSpeed;
//		}
//
//		// 視点移動(ベクトル加算)
//		viewProjection_.eye += move;
//
//		// 行列の再計算
//		viewProjection_.UpdateMatrix();
//
//		// デバッグ用表示
//		debugText_->SetPos(50, 50);
//		debugText_->Printf(
//			"eye:(%f,%f,%f)",
//			viewProjection_.eye.x,
//			viewProjection_.eye.y,
//			viewProjection_.eye.z);
//	}
//
//	// 注視点移動処理
//	{
//		// 注視点の移動ベクトル
//		Vector3 move = { 0,0,0 };
//
//		// 注視点移動の速さ
//		const float kTargetSpeed = 0.2f;
//
//		// 押した方向で移動ベクトルを変更
//		if (input_->PushKey(DIK_LEFT))
//		{
//			move.x -= kTargetSpeed;
//		}
//		else if (input_->PushKey(DIK_RIGHT))
//		{
//			move.x += kTargetSpeed;
//		}
//
//		// 注視点移動(ベクトルの加算)
//		viewProjection_.target += move;
//
//		// 行列の再計算
//		viewProjection_.UpdateMatrix();
//
//		// デバッグ用表示
//		debugText_->SetPos(50, 70);
//		debugText_->Printf(
//			"target:(%f,%f,%f)",
//			viewProjection_.target.x,
//			viewProjection_.target.y,
//			viewProjection_.target.z);
//	}
//
//	// 上方向回転処理
//	{
//		// 上方向の回転速さ[ラジアン/フレーム]
//		const float kUpRotSpeed = 0.05f;
//
//		// 押した方向で移動ベクトルを変更
//		if (input_->PushKey(DIK_SPACE))
//		{
//			viewAngle += kUpRotSpeed;
//			
//			// 2πを越えたら0にする
//			viewAngle = fmodf(viewAngle, XM_PI * 2.0f);
//		}
//
//		// 上方向ベクトルを計算(半径1の演習場)
//		viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};
//
//		// 行列の再計算
//		viewProjection_.UpdateMatrix();
//
//		// デバッグ用表示
//		debugText_->SetPos(50, 90);
//		debugText_->Printf(
//			"up:(%f,%f,%f)",
//			viewProjection_.up.x,
//			viewProjection_.up.y,
//			viewProjection_.up.z);
//	}
#pragma endregion
#pragma region 射影変換行列
	//// FoV変更処理
	//{
	//	const float angleChangeSpeed = 0.05f;
	//	// 上キーで視野角が広がる 下キーで視野角が狭まる
	//	if (input_->PushKey(DIK_UP)) {
	//		if (viewProjection_.fovAngleY < XM_PI) {
	//			viewProjection_.fovAngleY += angleChangeSpeed;
	//		}
	//	}
	//	else if (input_->PushKey(DIK_DOWN)) {
	//		if (viewProjection_.fovAngleY > 0.05f) {
	//			viewProjection_.fovAngleY -= angleChangeSpeed;
	//		}
	//	}
	//	// 行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	// デバッグ用表示
	//	debugText_->SetPos(50, 110);
	//	debugText_->Printf(
	//		"fovAnglrY(Degree):%f",
	//		(viewProjection_.fovAngleY) / (XM_PI / 180));
	//}
//
//// クリップ距離変更処理
//	{
//		const float nearChangeSpeed = 0.05f;
//		// 上キーで視野角が広がる 下キーで視野角が狭まる
//		if (input_->PushKey(DIK_UP)) {
//			viewProjection_.nearZ += nearChangeSpeed;
//		}
//		else if (input_->PushKey(DIK_DOWN)) {
//			viewProjection_.nearZ -= nearChangeSpeed;
//		}
//		// 行列の再計算
//		viewProjection_.UpdateMatrix();
//
//		// デバッグ用表示
//		debugText_->SetPos(50, 130);
//		debugText_->Printf("nearZ:%f",viewProjection_.nearZ);
//	}

#pragma endregion

	// キャラクター移動処理
	{
		// キャラクターの移動ベクトル
		XMFloat3 move = { 0, 0, 0 };

		// キャラクター移動の速さ
		const float kCharacterSpeed = 0.2f;

		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = { -kCharacterSpeed, 0, 0 };
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move = { kCharacterSpeed, 0, 0 };
		}

		// 注視点移動 (ベクトルの加算)
		worldTransforms_[PartId::kRoot].translation_.x += move.x;
		worldTransforms_[PartId::kRoot].translation_.y += move.y;
		worldTransforms_[PartId::kRoot].translation_.z += move.z;

		for (int i = 0; i < kNumPartId; i++)
		{
			matrix.UpdateMatrix(worldTransforms_[i]);
		}

		// デバッグ用表示
		debugText_->SetPos(50, 150);
		debugText_->Printf(
			"Root:(%f,%f,%f)", worldTransforms_[PartId::kRoot].translation_.x,
			worldTransforms_[PartId::kRoot].translation_.y,
			worldTransforms_[PartId::kRoot].translation_.z);
	}

	// 上半身回転処理
	{
		// 上半身の回転の速さ[ラジアン/frame]
		const float kChestRotSpeed = 0.05f;

		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_U)) {
			worldTransforms_[PartId::kChest].rotation_.y -= kChestRotSpeed;
		}
		else if (input_->PushKey(DIK_I)) {
			worldTransforms_[PartId::kChest].rotation_.y += kChestRotSpeed;
		}
	}
	// 上半身回転処理
	{
		// 上半身の回転の速さ[ラジアン/frame]
		const float kHipRotSpeed = 0.05f;

		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_J)) {
			worldTransforms_[PartId::kHip].rotation_.y -= kHipRotSpeed;
		}
		else if (input_->PushKey(DIK_K)) {
			worldTransforms_[PartId::kHip].rotation_.y += kHipRotSpeed;
		}
	}

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
	
	for (int i = 2; i < kNumPartId; i++)
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

	for (float i = 0; i < 21; i++)
	{
		PrimitiveDrawer::GetInstance()->DrawLine3d({ -20, -20 + (i * 2), 0 }, {  20, -20 + (i * 2), 0 }, { 1,0,0,1 });
		PrimitiveDrawer::GetInstance()->DrawLine3d({ -20 + (i * 2), -20, 0 }, { -20 + (i * 2), 20, 0 }, { 0,1,0,1 });
		PrimitiveDrawer::GetInstance()->DrawLine3d({ -20 + (i * 2), 0, -20 }, { -20 + (i * 2) ,0 ,20, }, { 0,0,1,1 });
	}
}
