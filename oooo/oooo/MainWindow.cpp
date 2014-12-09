#include "stdafx.h"
#include "MainWindow.h"
#include "Application.h"
#include <string.h>

//........................Application.............................//

Application App;
BOOL Application::InitInstance()
{
	m_pMainWnd = new MainWindow();
	m_pMainWnd->ShowWindow(SW_RESTORE);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

//........................MainWindow.............................//

MainWindow::MainWindow()
{
	this->Create(NULL, L"HelloWorld!!!");
	this->Processes = new ProcessInformation[100]();
	this->InitInstance();
}
MainWindow::~MainWindow()
{
	delete[] this->Processes;
	this->FreeInstance();
}

void MainWindow::InitInstance()
{
	TimerDelay = TIMER_DELAY_NORMAL;
	this->SetTimer(1, TimerDelay, NULL);
	this->MyButton.Create(L"Kill him!!!", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
						  CRect(10, 40, 100, 100), this, KILL_BUTTON_ID);

	this->ProcessImageList.Create(24, 24, ILC_COLOR32, 2, 2);
	this->ProcessList.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT, CRect(10, 100, 800, 450), this, 100);
	this->ProcessList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	
	this->ProcessList.SetImageList(&(this->ProcessImageList), LVSIL_SMALL);
	this->ProcessList.InsertColumn(0, L"Имя процесса", 0, 200);
	this->ProcessList.InsertColumn(1, L"Id процесса", 0, 100);
	this->ProcessList.InsertColumn(2, L"Загрузка ядра", 0, 100);

	this->MainMenu.CreateMenu();

	CMenu* FileSubMenu = new CMenu();
	FileSubMenu->CreatePopupMenu();
	FileSubMenu->AppendMenuW(MF_STRING, ABOUT__MENU_ITEM_ID, L"О программе");
	FileSubMenu->AppendMenuW(MF_STRING, TERMINATE__MENU_ITEM_ID, L"Выход");

	CMenu* ProcessSubMenu = new CMenu();
	ProcessSubMenu->CreatePopupMenu();
	ProcessSubMenu->AppendMenuW(MF_STRING, KILL_PROCESS__MENU_ITEM_ID, L"Завершить");
	ProcessSubMenu->AppendMenuW(MF_STRING, RESTART_PROCESS__MENU_ITEM_ID, L"Перезапустить");
	ProcessSubMenu->AppendMenuW(MF_STRING, NEW_PROCESS__MENU_ITEM_ID, L"Новый процесс");
	
	this->MainMenu.AppendMenuW(MF_STRING | MF_POPUP, (UINT_PTR)FileSubMenu->m_hMenu, L"Файл");
	this->MainMenu.AppendMenuW(MF_STRING | MF_POPUP | MF_ENABLED, (UINT_PTR)ProcessSubMenu->m_hMenu, L"Процесс");
	this->SetMenu(&this->MainMenu);

	this->FillProcessList();
	this->FillProcessList();
	this->FillProcessList();
}
void MainWindow::FreeInstance()
{
	this->ProcessImageList.DeleteImageList();
}
void MainWindow::FillProcessList()
{
	this->ProcessImageList.DeleteImageList();
	this->ProcessImageList.Create(24, 24, ILC_COLOR32, 0, 0);
	GetProcessInformation(this->ProcessesCount, this->Processes);
	for (int i = 0; i < this->ProcessesCount; i++)
	{
		WORD IconIndex = 0;
		HICON hIcon;
		if (Processes[i].FileName == L"")
			hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
		else
			hIcon = ExtractAssociatedIcon(App.m_hInstance, Processes[i].FileName.GetBuffer(), &(IconIndex));
		this->ProcessImageList.Add(hIcon);
		DestroyIcon(hIcon);
	}
	
	for (int i = 0; i < this->ProcessesCount; i++)
	{
		if (this->ProcessList.GetItemCount() < i + 1)
			this->ProcessList.InsertItem(i, L"", i);
		
		CString IdString;
		IdString.Format(L"%d", this->Processes[i].Id);
		CString KernelLoadString;
		KernelLoadString.Format(L"%.1f%c", this->Processes[i].KernelLoad, '%');

		this->ProcessList.SetItemText(i, 0, this->Processes[i].Name);
		this->ProcessList.SetItemText(i, 1, IdString.GetBuffer());
		this->ProcessList.SetItemText(i, 2, KernelLoadString.GetBuffer());
	}

	while (this->ProcessList.GetItemCount() > ProcessesCount)
		this->ProcessList.DeleteItem(this->ProcessList.GetItemCount() - 1);
}