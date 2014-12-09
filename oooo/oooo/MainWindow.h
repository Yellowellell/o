#pragma once

#include "MainFunctions.h"
#include <afxwin.h>

#define KILL_BUTTON_ID					1

#define KILL_PROCESS__MENU_ITEM_ID		10
#define RESTART_PROCESS__MENU_ITEM_ID	11
#define NEW_PROCESS__MENU_ITEM_ID		12
#define TERMINATE__MENU_ITEM_ID			13
#define ABOUT__MENU_ITEM_ID				14

class MainWindow : public CFrameWnd
{
public:
	CButton		MyButton;
	CImageList	ProcessImageList;
	CListCtrl	ProcessList;
	CMenu		MainMenu;

	MainWindow();
	~MainWindow();

protected:
	int						ProcessesCount;
	ProcessInformation*		Processes;

	void FillProcessList();
	void InitInstance();
	void FreeInstance();

protected:
	afx_msg void OnChar(UINT Char, UINT, UINT);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT);
	afx_msg void KillProcessClick();
	afx_msg void TerminateProgramClick();
	afx_msg void StartNewProcessClick();
	DECLARE_MESSAGE_MAP();
};