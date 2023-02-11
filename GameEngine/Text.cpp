#include "Text.h"


Text::Text()
{
	fontName_ = wchar_t(FILENAME_MAX);
}

void Text::Release()
{
	SAFE_DELETE(pText_);
}

int Text::Load(std::string text,std::string fontName,TEXT_RECT rect)
{
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory_));
	
	//�p�X�����t�@�C�����Ɗg���q�����ɂ���
	char name[_MAX_FNAME];	//�t�@�C����
	char ext[_MAX_EXT];		//�g���q
	_splitpath_s(fontName.c_str(), nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);//�t�@�C���p�X(�t���p�X)���t�@�C�����A�g���q�ɕ�����
	sprintf_s(name, "%s%s", name, ext);

	size_t ret;
	mbstowcs_s(&ret, &fontName_,(size_t)FILENAME_MAX, fontName.c_str(), fontName.length());
	size_t textSize;
	mbstowcs_s(&textSize, pText_,text.length(), text.c_str(), text.length());
	
	pWriteFactory_->CreateTextFormat(&fontName_, NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,72.0f, L"en-us", &pTextFormat_);
	pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	D2D::GetRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pColorBrush_);
	layoutRect_ = rect;

	return -1;
}
void Text::Draw()
{
	D2D::GetRenderTarget()->DrawText(pText_, textLength_, pTextFormat_,layoutRect_, pColorBrush_);
}
void Text::SetColor()
{
	D2D::GetRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pColorBrush_);
}
void Text::SetFont()
{
	pWriteFactory_->CreateTextFormat(&fontName_, NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"en-us", &pTextFormat_);
}
void Text::SetTransform()
{
}

void Text::SetRect()
{
}

