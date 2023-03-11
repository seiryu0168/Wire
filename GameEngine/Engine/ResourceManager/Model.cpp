#include "Model.h"
#include<vector>
namespace ModelManager
{
	struct ModelData
	{
		Fbx* pfbx_;
		Transform transform_;
		std::string fileName_;

		//アニメーションのフレーム
		float nowFrame_;
		float animSpeed_;
		int	  startFrame_;
		int   endFrame_;

		ModelData()
		{
			pfbx_ = nullptr;
			nowFrame_ = 0;
			animSpeed_ = 0;
			startFrame_ = 0;
			endFrame_ = 0;
		}
		void SetAnimFrame(int start, int end, float speed)
		{
			nowFrame_ = (float)start;
			startFrame_ = start;
			endFrame_ = end;
			animSpeed_ = speed;
		}
	};
	std::vector<ModelData*> modelData_;
	std::vector<int> polygonTestList_;
};

int ModelManager::Load(std::string fileName)
{
	ModelData* pModelData = new ModelData;
	pModelData->fileName_ = fileName;
	pModelData->pfbx_ = nullptr;

	//既にあるファイルと名前が同じならそのアドレスを代入する
	for (auto itr = modelData_.begin(); itr != modelData_.end(); itr++)
	{
		if (fileName == (*itr)->fileName_)
		{
			pModelData->pfbx_ = (*itr)->pfbx_;
			break;
		}
	}

	if (pModelData->pfbx_==nullptr)
	{
		pModelData->pfbx_ = new Fbx;
		pModelData->pfbx_->Load(fileName);
	}
	modelData_.push_back(pModelData);
	return (int)modelData_.size() - 1;
}

void ModelManager::SetTransform(int modelNum, Transform transform)
{
	if (modelData_[modelNum] != nullptr)
	{
		modelData_[modelNum]->transform_ = transform;
	}
}
void ModelManager::Draw(int modelNum)
{
	if (modelNum < 0 || modelNum >= modelData_.size() || modelData_[modelNum] == nullptr)
	{
		return;
	}
	modelData_[modelNum]->nowFrame_ += modelData_[modelNum]->animSpeed_;
	if (modelData_[modelNum]->nowFrame_ > (float)modelData_[modelNum]->endFrame_)
		modelData_[modelNum]->nowFrame_ = (float)modelData_[modelNum]->startFrame_;
	
	if (modelData_[modelNum]->pfbx_ != nullptr)
	{
		modelData_[modelNum]->pfbx_->Draw(modelData_[modelNum]->transform_, SHADER_TYPE::SHADER_3D,(int)modelData_[modelNum]->nowFrame_);
	}
}

void ModelManager::DrawOutLine(int modelNum ,XMFLOAT4 lineColor)
{
	if (modelNum < 0 || modelNum >= modelData_.size() || modelData_[modelNum] == nullptr)
	{
		return;
	}
	modelData_[modelNum]->nowFrame_ += modelData_[modelNum]->animSpeed_;
	if (modelData_[modelNum]->nowFrame_ > (float)modelData_[modelNum]->endFrame_)
		modelData_[modelNum]->nowFrame_ = (float)modelData_[modelNum]->startFrame_;

	if (modelData_[modelNum]->pfbx_ != nullptr)
	{
		modelData_[modelNum]->pfbx_->DrawOutLine(modelData_[modelNum]->transform_, (int)modelData_[modelNum]->nowFrame_,lineColor);
	}
}

void ModelManager::DrawToon(int modelNum)
{
	if (modelNum < 0 || modelNum >= modelData_.size() || modelData_[modelNum] == nullptr)
	{
		return;
	}
	modelData_[modelNum]->nowFrame_ += modelData_[modelNum]->animSpeed_;
	if (modelData_[modelNum]->nowFrame_ > (float)modelData_[modelNum]->endFrame_)
		modelData_[modelNum]->nowFrame_ = (float)modelData_[modelNum]->startFrame_;

	if (modelData_[modelNum]->pfbx_ != nullptr)
	{
		modelData_[modelNum]->pfbx_->DrawOutLine(modelData_[modelNum]->transform_, (int)modelData_[modelNum]->nowFrame_);
	}
}

void ModelManager::RayCast(int modelNum, RayCastData& ray)
{
	if (modelNum == -1)
	{
		return;
	}
	XMMATRIX invW = XMMatrixInverse(nullptr,modelData_[modelNum]->transform_.GetWorldMatrix());
	XMVECTOR vStart = XMLoadFloat3(&ray.start);
	XMVECTOR vPass = XMVectorSet(ray.start.x+ray.dir.x, ray.start.y + ray.dir.y, ray.start.z + ray.dir.z,0);

	vStart = XMVector3TransformCoord(vStart, invW);
	vPass= XMVector3TransformCoord(vPass, invW);

	XMStoreFloat3(&ray.start, vStart);
	XMStoreFloat3(&ray.dir, vPass - vStart);
	modelData_[modelNum]->pfbx_->RayCast(ray,modelData_[modelNum]->transform_);
}

void ModelManager::RayCast(RayCastData& ray)
{
	bool isHit = false;
	float length = 9999;
	XMVECTOR hitPos = XMVectorSet( 0,0,0,0 );
	XMVECTOR normal= XMVectorSet(0, 0, 0, 0);

	XMFLOAT3 start = ray.start;
	XMFLOAT3 dir = ray.dir;
	int hitModel = -1;
	ray.hitModelList.clear();
	//レイが対象になっているモデルに当たっているかどうか
	for (int i = 0; i < polygonTestList_.size(); i++)
	{
		//ローカル座標に変換するための逆行列
		XMMATRIX invW = XMMatrixInverse(nullptr, modelData_[polygonTestList_[i]]->transform_.GetWorldMatrix());
		
		//スタート位置と方向をベクトルにする
		XMVECTOR vStart = XMLoadFloat3(&start);
		XMVECTOR vPass = XMVectorSet(start.x + dir.x, start.y + dir.y, start.z + dir.z, 0);
		
		//逆行列で変換させる
		vStart = XMVector3TransformCoord(vStart, invW);
		vPass = XMVector3TransformCoord(vPass, invW);

		//ベクトルから座標に戻す
		XMStoreFloat3(&ray.start, vStart);
		XMStoreFloat3(&ray.dir, vPass - vStart);
		
		ray.hit = false;
		//レイキャスト
		modelData_[polygonTestList_[i]]->pfbx_->RayCast(ray, modelData_[polygonTestList_[i]]->transform_);
		
		//当たっていたら
		if (ray.hit)
		{

			if (length > ray.dist)
			{
				isHit = ray.hit;
				length = ray.dist;
				hitPos = ray.hitPos;
				normal = ray.normal;
			}
			ray.hitModelList.push_back({ polygonTestList_[i], ray.dist });
		}
	}
	ray.Adjust();
	ray.dir = dir;
	ray.start = start;
	ray.dist = length;
	ray.hit = isHit;
	ray.hitPos = hitPos;
	ray.normal = normal;
}

void ModelManager::SetModelNum(int modelNum)
{
	polygonTestList_.push_back(modelNum);
}

void ModelManager::DeleteModelNum(int modelNum)
{
		auto itr = polygonTestList_.begin();
	for (int i = 0; i <  polygonTestList_.size(); i++)
	{
		if (polygonTestList_[i] == modelNum)
		{
			polygonTestList_.erase(itr);
			return;
			
		}
		itr++;
	}
}

void ModelManager::AllDeleteModelNum()
{
	polygonTestList_.clear();
}

XMFLOAT3 ModelManager::GetBonePosition(int modelNum,std::string boneName)
{

	if (modelNum<0 || modelNum>modelData_.size() || modelData_[modelNum] == nullptr)
	{
		return XMFLOAT3(-9999.0f, -9999.0f, 9999.0f);
	}
	XMFLOAT3 pos = modelData_[modelNum]->pfbx_->GetBonePosition(boneName);
	XMVECTOR vBone = XMVector3TransformCoord(XMLoadFloat3(&pos), modelData_[modelNum]->transform_.GetWorldMatrix());
	XMStoreFloat3(&pos, vBone);
	return pos;
}

//複数のポインタが同じアドレスを参照してるから参照してない所までmodelData_を進めなきゃいけない
void ModelManager::Release()
{
	for (int i = 0; i < modelData_.size(); i++)
	{
		bool isRef = false;//参照しているかどうかのフラグ
		for (int j = i + 1; j < modelData_.size(); j++)
		{
			//同じものが参照されてたらtrue
			if (modelData_[i]->pfbx_ == modelData_[j]->pfbx_)
			{
				isRef = true;
			}
		}
		//もう参照されてないなら解放
		if (isRef==false)
		{
			SAFE_DELETE(modelData_[i]->pfbx_);
		}

		SAFE_DELETE(modelData_[i]);
	}
	modelData_.clear();
}