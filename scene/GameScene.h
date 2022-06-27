﻿#pragma once

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

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ変数
	// パーツID
	enum PartId {
		kCenter,	// 大元
		kHead,	// 頭
		kLeft,	// 左腕
		kRight,	// 右腕
		object2,
		object3,
		object4,

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
	uint32_t textureHandle_ = 0;

	// 3Dモデル
	Model* model_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransforms_[100];
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// マトリックス生成
	Matrix matrix;

	// 操作切り替え
	bool moveMode;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
