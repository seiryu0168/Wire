#include "RootJob.h"
#include"SceneManager.h"
#include<Windows.h>
#include"SAFE_DELETE_RELEASE.h"
#include"../EngineTime.h"

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
	Instantiate<SceneManager>(this);
}

void RootJob::Update()
{
	EngineTime::SetTime();
}

void RootJob::Draw()
{

}

void RootJob::Release()
{
}