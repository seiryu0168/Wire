#include "Direct2D.h"
#include"Engine/SAFE_DELETE_RELEASE.h"
namespace D2D
{
	ID2D1Factory* pFactory_ = nullptr;					//ファクトリ
	ID2D1HwndRenderTarget* pRenderTarget_ = nullptr;

}
HRESULT D2D::Initialize(int winW, int winH, HWND hWnd)
{
	HRESULT hr;
	hr= D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ファクトリの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	RECT rect;
	GetClientRect(hWnd, &rect);

	D2D1_SIZE_U size = D2D1::Size<UINT>(rect.right, rect.bottom);

	hr = pFactory_->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),D2D1::HwndRenderTargetProperties(hWnd,size),&pRenderTarget_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct2D : レンダーターゲットの作成に失敗", L"エラー", MB_OK);
		return hr;
	}
	return S_OK;
}



void D2D::Release()
{
	SAFE_RELEASE(pFactory_);
	SAFE_RELEASE(pRenderTarget_);
}

void D2D::RenderTest()
{
	HRESULT hr;
	ID2D1SolidColorBrush* pGreenBrush=nullptr;

	hr = pRenderTarget_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0.0f), &pGreenBrush);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct2D : テスト失敗", L"エラー", MB_OK);
		//return hr;
	}
	pRenderTarget_->BeginDraw();

	D2D1_ELLIPSE ell = D2D1::Ellipse(D2D1::Point2F(120.0f, 120.0f), 100.0f, 100.0f);
	pRenderTarget_->DrawEllipse(ell, pGreenBrush, 10.0f);

	pRenderTarget_->EndDraw();


	SAFE_RELEASE(pGreenBrush);
}
