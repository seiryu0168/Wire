#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>

using namespace DirectX;

//-----------------------------------------------------------
//カメラ
//-----------------------------------------------------------
namespace Camera
{
	//初期化（プロジェクション行列作成）
	void Initialize(float width, float height);

	//更新（ビュー行列作成）
	void Update();

	//視点（カメラの位置）を設定
	void SetPosition(XMVECTOR position);

	//焦点（見る位置）を設定
	void SetTarget(XMVECTOR target);

	//カメラの上方向ベクトルを設定
	void SetUpVector(XMVECTOR upVector);

	//視点(見える位置)を取得
	XMFLOAT3 GetPosition();
	//ビュー行列を取得
	XMMATRIX GetViewMatrix();

	//プロジェクション行列を取得
	XMMATRIX GetProjectionMatrix();

};