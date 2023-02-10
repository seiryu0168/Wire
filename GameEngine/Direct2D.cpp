#include "Direct2D.h"
#include"Engine/Direct3D.h"
#include"Engine/SAFE_DELETE_RELEASE.h"
namespace D2D
{
	ID2D1Factory*		   pFactory_	  = nullptr;	//�t�@�N�g��
	ID2D1RenderTarget* pRenderTarget_ = nullptr;    //�����_�[�^�[�Q�b�g
	IDXGIDevice*		   pDxgiDevice_   = nullptr;	//
	ID2D1SolidColorBrush*  pColorBrush_   = nullptr;	//�u���V	
	IDWriteFactory*		   pWriteFactory_ = nullptr;	//�����`��̃t�@�N�g��
	IDWriteTextFormat*	   pTextFormat_   = nullptr;	//�e�L�X�g�t�H�[�}�b�g
	const wchar_t*		   pWszText       = nullptr;	//�e�L�X�g
	UINT32				   textLength_	  = 0;
	D2D1_RECT_F			   layoutRect_;

}

HRESULT D2D::Initialize(int winW, int winH, HWND hWnd)
{
	HRESULT hr;
	hr= D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"�t�@�N�g���̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	IDXGISurface* pBackBuffer;
	Direct3D::GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(IDWriteFactory),reinterpret_cast<IUnknown**>(&pWriteFactory_));
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct2D : �e�L�X�g�`��p�t�@�N�g���̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	pWszText = L"Hello World!";
	textLength_ = (UINT32)wcslen(pWszText);

	hr = pWriteFactory_->CreateTextFormat(L"Gabliora", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"en-us", &pTextFormat_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct2D : �e�L�X�g�t�H���g�쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	hr = pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct2D : �A���C�����g�ݒ�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	hr = pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct2D : �A���C�����g�ݒ�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	RECT rect;
	GetClientRect(hWnd, &rect);
	
	float dpiScaleX_ = GetDpiForWindow(hWnd);
	float dpiScaleY_ = GetDpiForWindow(hWnd);
	D2D1_SIZE_U size = D2D1::Size<UINT>(rect.right, rect.bottom);
	D2D1_RENDER_TARGET_PROPERTIES prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiScaleX_, dpiScaleY_);
	hr = pFactory_->CreateDxgiSurfaceRenderTarget(pBackBuffer,prop , &pRenderTarget_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct2D : �����_�[�^�[�Q�b�g�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	hr = pRenderTarget_->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White),&pColorBrush_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct2D : �u���V�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}


	layoutRect_ = D2D1::RectF(static_cast<FLOAT>(rect.left) / dpiScaleX_,
										 static_cast<FLOAT>(rect.top) / dpiScaleY_,
										 static_cast<FLOAT>(rect.right - rect.left) / dpiScaleX_,
										 static_cast<FLOAT>(rect.bottom - rect.top) / dpiScaleY_);



	return S_OK;
}

void D2D::Release()
{
	SAFE_RELEASE(pFactory_);
	SAFE_RELEASE(pRenderTarget_);
	SAFE_RELEASE(pColorBrush_);
}

void D2D::RenderTest()
{
	HRESULT hr;
	ID2D1SolidColorBrush* pGreenBrush=nullptr;
	
	hr = pRenderTarget_->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0.0f), &pGreenBrush);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct2D : �e�X�g���s", L"�G���[", MB_OK);
		//return hr;
	}
	pRenderTarget_->BeginDraw();
	Drawtext();
	//D2D1_ELLIPSE ell = D2D1::Ellipse(D2D1::Point2F(1120.0f, 120.0f), 100.0f, 100.0f);
	//pRenderTarget_->DrawEllipse(ell, pGreenBrush, 10.0f);

	pRenderTarget_->EndDraw();


	SAFE_RELEASE(pGreenBrush);
}

void D2D::Drawtext()
{
	pRenderTarget_->SetTransform(D2D1::IdentityMatrix());	
	pRenderTarget_->DrawText(pWszText, textLength_, pTextFormat_, layoutRect_, pColorBrush_);
}
