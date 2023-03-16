#include "Text.h"



Text::Text()
{
	textLength_	   = 0;
	transform2D = { 0,0 };
	pText_		   = nullptr;
	pColorBrush_   = nullptr;
	pWriteFactory_ = nullptr;
	pTextFormat_   = nullptr;

}
Text::~Text()
{
	Release();	
}

void Text::Release()
{
	SAFE_RELEASE(pTextFormat_);
	SAFE_RELEASE(pWriteFactory_);
	SAFE_RELEASE(pColorBrush_);
	SAFE_DELETE(pText_);
}

int Text::Load(const std::string& text, const std::string& fontName, TEXT_RECT rect,STARTING_TYPE type)
{
	
	////�p�X�����t�@�C�����Ɗg���q�����ɂ���
	//char name[_MAX_FNAME];	//�t�@�C����
	//char ext[_MAX_EXT];		//�g���q
	//_splitpath_s(fontName.c_str(), nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);//�t�@�C���p�X(�t���p�X)���t�@�C�����A�g���q�ɕ�����
	//sprintf_s(name, "%s%s", name, ext);

	//�t�H���g���p�̔z��p��
	size_t ret;
	FontData data;
	data.pFontName_ = new wchar_t[fontName.length()+1];
	int a=mbstowcs_s(&ret, data.pFontName_, fontName.length() + 1, fontName.c_str(), fontName.length());
	size_t textSize;

	//�`�悷��e�L�X�g�p�̔z���p�ӂ���
	textLength_ = text.length()+1;
	pText_ = new wchar_t[textLength_];
	mbstowcs_s(&textSize, pText_, textLength_, text.c_str(), text.length());
	
	HRESULT hr=DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory_));
	data.pFontName_ = (wchar_t*)L"Meiryo";
	data.pLocale_ = (wchar_t*)L"en-us";

	SetFont(data);
	//pWriteFactory_->CreateTextFormat(pFontName_, NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,72.0f, L"ja-jp", &pTextFormat_);
	SetAlinmentType(type);
	D2D::GetRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pColorBrush_);
	layoutRect_ = rect;
	pWriteFactory_->CreateTextLayout(pText_, textLength_, pTextFormat_, (layoutRect_.right - layoutRect_.left), (layoutRect_.bottom - layoutRect_.top), &pLayout_);


	return 0;
}

void Text::Draw()
{
	//D2D1_RECT_F rect = layoutRect_;
	//rect.left += transform2D.x;
	//rect.top += transform2D.y;
	D2D::GetRenderTarget()->DrawTextLayout(transform2D, pLayout_, pColorBrush_);
	/*D2D::GetRenderTarget()->DrawText(pText_, textLength_, pTextFormat_,
								    { transform2D.x + layoutRect_.left,
									  transform2D.y + layoutRect_.top,
									  transform2D.x + layoutRect_.right,
									  transform2D.y + layoutRect_.bottom }, pColorBrush_);*/
}
void Text::SetColor()
{
	D2D::GetRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pColorBrush_);
}
void Text::SetRatio(float ratioX, float ratioY)
{
	transform2D.x = Direct3D::GetScreenWidth() * ratioX;
	transform2D.y = Direct3D::GetScreenHeight() * ratioY;
}

void Text::SetTextLayout()
{
	//pWriteFactory_->CreateTextLayout()
}

void Text::SetFontWeight(DWRITE_FONT_WEIGHT weightType, UINT32 startPos, UINT32 length)
{
	pLayout_->SetFontWeight(weightType, { startPos,length });
}

void Text::SetTextSize(float size, UINT32 startPos, UINT32 length)
{
	if (startPos + length >= textLength_)
	{
		length = startPos + length - textLength_;
	}
	pLayout_->SetFontSize(size, { startPos,length });
}

void Text::SetFont(const FontData& data)
{
	pWriteFactory_->CreateTextFormat(data.pFontName_, data.pCollection_, data.fontWaight_/*DWRITE_FONT_WEIGHT_REGULAR*/, data.fontStyle_/*DWRITE_FONT_STYLE_NORMAL*/, data.fontStretch_/*DWRITE_FONT_STRETCH_NORMAL*/, data.fontSize_, data.pLocale_, &pTextFormat_);
}
void Text::SetTransform(TEXT_POSITION pos)
{
	transform2D = pos;
}

void Text::SetRect(TEXT_RECT rect)
{	
	layoutRect_ = rect;
}
void Text::SetAlinmentType(STARTING_TYPE type)
{

	switch (type)
	{
	case LEFT_TOP:
		//���_����
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;

	case LEFT_CENTER:
		//���_��
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	
	case LEFT_BOTTOM:
		//���_����
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;

	case CENTER_TOP:
		//���_��
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	
	case CENTER_CENTER:
		//���_����
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	
	case CENTER_BOTTOM:
		//���_��
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;

	case RIGHT_TOP:
		//���_�E��
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case RIGHT_CENTER:
		//���_�E
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;

	case RIGHT_BOTTOM:
		//���_�E��
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;

	default:
		//�f�t�H���g
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	}
}

