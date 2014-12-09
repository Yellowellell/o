#include "stdafx.h"
#include "MainWindow.h"
#include "Application.h"

afx_msg void MainWindow::OnChar(UINT Char, UINT, UINT)	
{
	this->InvalidateRect(NULL);
}
afx_msg void MainWindow::OnPaint()						
{
	CPaintDC dc(this);
	dc.TextOutW(100, 100, L"  ", 50);
	//dc.TextOutW(100, 100, MainWindowTitle);
}
afx_msg void MainWindow::OnTimer(UINT ID)
{
	this->FillProcessList();
}
afx_msg void MainWindow::KillProcessClick()
{
	int i = this->ProcessList.GetSelectionMark();
	if (i == -1)
		return;

	KillProcess(Processes[i].Id);
}
afx_msg void MainWindow::TerminateProgramClick()
{
	this->CloseWindow();
	delete this;
}
afx_msg void MainWindow::StartNewProcessClick()
{

}

BEGIN_MESSAGE_MAP(MainWindow, CFrameWnd)
	ON_WM_CHAR		()
	ON_WM_PAINT		()
	ON_WM_TIMER		()
	ON_BN_CLICKED	(KILL_BUTTON_ID,				KillProcessClick)
	ON_COMMAND		(KILL_PROCESS__MENU_ITEM_ID,	KillProcessClick)
	ON_COMMAND		(NEW_PROCESS__MENU_ITEM_ID,		StartNewProcessClick)
	ON_COMMAND		(TERMINATE__MENU_ITEM_ID,		TerminateProgramClick)
END_MESSAGE_MAP()
