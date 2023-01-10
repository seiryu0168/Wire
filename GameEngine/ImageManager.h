#pragma once
#include<vector>
#include<string>

namespace ImageManager
{
	/// <summary>
	/// 画像ロード
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>画像番号</returns>
	int Load(std::string fileName);
	
	/// <summary>
	/// 画像描画
	/// </summary>
	/// <param name="imageHandle">画像番号</param>
	void Draw(int imageHandle);
	
	/// <summary>
	/// 画像の切り抜き範囲リセット
	/// </summary>
	/// <param name="imageHandle">画像番号</param>
	void ResetRect(int imageHandle);
	
	/// <summary>
	/// 画像の切り抜き範囲設定
	/// </summary>
	/// <param name="imageHandle">画像番号</param>
	/// <param name="top">上</param>
	/// <param name="bottom">下</param>
	/// <param name="left">左</param>
	/// <param name="right">右</param>
	void SetRect(int imageHandle,int top,int bottom,int left,int right);
};

