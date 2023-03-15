#pragma once
#include"Engine/DirectX_11/Direct2D.h"
//#include"Engine/DirectX_11/Direct3D.h"
#include<string>
typedef D2D_RECT_F TEXT_RECT;//left top right bottom

class Text
{
	struct FontData
	{
		float fontSize_;
		wchar_t* pFontName_;		//�t�H���g��
		wchar_t* pLocale_;			//���P�[��
		DWRITE_FONT_WEIGHT	fontWaight_;		//�t�H���g�̑���
		DWRITE_FONT_STYLE   fontStyle_;			//�t�H���g�X�^�C��
		DWRITE_FONT_STRETCH fontStretch_;
		IDWriteFontCollection* pCollection_;	//�t�H���g�R���N�V����
		FontData()
		{
			fontSize_ = 72.0f;
			pFontName_ = (wchar_t*)L"Gabliora";
			pLocale_ = (wchar_t*)L"en-us";
			fontWaight_ = DWRITE_FONT_WEIGHT_REGULAR;
			fontStyle_ = DWRITE_FONT_STYLE_NORMAL;
			fontStretch_ = DWRITE_FONT_STRETCH_NORMAL;
			pCollection_ = nullptr;
		}

		~FontData()
		{
			if (pCollection_ != nullptr)
			{
				pCollection_->Release();
				SAFE_DELETE(pCollection_);
			}

			SAFE_DELETE(pFontName_);
			//SAFE_DELETE(pLocale_);
		}
	};
private:
	XMINT2				  transform2D;		//���W
	size_t				  textLength_;		//�e�L�X�g�̒���
	D2D1_RECT_F			  layoutRect_;		//���C�A�E�g���N�g
	wchar_t*			  pText_;			//�e�L�X�g
	ID2D1SolidColorBrush* pColorBrush_;		//�u���V	
	IDWriteFactory*		  pWriteFactory_;	//�����`��̃t�@�N�g��
	IDWriteTextFormat*	  pTextFormat_;     //�e�L�X�g�t�H�[�}�b�g

public:
	Text();
	~Text();
	int Load(const std::string& text, const std::string& fontName, TEXT_RECT rect, STARTING_TYPE type);
	void Draw();
	void SetAlinmentType(STARTING_TYPE type);
	void SetFont(const FontData& data);
	void SetColor();
	void SetTransform();
	void SetRect();
	void Release();

};

