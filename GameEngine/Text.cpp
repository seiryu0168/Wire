#include "Text.h"


int Text::Load(std::string fontName)
{
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory_));
	
	//パス名をファイル名と拡張子だけにする
	char name[_MAX_FNAME];	//ファイル名
	char ext[_MAX_EXT];		//拡張子
	_splitpath_s(fontName.c_str(), nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);//ファイルパス(フルパス)をファイル名、拡張子に分ける
	sprintf_s(name, "%s%s", name, ext);

	wchar_t wtext[FILENAME_MAX];
	size_t ret;
	mbstowcs_s(&ret, wtext, fontName.c_str(), fontName.length());
	pWriteFactory_->CreateTextFormat(wtext, NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,72.0f, L"en-us", &pTextFormat_);
	pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

}
void Text::SetColor()
{
}
void Text::SetFont()
{

}
void Text::SetTransform()
{
}

void Text::SetRect()
{
}
