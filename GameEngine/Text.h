#pragma once
#include"Direct2D.h"
#include<string>
class Text
{
private:
	ID2D1SolidColorBrush* pColorBrush_;	//�u���V	
	IDWriteFactory*		  pWriteFactory_;		//�����`��̃t�@�N�g��
	IDWriteTextFormat*	  pTextFormat_;    //�e�L�X�g�t�H�[�}�b�g
	const wchar_t*		  pWszText_;			//�e�L�X�g
	UINT32				  textLength_;
	D2D1_RECT_F			  layoutRect_;

public:	
	int Load(std::string fontName);
	void SetFont();
	void SetColor();
	void SetTransform();
	void SetRect();

};

