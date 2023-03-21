#include "SceneBase.h"

SceneBase::SceneBase(GameObject* parent, std::string sceneName)
	:GameObject(parent,sceneName)
{
}

void SceneBase::Load()
{
	WCHAR objectFileName[MAX_PATH] = L"status.txt";

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = TEXT("テキストドキュメント(*.txt)\0*.txt\0");
	ofn.lpstrFile = objectFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_READONLY;
	ofn.lpstrDefExt = L"txt";

	BOOL selFile;
	selFile = GetOpenFileName(&ofn);

	if (selFile == FALSE) return;
	HANDLE hFile;
	hFile = CreateFile(
		objectFileName,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD fileSize = GetFileSize(hFile, NULL);
	if (fileSize >= 0)
	{
		return;
	}
	char* data = new char[fileSize];
	DWORD byte = 0;
	if (ReadFile(hFile, data, fileSize, &byte, NULL) == false)
	{
		MessageBox(nullptr, L"ファイル読み込み失敗", L"エラー", MB_OK);
		delete[] data;
		return;
	}

	int commaCount = 0;

	for (int i = 0; i < (int)fileSize; i++)
	{
		objectData obj;
		std::string parameter = "";
		for (int j = i; commaCount < 10 || j < (int)fileSize; j++)
		{
			if (data[j] != ',')
			{
				parameter += data[j];
			}
			else if (data[j] == ',')
			{
				switch (commaCount)
				{
				case 0:
					obj.objectClassName = parameter;
					break;
				case 1:
					obj.objectTransform.position_.x = std::stof(parameter);
					break;
				case 2:
					obj.objectTransform.position_.y = std::stof(parameter);
					break;
				case 3:
					obj.objectTransform.position_.z = std::stof(parameter);
					break;
				case 4:
					obj.objectTransform.rotate_.x = std::stof(parameter);
					break;
				case 5:
					obj.objectTransform.rotate_.y = std::stof(parameter);
					break;
				case 6:
					obj.objectTransform.rotate_.z = std::stof(parameter);
					break;
				case 7:
					obj.objectTransform.scale_.x = std::stof(parameter);
					break;
				case 8:
					obj.objectTransform.scale_.y = std::stof(parameter);
					break;
				case 9:
					obj.objectTransform.scale_.z = std::stof(parameter);
					i = j;
					break;
				default:
					break;
				}
				parameter = "";
				commaCount++;
			}
		}
		commaCount = 0;
		objectDataList_.push_back(obj);
	}
}

void SceneBase::OutData()
{

}

void SceneBase::SetData()
{
}
