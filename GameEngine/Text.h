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
	ID2D1SolidColorBrush* pColorBrush_;	//ブラシ	
	IDWriteFactory*		  pWriteFactory_;		//文字描画のファクトリ
	IDWriteTextFormat*	  pTextFormat_;    //テキストフォーマット
	wchar_t*			  pText_;			//テキスト
	size_t				  textLength_;
	D2D1_RECT_F			  layoutRect_;

public:
	Text();
	~Text();
	int Load(const std::string& text, const std::string& fontName, TEXT_RECT rect, STARTING_TYPE type);
	void Draw();
	void SetAlinmentType(STARTING_TYPE);
	void SetFont();
	void SetColor();
	void SetTransform();
	void SetRect();
	void Release();

};

