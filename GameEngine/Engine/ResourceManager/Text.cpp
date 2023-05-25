#include "Text.h"
#include<locale.h>



Text::Text()
{
	textLength_	   = 0;
	transform2D = { 0,0 };
	pText_		   = nullptr;
	pColorBrush_   = nullptr;
	pWriteFactory_ = nullptr;
	pTextFormat_   = nullptr;
	pLayout_	   = nullptr;

}
Text::~Text()
{
	Release();	
}

int Text::GetTextSize()
{
	return pLayout_->GetFontSize();
}

void Text::Release()
{
	SAFE_RELEASE(pTextFormat_);
	SAFE_RELEASE(pWriteFactory_);
	SAFE_RELEASE(pColorBrush_);
}

int Text::Load(const std::string& text, const std::string& fontName, TEXT_RECT rect,STARTING_TYPE type)
{
	//�t�H���g���p�̔z��p��
	size_t ret;
	FontData data;
	pFontName_ = new wchar_t[fontName.length() + 1];
	data.pFontName_ = pFontName_;
	int a=mbstowcs_s(&ret, data.pFontName_, fontName.length() + 1, fontName.c_str(), fontName.length());
	size_t textSize;

	//�`�悷��e�L�X�g�p�̔z���p�ӂ���
	textLength_ = text.length()+1;
	pText_ = new wchar_t[textLength_];

	//���݂̃��P�[���擾
	std::string locale= setlocale(LC_CTYPE, NULL);
	
	//���P�[������{��ɕύX
	setlocale(LC_CTYPE, "ja-jp");
	//�`�悷��e�L�X�g��string����wstring�ɕϊ�
	mbstowcs_s(&textSize, pText_, textLength_, text.c_str(), text.length());
	//���P�[�������ɖ߂�
	setlocale(LC_CTYPE, locale.c_str());
	//�ϊ�����������
	textLength_ = textSize;

	HRESULT hr=DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory_));
	//�e�L�X�g�t�H�[�}�b�g�Ƀt�H���g��ݒ�
	hr=SetFont(data);
	assert(hr==S_OK);
	//pWriteFactory_->CreateTextFormat(pFontName_, NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,72.0f, L"ja-jp", &pTextFormat_);
	//�A���C�����g�ݒ�
	SetAlinmentType(type);
	//�`��̂��߂̃u���V�쐬
	D2D::GetRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pColorBrush_);
	//�e�L�X�g�\���̗̈�ݒ�
	layoutRect_ = rect;
	
	//�e�L�X�g���C�A�E�g�쐬
	hr=pWriteFactory_->CreateTextLayout(pText_, textLength_, pTextFormat_, (layoutRect_.right - layoutRect_.left), (layoutRect_.bottom - layoutRect_.top), &pLayout_);
	assert(hr==S_OK);


	return 0;
}

void Text::Draw()
{
	
	D2D::GetRenderTarget()->BeginDraw();
	D2D::GetRenderTarget()->DrawTextLayout(transform2D, pLayout_, pColorBrush_);
	//D2D::GetRenderTarget()->DrawText(pText_, textLength_, pTextFormat_,
	//							    { transform2D.x + layoutRect_.left,
	//								  transform2D.y + layoutRect_.top,
	//								  transform2D.x + layoutRect_.right,
	//								  transform2D.y + layoutRect_.bottom }, pColorBrush_);
	D2D::GetRenderTarget()->EndDraw();
}
void Text::SetColor(XMFLOAT4 color)
{
	D2D1_COLOR_F colorF = { color.x, color.y,color.z,color.w };
	pColorBrush_->SetColor(colorF);
}
void Text::SetRatio(float ratioX, float ratioY)
{
	transform2D.x = Direct3D::GetScreenWidth() * ratioX;
	transform2D.y = Direct3D::GetScreenHeight() * ratioY;
}

void Text::SetPosition(XMFLOAT2 position)
{
	transform2D.x = 0.5f*position.x+0.5f*Direct3D::GetScreenWidth();
	transform2D.y = -(0.5f * position.y)+ 0.5f * Direct3D::GetScreenHeight();
}

void Text::SetTextLayout()
{
	//pWriteFactory_->CreateTextLayout()
}

HRESULT Text::SetText(std::string text)
{
	size_t textSize;
	textLength_ = text.length() + 1;
	pText_ = new wchar_t[textLength_];

	std::string locale = setlocale(LC_CTYPE, NULL);
	setlocale(LC_CTYPE, "ja-jp");
	mbstowcs_s(&textSize, pText_, textLength_, text.c_str(), text.length());
	setlocale(LC_CTYPE, locale.c_str());
	textLength_ = textSize;
	return pWriteFactory_->CreateTextLayout(pText_, textLength_, pTextFormat_, (layoutRect_.right - layoutRect_.left), (layoutRect_.bottom - layoutRect_.top), &pLayout_);
}

HRESULT Text::SetFontWeight(DWRITE_FONT_WEIGHT weightType, UINT32 startPos, UINT32 length)
{
	return pLayout_->SetFontWeight(weightType, { startPos,length });
}

HRESULT Text::SetTextSize(float size, UINT32 startPos, UINT32 length)
{
	if (startPos + length >= textLength_)
	{
		length = startPos + length - textLength_;
	}
	FontData data;
	data.fontSize_ = size;
	data.pFontName_ = pFontName_;
	size_t size = 0;
	pTextFormat_->GetLocaleName(data.pLocale_,size);
	pWriteFactory_->CreateTextFormat(data.pFontName_, data.pCollection_, data.fontWaight_, data.fontStyle_, data.fontStretch_, data.fontSize_, data.pLocale_, &pTextFormat_);
	return pLayout_->SetFontSize(size, { startPos,length });
}

HRESULT Text::SetFont(const FontData& data)
{
	HRESULT hr= pWriteFactory_->CreateTextFormat(data.pFontName_, data.pCollection_, data.fontWaight_/*DWRITE_FONT_WEIGHT_REGULAR*/, data.fontStyle_/*DWRITE_FONT_STYLE_NORMAL*/, data.fontStretch_/*DWRITE_FONT_STRETCH_NORMAL*/, data.fontSize_, data.pLocale_, &pTextFormat_);
	assert(hr==S_OK);
	return hr;
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

