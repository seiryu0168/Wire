#include "Text.h"


Text::Text()
{
	fontName_ = wchar_t(FILENAME_MAX);
	textLength_	   = 0;
	pColorBrush_   = nullptr;
	pWriteFactory_ = nullptr;
	pTextFormat_   = nullptr;
	pText_		   = nullptr;
}
Text::~Text()
{
	Release();	
}

void Text::Release()
{
	SAFE_DELETE(pText_);
	SAFE_RELEASE(pTextFormat_);
	SAFE_RELEASE(pWriteFactory_);
	SAFE_RELEASE(pColorBrush_);
}

int Text::Load(const std::string& text, const std::string& fontName, TEXT_RECT rect,STARTING_TYPE type)
{
	
	//パス名をファイル名と拡張子だけにする
	char name[_MAX_FNAME];	//ファイル名
	char ext[_MAX_EXT];		//拡張子
	_splitpath_s(fontName.c_str(), nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);//ファイルパス(フルパス)をファイル名、拡張子に分ける
	sprintf_s(name, "%s%s", name, ext);

	size_t ret;
	int a=mbstowcs_s(&ret, &fontName_,(size_t)FILENAME_MAX, fontName.c_str(), fontName.length());
	size_t textSize;

	//描画するテキスト用の配列を用意する
	textLength_ = text.length()+1;
	pText_ = new wchar_t[textLength_];
	mbstowcs_s(&textSize, pText_, textLength_, text.c_str(), text.length());
	
	HRESULT hr=DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory_));
	pWriteFactory_->CreateTextFormat(&fontName_, NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,72.0f, L"en-us", &pTextFormat_);
	SetAlinmentType(type);
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
void Text::SetAlinmentType(STARTING_TYPE type)
{

	switch (type)
	{
	case LEFT_TOP:
		//原点左上
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;

	case LEFT_CENTE:
		//原点左
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	
	case LEFT_BOTTOM:
		//原点左下
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;

	case CENTER_TOP:
		//原点上
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	
	case CENTER_CENTER:
		//原点中央
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	
	case CENTER_BOTTOM:
		//原点下
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;

	case RIGHT_TOP:
		//原点右上
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case RIGHT_CENTER:
		//原点右
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;

	case RIGHT_BOTTOM:
		//原点右下
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;

	default:
		//デフォルト
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	}
}

