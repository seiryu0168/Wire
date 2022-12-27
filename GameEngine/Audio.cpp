#include "Audio.h"
#include<xaudio2.h>
#include<vector>
namespace Audio
{
	//XAudio本体
	IXAudio2* pXAudio_ = nullptr;

	IXAudio2MasteringVoice* pMastaringVoice_ = nullptr;

	struct AudioData
	{
		XAUDIO2_BUFFER buf = {};

		IXAudio2SourceVoice** pSourceVoice = nullptr;

		//同時再生数(最大)
		int svNum;

		//ファイル名
		std::string fileName;
	};
	std::vector<AudioData> audioList_;
}

void Audio::Initialize()
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
	XAudio2Create(&pXAudio_);
	pXAudio_->CreateMasteringVoice(&pMastaringVoice_);
}

int Audio::Load(std::string fileName, int svNum)
{
	for (int i = 0; i < audioList_.size(); i++)
	{
		if (audioList_[i].fileName == fileName)
		{
			return i;
		}
	}

	struct Chunk
	{
		char id[4];			//ID
		unsigned int size;	//サイズ
	};

	HANDLE hFile;
	//パス名をファイル名と拡張子だけにする
	char name[_MAX_FNAME];	//ファイル名
	char ext[_MAX_EXT];		//拡張子
	_splitpath_s(fileName.c_str(), nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
	sprintf_s(name, "%s%s", name, ext);

	wchar_t wtext[FILENAME_MAX];
	size_t ret;
	mbstowcs_s(&ret, wtext, fileName.c_str(), fileName.length());
	hFile = CreateFile(wtext, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwBytes = 0;

	Chunk riffChunk;
	ReadFile(hFile, &riffChunk, 8, &dwBytes, NULL);

	char wave[4];
	ReadFile(hFile, &wave, 4, &dwBytes, NULL);
	
	Chunk formatChunk;
	ReadFile(hFile, &formatChunk, 8, &dwBytes, NULL);
	
	WAVEFORMATEX fmt;
	ReadFile(hFile, &fmt, formatChunk.size, &dwBytes, NULL);
	
	Chunk data;
	ReadFile(hFile, &data, 8, &dwBytes, NULL);

	char* pBuffer = new char[data.size];
	ReadFile(hFile, pBuffer, 8, &dwBytes, NULL);

	CloseHandle(hFile);

	AudioData ad;
	ad.fileName = fileName;
	ad.buf.pAudioData = (BYTE*)pBuffer;
	ad.buf.Flags = XAUDIO2_END_OF_STREAM;
	ad.buf.AudioBytes = data.size;

	ad.pSourceVoice = new IXAudio2SourceVoice * [svNum];
	for (int i = 0; i < svNum; i++)
	{
		pXAudio_->CreateSourceVoice(&ad.pSourceVoice[i], &fmt);
	}
	ad.svNum = svNum;
	audioList_.push_back(ad);
	return (int)audioList_.size() - 1;

}

void Audio::Play(int ID)
{

}

void Audio::Releace()
{

}