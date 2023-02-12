#pragma once
#include"Direct2D.h"
#include"Engine/Direct3D.h"
#include<string>
typedef D2D_RECT_F TEXT_RECT;//left top right bottom

class Text
{
private:
	XMINT2				  transform2D;
	size_t				  textLength_;
	D2D1_RECT_F			  layoutRect_;
	wchar_t*			  pFontName_;
	wchar_t*			  pLocale_;
	wchar_t*			  pText_;			//�e�L�X�g
	ID2D1SolidColorBrush* pColorBrush_;	//�u���V	
	IDWriteFactory*		  pWriteFactory_;		//�����`��̃t�@�N�g��
	IDWriteTextFormat*	  pTextFormat_;    //�e�L�X�g�t�H�[�}�b�g

public:
	Text();
	~Text();
	int Load(const std::string& text, const std::string& fontName, TEXT_RECT rect, STARTING_TYPE type);
	void Draw();
	void SetAlinmentType(STARTING_TYPE type);
	void SetFont();
	void SetColor();
	void SetTransform();
	void SetRect();
	void Release();

};

