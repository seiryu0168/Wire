#include "SetObject.h"
#include<fstream>
#include<sstream>
#include"resource.h"
SetObject::SetObject(GameObject* scene)
	:pScene_(scene)
{

}

//デストラクタ
SetObject::~SetObject()
{

}

void SetObject::Load(std::string fileName)
{
	//カレントディレクトリ取得
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);
	
	WCHAR path[FILENAME_MAX];
	//size_t ret;
	//mbstowcs_s(&ret, path, filePath.c_str(), filePath.length());
	//WCHAR dir[MAX_PATH];
	//std::wstring fileName;
	//std::wstring extName;
	//WCHAR fileName[FILENAME_MAX];
	//_wsplitpath_s(path, nullptr, 0, dir, MAX_PATH, fileName, FILENAME_MAX, nullptr, 0);
	
	if (SetCurrentDirectory(L"Assets")==ERROR_FILE_NOT_FOUND)
	{
		return;
	}
	std::ifstream fileReader;
	fileReader.open(fileName);
	std::string line;
	std::vector<char> dels;
	dels.push_back(':');
	dels.push_back(',');
	std::vector<std::string> recive;
	while (std::getline(fileReader, line))
	{
		objectData data;
		recive=SplitLine(line, dels);
		data.objectName = recive[0];
		data.objectTransform.position_.x = stof(recive[1]);
		data.objectTransform.position_.y = stof(recive[2]);
		data.objectTransform.position_.z = stof(recive[3]);
		objectDataList_.push_back(data);
	}
	fileReader.close();
	SetCurrentDirectory(currentDir);

	/*WCHAR objectFileName[MAX_PATH]=L"status.txt";

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = TEXT("テキストドキュメント(*.txt)\0*.txt\0");
	ofn.lpstrFile   = objectFileName;
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

	DWORD fileSize = GetFileSize(hFile,NULL);
	if (fileSize >= 0)
	{
		return;
	}
	char* data = new char[fileSize];
	DWORD byte = 0;
	if (ReadFile(hFile, data, fileSize, &byte, NULL)==false)
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
		for (int j = i; commaCount<10||j<(int)fileSize; j++)
		{
			if (data[j] != ',')
			{
				parameter += data[j];
			}
			else if(data[j] == ',')
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
					i=j;
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
	}*/
}

std::vector<std::string> SetObject::SplitLine(const std::string& line, const char& delimiter)
{
	std::vector<std::string>  ret;
	std::istringstream st(line);
	
	//文字列の初期位置をカウントしておく
	int first = 0;
	//終了位置
	int last = line.find_first_of(delimiter);
	while (first<line.size())
	{
		//既存の文字列から範囲指定して文字列作る
		std::string parts(line, first, last - first);
		
		//区切った文字列追加
		ret.push_back(parts);
		//初期位置更新
		first=last+1;
		
		//終了位置更新
		last = line.find_first_of(delimiter, first);

		//find_first_of関数で見つからなかった場合
		if (last == std::string::npos)
			last = line.size();

	}

	return ret;
}

std::vector<std::string> SetObject::SplitLine(const std::string& line, std::vector<char> delimiters)
{
	std::vector<std::string> ret;
	size_t first = 0;
	size_t last = 0;
	while (first<line.size())
	{
		size_t lastSub = 0;
		size_t length = 9999999;
		//最も小さく分割する
		for (int i = 0; i < delimiters.size(); i++)
		{
			lastSub = line.find_first_of(delimiters[i], first);
			//デリミタが見つかったら
			if (lastSub != std::string::npos)
			{
				//lengthより小さかったら
				if (lastSub - first <= length)
				{
					last = lastSub;
					length = lastSub - first;
				}
			}
			else
				last = line.size();	

		}

		std::string parts(line, first, last - first);
		first=last+1;
		ret.push_back(parts);
	}
	return ret;
}

void SetObject::ObjectSet()
{
	auto itr = pScene_->GetChildList()->begin();
	for (int i = 0; i < objectDataList_.size(); i++)
	{

		(*itr)->SetPosition(objectDataList_[i].objectTransform.position_);
		itr++;
		if (itr == pScene_->GetChildList()->end())
			break;
	}
}