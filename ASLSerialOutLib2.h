/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Feb 09 18:07:09 2005
 */
/* Compiler settings for C:\EyeTracker5000\Windows\MSVC6\ASLSerialOutLib2\ASLSerialOutLib2.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ASLSerialOutLib2_h__
#define __ASLSerialOutLib2_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IASLSerialOutPort2_FWD_DEFINED__
#define __IASLSerialOutPort2_FWD_DEFINED__
typedef interface IASLSerialOutPort2 IASLSerialOutPort2;
#endif 	/* __IASLSerialOutPort2_FWD_DEFINED__ */


#ifndef __IASLSerialOutPort3_FWD_DEFINED__
#define __IASLSerialOutPort3_FWD_DEFINED__
typedef interface IASLSerialOutPort3 IASLSerialOutPort3;
#endif 	/* __IASLSerialOutPort3_FWD_DEFINED__ */


#ifndef ___IASLSerialOutPort2Events_FWD_DEFINED__
#define ___IASLSerialOutPort2Events_FWD_DEFINED__
typedef interface _IASLSerialOutPort2Events _IASLSerialOutPort2Events;
#endif 	/* ___IASLSerialOutPort2Events_FWD_DEFINED__ */


#ifndef __ASLSerialOutPort2_FWD_DEFINED__
#define __ASLSerialOutPort2_FWD_DEFINED__

#ifdef __cplusplus
typedef class ASLSerialOutPort2 ASLSerialOutPort2;
#else
typedef struct ASLSerialOutPort2 ASLSerialOutPort2;
#endif /* __cplusplus */

#endif 	/* __ASLSerialOutPort2_FWD_DEFINED__ */


#ifndef __ASLSerialOutPort3_FWD_DEFINED__
#define __ASLSerialOutPort3_FWD_DEFINED__

#ifdef __cplusplus
typedef class ASLSerialOutPort3 ASLSerialOutPort3;
#else
typedef struct ASLSerialOutPort3 ASLSerialOutPort3;
#endif /* __cplusplus */

#endif 	/* __ASLSerialOutPort3_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_ASLSerialOutLib2_0000 */
/* [local] */ 


enum ASLSerialOutPort_Type
    {	ASL_TYPE_BYTE	= 0,
	ASL_TYPE_SHORT	= 1,
	ASL_TYPE_UNSIGNED_SHORT	= 2,
	ASL_TYPE_LONG	= 3,
	ASL_TYPE_UNSIGNED_LONG	= 4,
	ASL_TYPE_FLOAT	= 5,
	ASL_TYPE_NO_VALUE	= 6
    };


extern RPC_IF_HANDLE __MIDL_itf_ASLSerialOutLib2_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ASLSerialOutLib2_0000_v0_0_s_ifspec;

#ifndef __IASLSerialOutPort2_INTERFACE_DEFINED__
#define __IASLSerialOutPort2_INTERFACE_DEFINED__

/* interface IASLSerialOutPort2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IASLSerialOutPort2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("23494105-4C0E-4E25-BF79-75849C6FB9CB")
    IASLSerialOutPort2 : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Connect( 
            /* [in] */ long port,
            /* [in] */ long baudRate,
            /* [in] */ VARIANT_BOOL streamingMode,
            /* [in] */ long updateRate,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *itemTypes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartContinuousMode( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopContinuousMode( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDataRecord( 
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *items,
            /* [out] */ long __RPC_FAR *count,
            /* [out] */ VARIANT_BOOL __RPC_FAR *available) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLastError( 
            /* [out] */ BSTR __RPC_FAR *errorDesc) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IASLSerialOutPort2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IASLSerialOutPort2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IASLSerialOutPort2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IASLSerialOutPort2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IASLSerialOutPort2 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IASLSerialOutPort2 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IASLSerialOutPort2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IASLSerialOutPort2 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Connect )( 
            IASLSerialOutPort2 __RPC_FAR * This,
            /* [in] */ long port,
            /* [in] */ long baudRate,
            /* [in] */ VARIANT_BOOL streamingMode,
            /* [in] */ long updateRate,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *itemTypes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disconnect )( 
            IASLSerialOutPort2 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartContinuousMode )( 
            IASLSerialOutPort2 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StopContinuousMode )( 
            IASLSerialOutPort2 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDataRecord )( 
            IASLSerialOutPort2 __RPC_FAR * This,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *items,
            /* [out] */ long __RPC_FAR *count,
            /* [out] */ VARIANT_BOOL __RPC_FAR *available);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLastError )( 
            IASLSerialOutPort2 __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *errorDesc);
        
        END_INTERFACE
    } IASLSerialOutPort2Vtbl;

    interface IASLSerialOutPort2
    {
        CONST_VTBL struct IASLSerialOutPort2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IASLSerialOutPort2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IASLSerialOutPort2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IASLSerialOutPort2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IASLSerialOutPort2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IASLSerialOutPort2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IASLSerialOutPort2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IASLSerialOutPort2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IASLSerialOutPort2_Connect(This,port,baudRate,streamingMode,updateRate,itemTypes)	\
    (This)->lpVtbl -> Connect(This,port,baudRate,streamingMode,updateRate,itemTypes)

#define IASLSerialOutPort2_Disconnect(This)	\
    (This)->lpVtbl -> Disconnect(This)

#define IASLSerialOutPort2_StartContinuousMode(This)	\
    (This)->lpVtbl -> StartContinuousMode(This)

#define IASLSerialOutPort2_StopContinuousMode(This)	\
    (This)->lpVtbl -> StopContinuousMode(This)

#define IASLSerialOutPort2_GetDataRecord(This,items,count,available)	\
    (This)->lpVtbl -> GetDataRecord(This,items,count,available)

#define IASLSerialOutPort2_GetLastError(This,errorDesc)	\
    (This)->lpVtbl -> GetLastError(This,errorDesc)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IASLSerialOutPort2_Connect_Proxy( 
    IASLSerialOutPort2 __RPC_FAR * This,
    /* [in] */ long port,
    /* [in] */ long baudRate,
    /* [in] */ VARIANT_BOOL streamingMode,
    /* [in] */ long updateRate,
    /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *itemTypes);


void __RPC_STUB IASLSerialOutPort2_Connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IASLSerialOutPort2_Disconnect_Proxy( 
    IASLSerialOutPort2 __RPC_FAR * This);


void __RPC_STUB IASLSerialOutPort2_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IASLSerialOutPort2_StartContinuousMode_Proxy( 
    IASLSerialOutPort2 __RPC_FAR * This);


void __RPC_STUB IASLSerialOutPort2_StartContinuousMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IASLSerialOutPort2_StopContinuousMode_Proxy( 
    IASLSerialOutPort2 __RPC_FAR * This);


void __RPC_STUB IASLSerialOutPort2_StopContinuousMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IASLSerialOutPort2_GetDataRecord_Proxy( 
    IASLSerialOutPort2 __RPC_FAR * This,
    /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *items,
    /* [out] */ long __RPC_FAR *count,
    /* [out] */ VARIANT_BOOL __RPC_FAR *available);


void __RPC_STUB IASLSerialOutPort2_GetDataRecord_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IASLSerialOutPort2_GetLastError_Proxy( 
    IASLSerialOutPort2 __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *errorDesc);


void __RPC_STUB IASLSerialOutPort2_GetLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IASLSerialOutPort2_INTERFACE_DEFINED__ */


#ifndef __IASLSerialOutPort3_INTERFACE_DEFINED__
#define __IASLSerialOutPort3_INTERFACE_DEFINED__

/* interface IASLSerialOutPort3 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IASLSerialOutPort3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7998ECCF-C5B4-4980-9191-127E966609DF")
    IASLSerialOutPort3 : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Connect( 
            /* [in] */ BSTR cfgFile,
            /* [in] */ long port,
            /* [in] */ VARIANT_BOOL eyeheadIntegration,
            /* [out] */ long __RPC_FAR *baudRate,
            /* [out] */ long __RPC_FAR *updateRate,
            /* [out] */ VARIANT_BOOL __RPC_FAR *streamingMode,
            /* [out] */ long __RPC_FAR *itemCount,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *itemNames) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartContinuousMode( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopContinuousMode( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetScaledData( 
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *items,
            /* [out] */ long __RPC_FAR *count,
            /* [out] */ VARIANT_BOOL __RPC_FAR *available) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLastError( 
            /* [out] */ BSTR __RPC_FAR *errorDesc) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IASLSerialOutPort3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IASLSerialOutPort3 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IASLSerialOutPort3 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IASLSerialOutPort3 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IASLSerialOutPort3 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IASLSerialOutPort3 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IASLSerialOutPort3 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IASLSerialOutPort3 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Connect )( 
            IASLSerialOutPort3 __RPC_FAR * This,
            /* [in] */ BSTR cfgFile,
            /* [in] */ long port,
            /* [in] */ VARIANT_BOOL eyeheadIntegration,
            /* [out] */ long __RPC_FAR *baudRate,
            /* [out] */ long __RPC_FAR *updateRate,
            /* [out] */ VARIANT_BOOL __RPC_FAR *streamingMode,
            /* [out] */ long __RPC_FAR *itemCount,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *itemNames);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disconnect )( 
            IASLSerialOutPort3 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartContinuousMode )( 
            IASLSerialOutPort3 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StopContinuousMode )( 
            IASLSerialOutPort3 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetScaledData )( 
            IASLSerialOutPort3 __RPC_FAR * This,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *items,
            /* [out] */ long __RPC_FAR *count,
            /* [out] */ VARIANT_BOOL __RPC_FAR *available);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLastError )( 
            IASLSerialOutPort3 __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *errorDesc);
        
        END_INTERFACE
    } IASLSerialOutPort3Vtbl;

    interface IASLSerialOutPort3
    {
        CONST_VTBL struct IASLSerialOutPort3Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IASLSerialOutPort3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IASLSerialOutPort3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IASLSerialOutPort3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IASLSerialOutPort3_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IASLSerialOutPort3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IASLSerialOutPort3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IASLSerialOutPort3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IASLSerialOutPort3_Connect(This,cfgFile,port,eyeheadIntegration,baudRate,updateRate,streamingMode,itemCount,itemNames)	\
    (This)->lpVtbl -> Connect(This,cfgFile,port,eyeheadIntegration,baudRate,updateRate,streamingMode,itemCount,itemNames)

#define IASLSerialOutPort3_Disconnect(This)	\
    (This)->lpVtbl -> Disconnect(This)

#define IASLSerialOutPort3_StartContinuousMode(This)	\
    (This)->lpVtbl -> StartContinuousMode(This)

#define IASLSerialOutPort3_StopContinuousMode(This)	\
    (This)->lpVtbl -> StopContinuousMode(This)

#define IASLSerialOutPort3_GetScaledData(This,items,count,available)	\
    (This)->lpVtbl -> GetScaledData(This,items,count,available)

#define IASLSerialOutPort3_GetLastError(This,errorDesc)	\
    (This)->lpVtbl -> GetLastError(This,errorDesc)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IASLSerialOutPort3_Connect_Proxy( 
    IASLSerialOutPort3 __RPC_FAR * This,
    /* [in] */ BSTR cfgFile,
    /* [in] */ long port,
    /* [in] */ VARIANT_BOOL eyeheadIntegration,
    /* [out] */ long __RPC_FAR *baudRate,
    /* [out] */ long __RPC_FAR *updateRate,
    /* [out] */ VARIANT_BOOL __RPC_FAR *streamingMode,
    /* [out] */ long __RPC_FAR *itemCount,
    /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *itemNames);


void __RPC_STUB IASLSerialOutPort3_Connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IASLSerialOutPort3_Disconnect_Proxy( 
    IASLSerialOutPort3 __RPC_FAR * This);


void __RPC_STUB IASLSerialOutPort3_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IASLSerialOutPort3_StartContinuousMode_Proxy( 
    IASLSerialOutPort3 __RPC_FAR * This);


void __RPC_STUB IASLSerialOutPort3_StartContinuousMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IASLSerialOutPort3_StopContinuousMode_Proxy( 
    IASLSerialOutPort3 __RPC_FAR * This);


void __RPC_STUB IASLSerialOutPort3_StopContinuousMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IASLSerialOutPort3_GetScaledData_Proxy( 
    IASLSerialOutPort3 __RPC_FAR * This,
    /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *items,
    /* [out] */ long __RPC_FAR *count,
    /* [out] */ VARIANT_BOOL __RPC_FAR *available);


void __RPC_STUB IASLSerialOutPort3_GetScaledData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IASLSerialOutPort3_GetLastError_Proxy( 
    IASLSerialOutPort3 __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *errorDesc);


void __RPC_STUB IASLSerialOutPort3_GetLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IASLSerialOutPort3_INTERFACE_DEFINED__ */



#ifndef __ASLSERIALOUTLIB2Lib_LIBRARY_DEFINED__
#define __ASLSERIALOUTLIB2Lib_LIBRARY_DEFINED__

/* library ASLSERIALOUTLIB2Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ASLSERIALOUTLIB2Lib;

#ifndef ___IASLSerialOutPort2Events_DISPINTERFACE_DEFINED__
#define ___IASLSerialOutPort2Events_DISPINTERFACE_DEFINED__

/* dispinterface _IASLSerialOutPort2Events */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IASLSerialOutPort2Events;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C8139AC3-A35F-4507-918C-2DF364F88C26")
    _IASLSerialOutPort2Events : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IASLSerialOutPort2EventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _IASLSerialOutPort2Events __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _IASLSerialOutPort2Events __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _IASLSerialOutPort2Events __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _IASLSerialOutPort2Events __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _IASLSerialOutPort2Events __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _IASLSerialOutPort2Events __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _IASLSerialOutPort2Events __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _IASLSerialOutPort2EventsVtbl;

    interface _IASLSerialOutPort2Events
    {
        CONST_VTBL struct _IASLSerialOutPort2EventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IASLSerialOutPort2Events_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IASLSerialOutPort2Events_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IASLSerialOutPort2Events_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IASLSerialOutPort2Events_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IASLSerialOutPort2Events_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IASLSerialOutPort2Events_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IASLSerialOutPort2Events_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IASLSerialOutPort2Events_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ASLSerialOutPort2;

#ifdef __cplusplus

class DECLSPEC_UUID("9A777A5F-2C6E-44C3-BE85-259891AC8210")
ASLSerialOutPort2;
#endif

EXTERN_C const CLSID CLSID_ASLSerialOutPort3;

#ifdef __cplusplus

class DECLSPEC_UUID("E8697DF4-FEF7-4CD0-8AAD-F0FB0A1A332D")
ASLSerialOutPort3;
#endif
#endif /* __ASLSERIALOUTLIB2Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long __RPC_FAR *, unsigned long            , LPSAFEARRAY __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, LPSAFEARRAY __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, LPSAFEARRAY __RPC_FAR * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long __RPC_FAR *, LPSAFEARRAY __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
