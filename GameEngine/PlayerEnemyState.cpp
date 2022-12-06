#include "PlayerEnemyState.h"
#include"Engine/Input.h"
#include"Engine/Model.h"
PlayerState::PlayerState(Player* player)
{
	pPlayer_ = player;
}

void PlayerState::Initialize()
{
}

void PlayerState::Update()
{
    //RayCastData ray;

    //if (Input::GetLTrigger())
    //{
    //    //レイキャストの判定距離の上限
    //    ray.distLimit = 100.0f;
    //    aimFlag_ = true;
    //    //当たる位置の計算
    //    XMVECTOR vPlayerDir = XMVector3TransformCoord(vBaseTarget_, matCamY_ * matCamX_);
    //    XMVECTOR vPtrDir = vPlayerDir;
    //    XMStoreFloat3(&ray.start, pPlayer_->vPlayerPos_);
    //    XMStoreFloat3(&ray.dir, vPtrDir);

    //    //エイムアシスト範囲内かどうか判定
    //    if (enemyList_.size() > 0/*&&IsAssistRange(vPlayerDir, pEnemy->GetTransform().position_,ray.distLimit)*/)
    //    {
    //        EnemyNormal* pEnemy = pPlayer_->AimAssist(&ray);
    //        if (pEnemy != nullptr)
    //        {
    //            vPtrDir = XMVector3TransformCoord(vPtrDir, LookAtMatrix(pEnemy->GetTransform().position_, vPtrDir));
    //            XMStoreFloat3(&ray.dir, vPtrDir);
    //        }
    //    }

    //    //レイキャストの始点と方向を入力
    //    Model::RayCast(ray);

    //    //当たった位置にマーカー表示
    //    if (ray.hit && !flyFlag_)
    //    {
    //        rotateSpeed_ = 2.0f;
    //        XMFLOAT3 pointerPos;
    //        XMStoreFloat3(&pointerPos, ray.hitPos);
    //        pPointer->SetPointerPos(pointerPos);
    //        pPointer->SetDraw(ray.hit);
    //    }
    //}
}

EnemyState::EnemyState(Enemy* enemy)
	:EnemyState(enemy)
{
	pEnemy_ = enemy;
}

void EnemyState::Initialize()
{

}

void EnemyState::Update()
{

}