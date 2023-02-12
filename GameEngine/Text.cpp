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
	
	//�p�X�����t�@�C�����Ɗg���q�����ɂ���
	char name[_MAX_FNAME];	//�t�@�C����
	char ext[_MAX_EXT];		//�g���q
	_splitpath_s(fontName.c_str(), nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);//�t�@�C���p�X(�t���p�X)���t�@�C�����A�g���q�ɕ�����
	sprintf_s(name, "%s%s", name, ext);

	size_t ret;
	int a=mbstowcs_s(&ret, &fontName_,(size_t)FILENAME_MAX, fontName.c_str(), fontName.length());
	size_t textSize;

	//�`�悷��e�L�X�g�p�̔z���p�ӂ���
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
		//���_����
		pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;

	case LEFT_CENTE:
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

