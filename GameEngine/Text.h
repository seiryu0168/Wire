#pragma once
#include"Direct2D.h"
#include<string>
class Text
{
private:
	ID2D1SolidColorBrush* pColorBrush_;	//ブラシ	
	IDWriteFactory*		  pWriteFactory_;		//文字描画のファクトリ
	IDWriteTextFormat*	  pTextFormat_;    //テキストフォーマット
	const wchar_t*		  pWszText_;			//テキスト
	UINT32				  textLength_;
	D2D1_RECT_F			  layoutRect_;

public:	
	int Load(std::string fontName);
	void SetFont();
	void SetColor();
	void SetTransform();
	void SetRect();

};

