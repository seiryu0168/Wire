#pragma once
#include "Engine/GameObject/GameObject.h"
#include"Engine/ResourceManager/Text.h"
#include<map>
class MissionUI : public GameObject
{
public:
	std::vector<Text*> textList;
	std::map<std::string, Text* > noticeTextList_;
	std::vector<int> imageList;

	MissionUI(GameObject* parent,std::string uiName);
	~MissionUI();

	void SetText(std::vector<Text*> txtList);
	void SetImage(std::vector<int> imgList);
	/// <summary>
	/// 通知テキスト追加
	/// </summary>
	/// <param name="key">キー</param>
	/// <param name="text">テキスト</param>
	void AddNotice(std::string key, Text* text);
	
	/// <summary>
	/// 通知内容を変える
	/// </summary>
	/// <param name="key">キー</param>
	/// <param name="text">変更内容</param>
	void ChangeNotice(std::string key, std::string text);
	
	/// <summary>
	/// 通知を消す
	/// </summary>
	/// <param name="key">キー</param>
	void DelNotice(std::string key);

	/// <summary>
	/// テキスト間の距離計算
	/// </summary>
	void CalcTextDistance();
	void Release();
};

