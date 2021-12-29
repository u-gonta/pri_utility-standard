#include "pch.h"
#include "CButtonExpand.h"

BEGIN_MESSAGE_MAP(CButtonExpand, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()


void CButtonExpand::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	CRect window;

	this->GetWindowRect(window);
	this->GetParent()->SendMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(window.left + point.x, window.top + point.y));

	CButton::OnLButtonDown(nFlags, point);
}


void CButtonExpand::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	CRect window;

	this->GetWindowRect(window);
	this->GetParent()->SendMessage(WM_LBUTTONUP, 0, MAKELPARAM(window.left + point.x, window.top + point.y));

	CButton::OnLButtonUp(nFlags, point);
}


void CButtonExpand::OnMouseLeave()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	this->GetParent()->SendMessage(WM_MOUSELEAVE, 0, 0);

	CButton::OnMouseLeave();
}
