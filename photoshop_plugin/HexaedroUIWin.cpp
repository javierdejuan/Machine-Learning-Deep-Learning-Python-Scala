// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2002 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this 
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "HexaedroPlugin.h"
#include "HexaedroUI.h"
#include "FilterBigDocument.h"



DLLExport BOOL WINAPI DissolveProc(HWND hDlg,
	UINT wMsg,
	WPARAM wParam,
	LPARAM /*lParam*/)
{
	BOOL returnValue = TRUE;
	int	item, cmd;
	switch (wMsg)
	{
	case WM_INITDIALOG:
		CenterDialog(hDlg);
		
	case WM_PAINT:
		returnValue = FALSE;
		break;

	case WM_COMMAND:
		item = LOWORD(wParam);
		cmd = HIWORD(wParam);
		switch (item)
		{
		case kDOK:
		case kDCancel:
			if (cmd == BN_CLICKED)
			{
				EndDialog(hDlg, item);
				returnValue = TRUE;
			}
			break;

		default:
			if (item >= kDFirstRadioHexaedro && item <= kDLastRadioHexaedro)
			{
				if (cmd == BN_CLICKED)
				{
					CheckRadioButton(hDlg,
						kDFirstRadioHexaedro,
						kDLastRadioHexaedro,
						COMMANDID(wParam));
					gParams->radioItem = item;
				}
			}
			break;
		}
		returnValue = TRUE;
		break;

	default:
		returnValue = FALSE;
		break;
	}
	return returnValue;
}


//-------------------------------------------------------------------------------
//
// DoUI
//
// Pop the UI and return true if the last item was the OK button.
// 
//-------------------------------------------------------------------------------
Boolean DoUI(void)
{
	PlatformData* platform = (PlatformData*)(gFilterRecord->platformData);

	INT_PTR result = DialogBoxParam(GetDLLInstance(),
							(LPSTR)"DISSOLVEDIALOG",
							(HWND)platform->hwnd,
							(DLGPROC)DissolveProc,
							NULL);
	//used by the DissolveProc routine, don't let the error go further
	*gResult = noErr;
	return (result == kDOK);
}




//-------------------------------------------------------------------------------
//
// DoAbout
//
// Pop a simple about box for this plug in.
//
// NOTE:	The global gFilterRecord is NOT a FilterRecord*. You must cast it to
//			an AboutRecord*. See PIAbout.h
//
//-------------------------------------------------------------------------------
void DoAbout(void)
{
	AboutRecord* aboutRecord = (AboutRecord*)gFilterRecord;
	ShowAbout(aboutRecord);
}
// end DissolveUIWin.cpp
