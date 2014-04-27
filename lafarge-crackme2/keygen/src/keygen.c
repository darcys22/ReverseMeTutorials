/*******************************************************************
 *
 *	Keygen Template by Cyclops
 *	http://cyclops.ueuo.com
 *
 *	-Mera Bharat Mahan
 ********************************************************************/
#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#include <Winuser.h>
#include "resource.h"

#include "music.c"

#include "crc.c"

#define WIN32_LEAN_AND_MEAN

#define MAX_NAME	32
#define MAX_SERIAL	50
#define AUTHOR		"AAAA"
#define APPINFO		"Keygen template butchered by Aimless Wanderer"

//Lib files needed for our keygen
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "minifmod.lib")
#pragma comment(lib, "winmm.lib")

//Global variables 
HINSTANCE	hInst;

//Bitmap holders for buttons
HBITMAP	hbmAbout;
HBITMAP hbmAboutDown;
HBITMAP hbmGenerate;
HBITMAP hbmGenerateDown;
HBITMAP hbmClose;
HBITMAP hbmCloseDown;
HBITMAP hbmCross;
HBITMAP hbmCrossDown;
HBITMAP hbmMin;
HBITMAP hbmMinDown;

//Button handles
HWND hAbout;
HWND hGenerate;
HWND hClose;
HWND hCross;
HWND hMin;

//Chip Music stuff
FMUSIC_MODULE *mod=NULL;

//typedef for functions
typedef DWORD (WINAPI *pLayer)(HWND, DWORD, BYTE, DWORD);
typedef DWORD (WINAPI *pAnime)(HWND, DWORD, DWORD);

//Declaration of our Serial Generating routine
void GenSerial();

//Make form transaparent using   "SetLayeredWindowAttributes"
void SetTransparency(HWND hWnd)
{
	pLayer SetLayer;
	LONG wAttr;

	SetLayer = (pLayer)GetProcAddress(LoadLibrary("user32.dll"), "SetLayeredWindowAttributes");

	wAttr  = GetWindowLong(hWnd, GWL_EXSTYLE);
	wAttr |= 0x80000;								//WS_EX_LAYERED ;)
	SetWindowLong(hWnd, GWL_EXSTYLE, wAttr);
	SetLayer(hWnd, 0, 0xD5, 2);

}
//Animate the window creation
//This may conflict with other compilers, but VC++ 6.0 cant directly link this API :P
//void AnimateWindow(HWND hWnd)
//{
//	pAnime	 Animate;
//	Animate = (pAnime)GetProcAddress(LoadLibrary("user32.dll"), "AnimateWindow");
//	
//	Animate(hWnd, 0x3E8, 0x80000);
//}
//Set region of the window
void SetRegion(HWND hWnd)
{
	HRSRC	hRsrc;
	DWORD	size;
	HRGN	hRgn;
	LPVOID	pRsrc;

	hRsrc = FindResource(hInst, MAKEINTRESOURCE(IDR_RGN1), "RGN");
	pRsrc = LockResource(LoadResource(hInst, hRsrc));
	size  = SizeofResource(hInst, hRsrc);
	hRgn  = ExtCreateRegion(NULL, size, pRsrc);

	SetWindowRgn(hWnd, hRgn, TRUE);

}
//Initiate 
void InitAll(HWND hWnd)
{
	hAbout    = GetDlgItem(hWnd, IDC_About);
	hGenerate = GetDlgItem(hWnd, IDC_Generate);
	hClose    = GetDlgItem(hWnd, IDC_Close);
	hCross    = GetDlgItem(hWnd, IDC_Cross);
	hMin      = GetDlgItem(hWnd, IDC_Min);

	hbmAbout		= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_AbtN));
	hbmAboutDown	= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_AbtP));
	hbmGenerate		= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GenN));
	hbmGenerateDown = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GenP));
	hbmClose		= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CloseN));
	hbmCloseDown	= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CloseP));
	hbmCross		= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CrossN));
	hbmCrossDown	= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CrossP));
	hbmMin			= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MinN));
	hbmMinDown		= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MinP));
	
}
//Set default values
void SetDefaults(HWND hWnd)
{
	SetWindowText(hWnd, APPINFO);
	SendDlgItemMessage(hWnd,
		IDC_Name,EM_LIMITTEXT,
		(WPARAM) MAX_NAME,
		(LPARAM) 0);
	SetDlgItemText(hWnd, IDC_Name, AUTHOR);

	loadmusic();
	mod = FMUSIC_LoadSong(NULL, NULL);
	if(mod) FMUSIC_PlaySong(mod);
}
//Invalidate buttons
void InvalidateButtons()
{
	InvalidateRect(hAbout,	  NULL, FALSE);
	InvalidateRect(hGenerate, NULL, FALSE);
	InvalidateRect(hClose,	  NULL, FALSE);
	InvalidateRect(hCross,    NULL, FALSE);
	InvalidateRect(hMin,	  NULL, FALSE);
}
//Draw buttons on click
void DrawButtons(LPARAM lParam)
{
	LPDRAWITEMSTRUCT	lpdis;
	HDC					hdcMem;

	lpdis	= (LPDRAWITEMSTRUCT) lParam;
	hdcMem	= CreateCompatibleDC(lpdis->hDC);

	switch(lpdis->CtlID)
	{
		case IDC_About:
			if (lpdis->itemState & ODS_SELECTED)
				SelectObject(hdcMem, hbmAboutDown);
			else
				SelectObject(hdcMem, hbmAbout);
			break;
		case IDC_Generate:
			if (lpdis->itemState & ODS_SELECTED)
				SelectObject(hdcMem, hbmGenerateDown);
			else
				SelectObject(hdcMem, hbmGenerate);
			break;
		case IDC_Close:
			if (lpdis->itemState & ODS_SELECTED)
				SelectObject(hdcMem, hbmCloseDown);
			else
				SelectObject(hdcMem, hbmClose);
			break;
		case IDC_Cross:
			if (lpdis->itemState & ODS_SELECTED)
				SelectObject(hdcMem, hbmCrossDown);
			else
				SelectObject(hdcMem, hbmCross);
			break;
		case IDC_Min:
			if (lpdis->itemState & ODS_SELECTED)
				SelectObject(hdcMem, hbmMinDown);
			else
				SelectObject(hdcMem, hbmMin);
			break;
	
	}

	BitBlt(
		lpdis->hDC,
		lpdis->rcItem.left,
		lpdis->rcItem.top, 
		lpdis->rcItem.right - lpdis->rcItem.left,
		lpdis->rcItem.bottom - lpdis->rcItem.top,
		hdcMem,
		0, 0,
		SRCCOPY);
	DeleteDC(hdcMem);

}
//WM_COMMAND dispatcher
void Dispatcher(HWND hWnd, WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
		case IDC_Generate:
		case IDC_Name:
			GenSerial(hWnd);
			break;
		case IDC_About:
			MessageBox(hWnd,APPINFO,"Info",MB_ICONASTERISK);
			break;
		case IDC_Close:
		case IDC_Cross:
			FMUSIC_StopSong(mod);
			FMUSIC_FreeSong(mod);
			EndDialog(hWnd,0);
			break;
		case IDC_Min:
			ShowWindow(hWnd, SW_MINIMIZE);
			UpdateWindow(hWnd);
			break;
	}
}
//Main Dialog Proc
BOOL CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			InitAll(hWnd);
			//AnimateWindow(hWnd);
			SetRegion(hWnd);
			SetTransparency(hWnd);
			SetDefaults(hWnd);
			break;
		case WM_DRAWITEM:
			DrawButtons(lParam);
			break;
		case WM_COMMAND:
			Dispatcher(hWnd, wParam);
			break;
		case WM_CTLCOLORSTATIC:
			SetTextColor((HDC)wParam, RGB(91,90,98));
			return (BOOL)GetStockObject(RGB(0,0,0));
			break;
		case WM_CTLCOLOREDIT:
			SetTextColor((HDC)wParam, RGB(91,90,98));
			return(BOOL)GetStockObject(RGB(0,0,0));
			break;
		case WM_MOUSEMOVE:
			ReleaseCapture();
			//InvalidateButtons();
			SendMessage(hWnd,WM_SYSCOMMAND, 0x0F012, 0);
			break;
	}
	return 0;
}
//Our own WinMain!!! :D
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	hInst=GetModuleHandle(NULL);
	InitCommonControls();
	
	DialogBoxParam(hInstance,
		MAKEINTRESOURCE(IDD_DIALOG1),
		NULL,
		(DLGPROC)DialogProc,
		0);
	return 0;
}
//Generate Serial
void GenSerial(HWND hWnd)
{
	int len;
	char szName[MAX_NAME+1];
	char szSerial[MAX_SERIAL];
	unsigned int *intSerial;

	unsigned char bArray1[5] = { 0xAA, 0x89, 0xC4, 0xFE, 0x00 };
	unsigned char bArray2[5] = { 0x78, 0xF0, 0xD0, 0x03, 0x00 };
	unsigned char bArray3[5] = { 0xF7, 0xFD, 0xF4, 0xE7, 0x00 };
	unsigned char bArray4[5] = { 0xB5, 0x1B, 0xC9, 0x50, 0x00 };

	len = GetDlgItemText(hWnd, IDC_Name, szName, MAX_NAME);
	if(len < 4)
	{
		SetDlgItemText(hWnd,
			IDC_Serial,
			"Name needs to be 4 chars or longer...");
	}
	else
	{
		memcpy(szSerial, szName, len+1);
		xorforward(szSerial, bArray1, len);
		xorbackward(szSerial, bArray2, len);
		xorforward(szSerial, bArray3, len);
		xorbackward(szSerial, bArray4, len);
		addfour(szSerial, len);


		intSerial = (unsigned int *)(szSerial+1);
		div10(*intSerial, szSerial);
		reversal(szSerial);

		SetDlgItemText(hWnd, IDC_Serial, szSerial);
	}
}

int reversal(char *stringz)
{
	int x = 0;
	int y = strlen(stringz) - 1;
	char z;

	for (; x < y; x++, y--)
	{
		z = stringz[x];
		stringz[x] = stringz[y];
		stringz[y] = z;
	}
}

int div10(unsigned int decimalz, char *stringz)
{
	int x = 0;

	memset(&stringz[0], 0, MAX_SERIAL);
	while (decimalz > 0){
		stringz[x] = (decimalz % 10 + 48);
		decimalz /= 10;
		x++;
	}	
	return 0;

}	

int xorforward(char *stringz, unsigned char *arr, size_t length)
{
	int y = 0;
	unsigned char temp[MAX_SERIAL];
	size_t len = strlen(arr);
	int x = 1;

	stringz[length+1] = 0x00;
	memcpy(temp, stringz, length+1);

	for (; x <= length; x++ ) {
		if (y > len-1) y = 0;
		stringz[x] ^= arr[y];
		arr[y] = temp[x];
		y++;
    };

	return 0;

}

int xorbackward(char *stringz, unsigned char *arr, size_t length)
{
	int y = 0;
	unsigned char temp[MAX_SERIAL];
	size_t len = strlen(arr);
	int x = length + 1;

	memcpy(temp, stringz, length+1);

	for (; x > 1; x-- ) {
		if (y > len-1) y = 0;
		stringz[x-1] ^= arr[y];
		arr[y] = temp[x];
		y++;
    };
	return 0;

}

int addfour(char *stringz, size_t length)
{
	//size_t length = strlen(stringz);
	int x = 5;
	for (; x < length; x++){
		stringz[x%4] += stringz[x];
	};
	stringz[length+1] = 0x00;
	return 0;
}