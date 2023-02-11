#pragma once
#include"Direct2D.h"
#include"Engine/Direct3D.h"
#include<string>
typedef D2D_RECT_F TEXT_RECT;

class Text
{
private:
	wchar_t				  fontName_;
	wchar_t				  locale_;
	XMINT2				  transform2D;
	ID2D1SolidColorBrush* pColorBrush_;	//�u���V	
	IDWriteFactory*		  pWriteFactory_;		//�����`��̃t�@�N�g��
	IDWriteTextFormat*	  pTextFormat_;    //�e�L�X�g�t�H�[�}�b�g
	wchar_t*			  pText_;			//�e�L�X�g
	UINT32				  textLength_;
	D2D1_RECT_F			  layoutRect_;

public:
	Text();
	~Text();
	int Load(std::string text, std::string fontName, TEXT_RECT rect);
	void Draw();
	void SetFont();
	void SetColor();
	void SetTransform();
	void SetRect();
	void Release();

};

