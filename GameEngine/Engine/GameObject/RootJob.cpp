#include "RootJob.h"
#include"../SceneManager.h"
#include"../SAFE_DELETE_RELEASE.h"

#pragma comment(lib, "winmm.lib")

RootJob::RootJob() 
	: GameObject(nullptr,"RootJob")
{

}

RootJob::~RootJob()
{

}

void RootJob::Initialize()
{
	if (FindChild("SceneManager") == nullptr)
	{
		Instantiate<SceneManager>(this);
	}
}

void RootJob::Update()
{
}

void RootJob::Draw()
{

}

void RootJob::Release()
{
}