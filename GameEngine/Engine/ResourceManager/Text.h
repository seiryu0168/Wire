#pragma once
#include"../DirectX_11/Direct2D.h"
//#include"Engine/DirectX_11/Direct3D.h"
#include<string>
typedef D2D_RECT_F TEXT_RECT;//left top right bottom
typedef D2D1_POINT_2F TEXT_POSITION;

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
	};
private:
	size_t				  textLength_;		//�e�L�X�g�̒���
	D2D1_RECT_F			  layoutRect_;		//���C�A�E�g���N�g
	wchar_t*			  pText_;			//�e�L�X�g
	ID2D1SolidColorBrush* pColorBrush_;		//�u���V	
	IDWriteFactory*		  pWriteFactory_;	//�����`��̃t�@�N�g��
	IDWriteTextFormat*	  pTextFormat_;     //�e�L�X�g�t�H�[�}�b�g
	IDWriteTextLayout*	  pLayout_;			//�e�L�X�g���C�A�E�g
public:
	TEXT_POSITION				  transform2D;		//���W
	Text();
	~Text();
	int Load(const std::string& text, const std::string& fontName, TEXT_RECT rect, STARTING_TYPE type);
	void Draw();
	void SetAlinmentType(STARTING_TYPE type);
	void SetFont(const FontData& data);
	void SetColor(XMFLOAT4 color);
	void SetTextSize(float size,UINT32 startPos,UINT32 length);
	void SetFontWeight(DWRITE_FONT_WEIGHT weightType,UINT32 startPos,UINT32 length);
	void SetText(std::string text);
	void SetTransform(TEXT_POSITION pos);
	void SetTextLayout();
	void SetRatio(float ratioX,float ratioY);
	void SetRect(TEXT_RECT rect);
	void Release();

};

