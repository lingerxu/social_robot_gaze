// EventHandler.cpp: implementation of the CEventHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EventHandler.h"
//#include "ETSerialPortViewer.h"
//#include "ETSerialPortViewerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define WM_NEW_MSG  (WM_USER + 1)

CComModule _Module;
extern 	CWnd* m_pMain;
extern IASLSerialOutPort3* gpISerialOutPort;
extern FILE* fp;
extern int isRecalled;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEventHandler::CEventHandler()
{
	m_pMain = NULL;
}

CEventHandler::~CEventHandler()
{
}

void CEventHandler::SetMainWindow(HWND hWnd)
{
	m_pMain = CWnd::FromHandle(hWnd);
}

//////////////////////////////////////////////////////////////////////
// Operation
//////////////////////////////////////////////////////////////////////


HRESULT __stdcall CEventHandler::Notify()
{
	//return E_FAIL;
	if (m_pMain == NULL)
		return E_FAIL;

	//LPSAFEARRAY* items = new LPSAFEARRAY; // we want a new location at each call
	LPSAFEARRAY items;
    long count;
	VARIANT_BOOL bAvailable;
	CComVariant value;
	long i=0;
	SYSTEMTIME st;
	GetSystemTime(&st);
	

	HRESULT hr = gpISerialOutPort->GetScaledData(&items, &count, &bAvailable);

	if (FAILED(hr))
		return hr;

	if (bAvailable == VARIANT_TRUE)
	{
		// get a data item from the eye tracker
		if(isRecalled==1)
		{
			fprintf(fp, "%02d%02d%02d.%03d\t", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
			for (long i = 0; i < count; i++)
			{
				SafeArrayGetElement(items, &i, &value);
				value.ChangeType(VT_BSTR);
				CString str = value.bstrVal;
				VariantClear(&value);	
				printf("%s, ", str);
				fprintf(fp, "%s\t", str);
			}
			// Clean up
			//UpdateData(FALSE);
			printf("\n");
			fprintf(fp,"\n");
			SafeArrayDestroy(items);
		}
	
	//m_pMain->PostMessage(WM_NEW_MSG, (WPARAM)items, count);
		// Note: message handler will perform the clean-up
	//	printf("in notify\n");
	}
	else
	{
		// This is unlikely to happen - we only receive a callback when
		// bAvailable is true, but just in case
		delete items;
	}

	return S_OK;
}

