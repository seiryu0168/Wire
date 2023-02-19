#pragma once
#include<DirectXMath.h>
#include"Fbx.h"
using namespace DirectX;

namespace Math
{
	float Det(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c);
	/// <summary>
	/// �����Ɩʂ̔���
	/// </summary>
	/// <param name="start">�����̊J�n�ʒu</param>
	/// <param name="dir">�����̕���</param>
	/// <param name="v0">�ʂ��\�����钸�_1</param>
	/// <param name="v1">�ʂ��\�����钸�_2</param>
	/// <param name="v2">�ʂ��\�����钸�_3</param>
	/// <param name="dist">�J�n�ʒu����̋���</param>
	/// <param name="hitPos">�����Ƃ̏Փˈʒu(���[�J�����W)</param>
	/// <returns>�����������ǂ���</returns>
	bool Intersect(XMFLOAT3 start, XMFLOAT3 dir, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2,float& dist, XMVECTOR& hitPos);
	
	/// <summary>
	/// �����Ɩʂ̔���
	/// </summary>
	/// <param name="rayCast">���C�̃f�[�^</param>
	/// <param name="v0">�ʂ��\�����钸�_1</param>
	/// <param name="v1">�ʂ��\�����钸�_2</param>
	/// <param name="v2">�ʂ��\�����钸�_3</param>
	/// <returns>�����������ǂ���</returns>
	bool Intersect(RayCastData& rayCast, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2);
	/// <summary>
	/// �@���Ƃ̊p�x�𒲂ׂ�
	/// </summary>
	/// <param name="vNormal">���ׂ����@��</param>
	/// <param name="vDir">����</param>
	/// <returns>�\���ǂ���</returns>
	bool IsFrontSurface(XMVECTOR vNormal, XMVECTOR vDir);
}

