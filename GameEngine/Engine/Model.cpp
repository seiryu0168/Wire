#include "Model.h"
#include<vector>
//#include"Direct3D.h"
namespace Model
{
	struct ModelData
	{
		Fbx* pfbx_;
		Transform transform_;
		std::string fileName_;
	};
	std::vector<ModelData*> modelData_;
};

int Model::Load(std::string fileName)
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
	return modelData_.size() - 1;
}

void Model::SetTransform(int modelNum, Transform transform)
{
	if (modelData_[modelNum] != nullptr)
	{
		modelData_[modelNum]->transform_ = transform;
	}
}
void Model::Draw(int modelNum)
{
	if (modelData_[modelNum]->pfbx_ != nullptr)
	{
		modelData_[modelNum]->pfbx_->Draw(modelData_[modelNum]->transform_, SHADER_3D);
	}
}

void Model::RayCast(int modelNum, RayCastData& ray)
{
	XMMATRIX invW = XMMatrixInverse(nullptr,modelData_[modelNum]->transform_.GetWorldMatrix());
	XMVECTOR vStart = XMLoadFloat3(&ray.start);
	XMVECTOR vPass = XMVectorSet(ray.start.x+ray.dir.x, ray.start.y + ray.dir.y, ray.start.z + ray.dir.z,0);

	vStart = XMVector3TransformCoord(vStart, invW);
	vPass= XMVector3TransformCoord(vPass, invW);

	XMStoreFloat3(&ray.start, vStart);
	XMStoreFloat3(&ray.dir, vPass - vStart);
		modelData_[modelNum]->pfbx_->RayCast(ray,modelData_[modelNum]->transform_);
}


//複数のポインタが同じアドレスを参照してるから参照してない所までmodelData_を進めなきゃいけない
void Model::Release()
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
		if (!isRef)
		{
			SAFE_DELETE(modelData_[i]->pfbx_);
		}

		SAFE_DELETE(modelData_[i]);
	}
	modelData_.clear();
}