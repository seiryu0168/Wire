#pragma once
#include<DirectXMath.h>
#include"Fbx.h"
using namespace DirectX;

namespace Math
{
	float Det(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c);
	/// <summary>
	/// 線分と面の判定
	/// </summary>
	/// <param name="start">線分の開始位置</param>
	/// <param name="dir">線分の方向</param>
	/// <param name="v0">面を構成する頂点1</param>
	/// <param name="v1">面を構成する頂点2</param>
	/// <param name="v2">面を構成する頂点3</param>
	/// <param name="dist">開始位置からの距離</param>
	/// <param name="hitPos">線分との衝突位置(ローカル座標)</param>
	/// <returns>当たったかどうか</returns>
	bool Intersect(XMFLOAT3 start, XMFLOAT3 dir, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2,float& dist, XMVECTOR& hitPos);
	
	/// <summary>
	/// 線分と面の判定
	/// </summary>
	/// <param name="rayCast">レイのデータ</param>
	/// <param name="v0">面を構成する頂点1</param>
	/// <param name="v1">面を構成する頂点2</param>
	/// <param name="v2">面を構成する頂点3</param>
	/// <returns>当たったかどうか</returns>
	bool Intersect(RayCastData& rayCast, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2);
	/// <summary>
	/// 法線との角度を調べる
	/// </summary>
	/// <param name="vNormal">調べたい法線</param>
	/// <param name="vDir">方向</param>
	/// <returns>表かどうか</returns>
	bool IsFrontSurface(XMVECTOR vNormal, XMVECTOR vDir);
}

