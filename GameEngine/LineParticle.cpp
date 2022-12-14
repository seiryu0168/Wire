#include "LineParticle.h"
#include"Engine/Direct3D.h"
#include"DirectXMath.h"
#include"Engine/Camera.h"
#include<vector>
LineParticle::LineParticle()
	:WIDTH_(0.5),
	LENGTH_(30),
	tipWidth_(0),
	pVertexBuffer_(nullptr),
	pConstantBuffer_(nullptr),
	pTexture_(nullptr)
{
	
}
LineParticle::LineParticle(float width, int length, float tipWidth)
	:WIDTH_(width),
	LENGTH_(length),
	tipWidth_(tipWidth),
	pVertexBuffer_(nullptr),
	pConstantBuffer_(nullptr),
	pTexture_(nullptr)
{
	tipWidth_ = max(tipWidth_, 0);
}

//現在位置を記憶 : pos
void LineParticle::AddPosition(XMFLOAT3 pos)
{
	//リストの先端に現在位置を入れる
	positionList_.push_front(pos);
	
	//LENGTH超えてたら終端のpositionを削除
	if (positionList_.size() > LENGTH_)
	{
		positionList_.pop_back();
	}

	//頂点バッファ解放
	SAFE_RELEASE(pVertexBuffer_);

	//カメラの位置取得(ベクトルで)
	XMFLOAT3 camPos = Camera::GetPosition();
	XMVECTOR vCamPos = XMLoadFloat3(&camPos);

	//頂点データ作成
	VERTEX* vertices = new VERTEX[LENGTH_ * 2];
	int index = 0;

	auto itr = positionList_.begin();
	for (int i = 0; i < LENGTH_; i++)
	{
		//記憶している位置取得
		XMVECTOR vPos = XMLoadFloat3(&(*itr));

		itr++;
		if (itr == positionList_.end())
		{
			break;
		}

		//さっき取得した位置から次の位置に向かうベクトル
		XMVECTOR vLine = XMLoadFloat3(&(*itr)) - vPos;

		XMVECTOR vArm = XMVector3Cross(vLine, vCamPos);
		vArm = XMVector3Normalize(vArm)*WIDTH_;

		XMFLOAT3 pos;
		XMStoreFloat3(&pos, vPos + vArm);

		VERTEX vertex1 = { pos,XMFLOAT3((float)i/LENGTH_ +tipWidth_,0,0) };

		XMStoreFloat3(&pos, vPos - vArm);

		VERTEX vertex2 = { pos,XMFLOAT3((float)i / LENGTH_+tipWidth_,1,0) };

		int s = sizeof(VERTEX);

		vertices[index] = vertex1;
		index++;
		vertices[index] = vertex2;
		index++;
	}
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * LENGTH_ * 2;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	HRESULT hr= Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	hr = Direct3D::pDevice->GetDeviceRemovedReason();
	
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ラインパーティクルのポジション更新失敗", L"エラー", MB_OK);
	}
	delete[] vertices;
}

HRESULT LineParticle::Load(std::string fileName)
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd_constant;
	bd_constant.ByteWidth = sizeof(CONSTANT_BUFFER);
	bd_constant.Usage = D3D11_USAGE_DYNAMIC;
	bd_constant.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd_constant.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd_constant.MiscFlags = 0;
	bd_constant.StructureByteStride = 0;

	hr = Direct3D::pDevice->CreateBuffer(&bd_constant, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ラインパーティクル用コンスタントバッファの作成に失敗", L"エラー", MB_OK);
		return hr;
	}
	pTexture_ = new Texture();
	wchar_t name[FILENAME_MAX];
	size_t ret;
	mbstowcs_s(&ret, name, fileName.c_str(), fileName.length());

	hr = pTexture_->Load(name);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ラインパーティクルのテクスチャのロードに失敗", L"エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

void LineParticle::Draw(XMMATRIX matW)
{
	HRESULT hr;
	Direct3D::SetShader(SHADER_EFF);
	CONSTANT_BUFFER cb;
		cb.matWVP = XMMatrixTranspose(matW*Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.color = XMFLOAT4(1, 1, 1, 1);

	D3D11_MAPPED_SUBRESOURCE pdata;

	//GPUからのデータアクセスを止める
	hr = Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"デバイスコンテキスト失敗", L"エラー", MB_OK);
	}
	//データを送る
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));
	
	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	//データアクセス再開
	Direct3D::pContext->Unmap(pConstantBuffer_, 0);

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	//頂点バッファ
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);//頂点シェーダー用
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);//ピクセルシェーダー用

	//頂点の並び方を指定
	UINT vertexCount = 0;
	if(positionList_.empty())
	{
		vertexCount = 0;
	}
	else
	{
		vertexCount = (positionList_.size() - 1) * 2;
	}

	Direct3D::pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); //設定を変える
	Direct3D::pContext->Draw(vertexCount, 0);
	Direct3D::pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void LineParticle::SetLineParameter(float width, int length, float tipWidth)
{
	WIDTH_ = width;
	LENGTH_ = length;
	tipWidth_ = tipWidth;
	tipWidth_ = max(tipWidth_, 0);
	
}

void LineParticle::DeleteLine()
{
	positionList_.clear();
}

void LineParticle::Release()
{
	SAFE_RELEASE(pTexture_);
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
	positionList_.clear();
}
