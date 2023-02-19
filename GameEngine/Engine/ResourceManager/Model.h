#pragma once
#include<string>
#include"../DirectX_11/Fbx.h"
#include"../GameObject/Transform.h"
#include<vector>
namespace ModelManager
{
	/// <summary>
	/// モデルのロード
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns></returns>
	int Load(std::string fileName);
	/// <summary>
	/// transformを設定
	/// </summary>
	/// <param name="modelNum">モデル番号</param>
	/// <param name="transform">呼び出し元のtransform</param>
	void SetTransform(int modelNum, Transform transform);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="modelNum">モデル番号</param>
	void Draw(int modelNum);
	void DrawOutLine(int modelNum);
	void DrawToon(int modelNum);
	/// <summary>
	/// レイキャスト
	/// </summary>
	/// <param name="modelNum">レイを飛ばすモデル番号</param>
	/// <param name="ray">レイのデータ</param>
	void RayCast(int modelNum, RayCastData& ray);
	/// <summary>
	/// 複数の3Dモデルに対してレイキャスト
	/// </summary>
	/// <param name="ray">レイのデータ</param>
	void RayCast(RayCastData& ray);
	/// <summary>
	/// 複数レイキャストに使うモデル番号の追加
	/// </summary>
	/// <param name="modelNum"></param>
	void SetModelNum(int modelNum);
	/// <summary>
	/// 複数レイキャストに使うモデル番号の削除
	/// </summary>
	/// <param name="modelNum"></param>
	void DeleteModelNum(int modelNum);
	void Release();
};

