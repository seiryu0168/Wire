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
	/// �ʒm�e�L�X�g�ǉ�
	/// </summary>
	/// <param name="key">�L�[</param>
	/// <param name="text">�e�L�X�g</param>
	void AddNotice(std::string key, Text* text);
	
	/// <summary>
	/// �ʒm���e��ς���
	/// </summary>
	/// <param name="key">�L�[</param>
	/// <param name="text">�ύX���e</param>
	void ChangeNotice(std::string key, std::string text);
	
	/// <summary>
	/// �ʒm������
	/// </summary>
	/// <param name="key">�L�[</param>
	void DelNotice(std::string key);

	/// <summary>
	/// �e�L�X�g�Ԃ̋����v�Z
	/// </summary>
	void CalcTextDistance();
	void Release();
};

