// EventHandler.h: interface for the CEventHandler class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_EVENTHANDLER_H__D946D76A_186E_4318_AD89_8A38F1E75588__INCLUDED_)
#define AFX_EVENTHANDLER_H__D946D76A_186E_4318_AD89_8A38F1E75588__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ASLSerialOutLib2.h"

class CEventHandler : 
	public IDispEventImpl<
		0,
		CEventHandler,
		&DIID__IASLSerialOutPort2Events,
		&LIBID_ASLSERIALOUTLIB2Lib,
		1,
		0>
{
public:
	CEventHandler();
	virtual ~CEventHandler();
	void SetMainWindow(HWND hWnd);

	HRESULT __stdcall Notify();

	BEGIN_SINK_MAP(CEventHandler)
		SINK_ENTRY_EX(0, DIID__IASLSerialOutPort2Events, 1, Notify)
	END_SINK_MAP()

};

#endif // !defined(AFX_EVENTHANDLER_H__D946D76A_186E_4318_AD89_8A38F1E75588__INCLUDED_)
