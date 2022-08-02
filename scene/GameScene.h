#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "WinApp.h"
#include "Matrix.h"
#include "XMFloat.h"
#include "Vector3.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ変数
	// パーツID
	enum PartId {
		kRoot,	// 大元
		kSpine,	// 脊髄
		kChest,	// 胸
		//kHead,	// 頭
		//kArmL,	// 左腕
		//kArmR,	// 右腕
		//kHip,	// 尻
		//kLegL,	// 左足
		//kLegR,	// 右足

		kNumPartId
	};

public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_[2] = {0, 0};

	// 3Dモデル
	Model* model_ = nullptr;

	// ランダム格納
	XMFloat3 random;

	// ワールドトランスフォーム
	WorldTransform worldTransforms_[100];
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// まとりっくす
	Matrix matrix;

	// カメラ上方向の角度
	float viewAngle = 0.0f;

	const int ray = 5;

	const int floor = 7;

public:
	Vector3 startRay;
	Vector3 endRay;

	bool CollisionRayToObject(Vector3 startRay, Vector3 endRay, WorldTransform object);

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
