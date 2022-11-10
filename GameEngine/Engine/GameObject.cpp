#include "GameObject.h"
#include"SAFE_DELETE_RELEASE.h"
GameObject::GameObject() : GameObject(nullptr,"")
{

}
GameObject::GameObject(GameObject* parent, const std::string& name)
	: pParent_(parent),objectName_(name),killFlag(0)
{

}
GameObject::~GameObject()
{

}

void GameObject::UpdateSub()
{
	/////////アップデート/////////
	Update();
	for (auto itr = childList_.begin(); itr != childList_.end();itr++)
	{
		(*itr)->UpdateSub();
	}
	////////当たり判定///////////
	for (auto itr = childList_.begin(); itr != childList_.end();)
	{
		if ((*itr)->killFlag==true)
		{
			(*itr)->ReleaseSub();
			delete *itr;
			itr=childList_.erase(itr);
		}
		else
		{
			(*itr)->Collision(GetParent());
			itr++;
		}
	}
}

void GameObject::FixedUpdateSub()
{
	FixedUpdate();
	for(auto itr = childList_.begin();itr!=childList_.end();itr++)
	{
		(*itr)->FixedUpdateSub();
	}

	for (auto itr = childList_.begin(); itr != childList_.end();)
	{
		if ((*itr)->killFlag == true)
		{
			(*itr)->ReleaseSub();
			delete* itr;
			itr = childList_.erase(itr);
		}
		else
		{
			(*itr)->Collision(GetParent());
			itr++;
		}
	}
}

void GameObject::DrawSub()
{
	Draw();
	for (auto i = childList_.begin(); i != childList_.end(); i++)
	{
		(*i)->DrawSub();
	}
}

void GameObject::ReleaseSub()
{
	for (auto i = colliderList_.begin(); i != colliderList_.end(); i++)
	{
		SAFE_DELETE(*i);
	}
	for (auto i = childList_.begin(); i != childList_.end(); i++)
	{
		(*i)->ReleaseSub();
		SAFE_DELETE(*i);
	}

	Release();
}






//////////////////////////当たり判定関係////////////////////

//衝突判定
void GameObject::Collision(GameObject* pTarget)
{
	//pTargetが自分かnullptrだったらなんもしない
	if (pTarget == nullptr||pTarget==this)
	{
		return;
	}
	//複数コライダー持ってるかもしれないから二重ループ
	for (auto i = this->colliderList_.begin(); i != this->colliderList_.end(); i++)
	{

		for (auto j = pTarget->colliderList_.begin(); j != pTarget->colliderList_.end();j++)
		{
			if ((*i)->IsHit(*j))
			{
				this->OnCollision(pTarget);
			}
		}
	}

	if (pTarget->childList_.empty())
	{
		return;
	}

	for (auto itr = pTarget->childList_.begin(); itr != pTarget->childList_.end(); itr++)
	{
		Collision(*itr);
	}
}

//コライダーを追加
//collider->pColObjectにセットする奴
void GameObject::AddCollider(Collider* collider)
{
	collider->SetGemaObject(this);
	colliderList_.push_back(collider);
}

void GameObject::KillAllChildren()
{
	//リストに何も無ければ終わり
	if (childList_.empty())
	{
		return;
	}
	//リストにある子どもを全員消す
	for (auto itr = this->childList_.begin(); itr != this->childList_.end();)
	{
		KillObjectSub(*itr);
		delete* itr;
		itr = this->childList_.erase(itr);
	}
}

void GameObject::KillObjectSub(GameObject* pTarget)
{
	if (!pTarget->childList_.empty())
	{
		for (auto itr = pTarget->childList_.begin(); itr != pTarget->childList_.end(); itr++)
		{
			KillObjectSub(*itr);
			delete* itr;
			itr = pTarget->childList_.erase(itr);
		}
		pTarget->childList_.clear();
	}
	pTarget->Release();
}
void GameObject::PushBackChild(GameObject* pTarget)
{
	assert(pTarget != nullptr);
	pTarget->pParent_ = this;
	childList_.push_back(pTarget);
}

//親を取得
GameObject* GameObject::GetParent()
{
	return pParent_;
}

GameObject* GameObject::GetRootJob()
{
	if (this->GetParent() == nullptr)
	{
		return this;
	}
	else return GetParent()->GetRootJob();
}

GameObject* GameObject::FindObject(std::string name)
{
	GameObject* obj= GetRootJob()->FindChild(name);
	return obj;//GetRootJob()->FindChild(name);
}

GameObject* GameObject::FindChild(std::string name)
{
	
	if (this->childList_.empty())
	{
		return nullptr;
	}

	//子の中から探す
	for (auto itr = this->childList_.begin(); itr != this->childList_.end(); itr++)
	{
		//名前が一致したら返す
		if (name == (*itr)->objectName_)
		{
			return *itr;
		}

		//孫のオブジェクトも探す
		GameObject* obj = (*itr)->FindChild(name);
		if (obj != nullptr)
		{
			return obj;
		}
	}	
	//見つからなかった時
	return nullptr;
}

Transform GameObject::GetTransform()
{
	return this->transform_;
}
XMFLOAT3  GameObject::GetPosition()
{
	return this->transform_.position_;
}
XMFLOAT3  GameObject::GetRotate()
{
	return this->transform_.rotate_;
}
XMFLOAT3  GameObject::GetScale()
{
	return this->transform_.scale_;
}

float GameObject::Easing(int frameCount)
{

}
float GameObject::Easing(float time)
{
}

//ワールド行列取得
//親の影響込みの最終的な行列
XMMATRIX GameObject::GetWorldMatrix()
{
	return transform_.GetWorldMatrix();
}