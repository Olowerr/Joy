#include "UIElement.h"

UIElement::UIElement(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size)
	:pos(pos), size(size)
{
	DirectX::XMFLOAT2 data[2] = { pos,size };
	Backend::CreateConstCBuffer(&transformBuffer, data, 16);
}

bool UIElement::Hovered()
{
	const DirectX::XMUINT2 mPos(Backend::GetMouse().GetXPos(), Backend::GetMouse().GetYPos());
	return mPos.x > pos.x && mPos.x < pos.x + size.x
		&& mPos.y > pos.x && mPos.y < pos.y + size.y;
}

bool UIElement::Clicked()
{
	return Hovered() && Backend::GetMouse().LeftReleased();
}

void UIElement::Draw()
{
	Backend::GetDeviceContext()->VSSetConstantBuffers(0, 1, &transformBuffer);
	BindResource();

	Backend::GetDeviceContext()->Draw()
}
