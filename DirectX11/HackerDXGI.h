#pragma once

#include <dxgi1_2.h>

#include "HackerDevice.h"
#include "HackerContext.h"
#include "Overlay.h"


// Forward references required because of circular references from the
// other 'Hacker' objects.

class HackerDevice;
class HackerContext;
class Overlay;

// -----------------------------------------------------------------------------

// MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")

class HackerUnknown : public IUnknown
{
private:
	IUnknown *mOrigUnknown;

public:
	HackerUnknown(IUnknown *pUnknown);


	STDMETHOD(QueryInterface)(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject);

	STDMETHOD_(ULONG, AddRef)(THIS);

	STDMETHOD_(ULONG, Release)(THIS);
};


// -----------------------------------------------------------------------------

class HackerDXGIObject : public HackerUnknown
{
private:
	IDXGIObject *mOrigObject;

public:
	HackerDXGIObject(IDXGIObject *pObject);


	STDMETHOD(SetPrivateData)(
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [in] */ UINT DataSize,
		/* [annotation][in] */
		_In_reads_bytes_(DataSize)  const void *pData);

	STDMETHOD(SetPrivateDataInterface)(
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [annotation][in] */
		_In_  const IUnknown *pUnknown);

	STDMETHOD(GetPrivateData)(
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [annotation][out][in] */
		_Inout_  UINT *pDataSize,
		/* [annotation][out] */
		_Out_writes_bytes_(*pDataSize)  void *pData);

	STDMETHOD(GetParent)(
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][retval][out] */
		_Out_  void **ppParent);
};

			  
// -----------------------------------------------------------------------------

class HackerDXGIAdapter : public HackerDXGIObject
{
private:
	IDXGIAdapter *mOrigAdapter;
	HackerDevice *mHackerDevice;
	HackerContext *mHackerContext;

public:
	HackerDXGIAdapter(IDXGIAdapter *pAdapter, HackerDevice *pDevice, HackerContext *pContext);


	// Override the GetParent so we can wrap objects returned.
	STDMETHOD(GetParent)(
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][retval][out] */
		_Out_  void **ppParent) override;

	STDMETHOD(EnumOutputs)(THIS_
		/* [in] */ UINT Output,
		/* [annotation][out][in] */
		__out IDXGIOutput **ppOutput);

	STDMETHOD(GetDesc)(THIS_
		/* [annotation][out] */
		__out DXGI_ADAPTER_DESC *pDesc);

	STDMETHOD(CheckInterfaceSupport)(THIS_
		/* [annotation][in] */
		__in  REFGUID InterfaceName,
		/* [annotation][out] */
		__out  LARGE_INTEGER *pUMDVersion);
};


// -----------------------------------------------------------------------------

class HackerDXGIAdapter1 : public HackerDXGIAdapter
{
private:
	IDXGIAdapter1 *mOrigAdapter1;

public:
	HackerDXGIAdapter1(IDXGIAdapter1 *pAdapter, HackerDevice *pDevice, HackerContext *pContext);

	STDMETHOD(GetDesc1)(THIS_
		/* [annotation][out] */
		__out  DXGI_ADAPTER_DESC1 *pDesc);
};


// -----------------------------------------------------------------------------

class HackerDXGIDevice : public HackerDXGIObject
{
private:
	IDXGIDevice *mOrigDXGIDevice;
	HackerDevice *mHackerDevice;
	HackerContext *mHackerContext;

public:
	HackerDXGIDevice(IDXGIDevice *pDXGIDevice, HackerDevice *pDevice, HackerContext *pContext);


	// Override the GetParent so we can wrap objects returned.
	STDMETHOD(GetParent)(
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][retval][out] */
		_Out_  void **ppParent) override;

	STDMETHOD(GetAdapter)(
		/* [annotation][out] */
		_Out_  HackerDXGIAdapter **pAdapter);

	STDMETHOD(CreateSurface)(
		/* [annotation][in] */
		_In_  const DXGI_SURFACE_DESC *pDesc,
		/* [in] */ UINT NumSurfaces,
		/* [in] */ DXGI_USAGE Usage,
		/* [annotation][in] */
		_In_opt_  const DXGI_SHARED_RESOURCE *pSharedResource,
		/* [annotation][out] */
		_Out_  IDXGISurface **ppSurface);

	STDMETHOD(QueryResourceResidency)(
		/* [annotation][size_is][in] */
		_In_reads_(NumResources)  IUnknown *const *ppResources,
		/* [annotation][size_is][out] */
		_Out_writes_(NumResources)  DXGI_RESIDENCY *pResidencyStatus,
		/* [in] */ UINT NumResources);

	STDMETHOD(SetGPUThreadPriority)(
		/* [in] */ INT Priority);

	STDMETHOD(GetGPUThreadPriority)(
		/* [annotation][retval][out] */
		_Out_  INT *pPriority);
};


// -----------------------------------------------------------------------------

class HackerDXGIOutput : public IDXGIOutput
{
private:
	IDXGIOutput	*mOrigOutput;

public:
	HackerDXGIOutput(IDXGIOutput *pOutput);


	// ******************* IDirect3DUnknown methods 

	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	// ******************* IDXGIObject methods

	STDMETHOD(SetPrivateData)(THIS_
		/* [annotation][in] */
		__in  REFGUID Name,
		/* [in] */ UINT DataSize,
		/* [annotation][in] */
		__in_bcount(DataSize)  const void *pData);

	STDMETHOD(SetPrivateDataInterface)(THIS_
		/* [annotation][in] */
		__in  REFGUID Name,
		/* [annotation][in] */
		__in  const IUnknown *pUnknown);

	STDMETHOD(GetPrivateData)(THIS_
		/* [annotation][in] */
		__in  REFGUID Name,
		/* [annotation][out][in] */
		__inout  UINT *pDataSize,
		/* [annotation][out] */
		__out_bcount(*pDataSize)  void *pData);

	STDMETHOD(GetParent)(THIS_
		/* [annotation][in] */
		__in  REFIID riid,
		/* [annotation][retval][out] */
		__out  void **ppParent);

	// ******************* IDXGIOutput methods 
	STDMETHOD(GetDesc)(THIS_
		/* [annotation][out] */
		__out  DXGI_OUTPUT_DESC *pDesc);

	STDMETHOD(GetDisplayModeList)(THIS_
		/* [in] */ DXGI_FORMAT EnumFormat,
		/* [in] */ UINT Flags,
		/* [annotation][out][in] */
		__inout  UINT *pNumModes,
		/* [annotation][out] */
		__out_ecount_part_opt(*pNumModes, *pNumModes)  DXGI_MODE_DESC *pDesc);

	STDMETHOD(FindClosestMatchingMode)(THIS_
		/* [annotation][in] */
		__in  const DXGI_MODE_DESC *pModeToMatch,
		/* [annotation][out] */
		__out  DXGI_MODE_DESC *pClosestMatch,
		/* [annotation][in] */
		__in_opt  IUnknown *pConcernedDevice);

	STDMETHOD(WaitForVBlank)(THIS_);

	STDMETHOD(TakeOwnership)(THIS_
		/* [annotation][in] */
		__in  IUnknown *pDevice,
		BOOL Exclusive);

	virtual void STDMETHODCALLTYPE ReleaseOwnership(void);

	STDMETHOD(GetGammaControlCapabilities)(THIS_
		/* [annotation][out] */
		__out  DXGI_GAMMA_CONTROL_CAPABILITIES *pGammaCaps);

	STDMETHOD(SetGammaControl)(THIS_
		/* [annotation][in] */
		__in  const DXGI_GAMMA_CONTROL *pArray);

	STDMETHOD(GetGammaControl)(THIS_
		/* [annotation][out] */
		__out  DXGI_GAMMA_CONTROL *pArray);

	STDMETHOD(SetDisplaySurface)(THIS_
		/* [annotation][in] */
		__in  IDXGISurface *pScanoutSurface);

	STDMETHOD(GetDisplaySurfaceData)(THIS_
		/* [annotation][in] */
		__in  IDXGISurface *pDestination);

	STDMETHOD(GetFrameStatistics)(THIS_
		/* [annotation][out] */
		__out  DXGI_FRAME_STATISTICS *pStats);
};


// -----------------------------------------------------------------------------

class HackerDXGIDeviceSubObject: public HackerDXGIObject
{
private:
	IDXGIDeviceSubObject *mOrigDeviceSubObject;

public:
	HackerDXGIDeviceSubObject(IDXGIDeviceSubObject *pSubObject);


	STDMETHOD(GetDevice)(
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][retval][out] */
		_Out_  void **ppDevice);
};


// -----------------------------------------------------------------------------

class HackerDXGISwapChain : public HackerDXGIDeviceSubObject
{
private:
	IDXGISwapChain *mOrigSwapChain;
	Overlay *mOverlay;

public:

	HackerDXGISwapChain(IDXGISwapChain *pOutput, HackerDevice *pDevice, HackerContext *pContext);

	IDXGISwapChain* GetOrigSwapChain();

	//**** IDXGISwapChain implementation
    STDMETHOD(Present)(THIS_
            /* [in] */ UINT SyncInterval,
            /* [in] */ UINT Flags);
        
    STDMETHOD(GetBuffer)(THIS_
            /* [in] */ UINT Buffer,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][out][in] */ 
            _Out_  void **ppSurface);
        
    STDMETHOD(SetFullscreenState)(THIS_
            /* [in] */ BOOL Fullscreen,
            /* [annotation][in] */ 
            _In_opt_  IDXGIOutput *pTarget);
        
    STDMETHOD(GetFullscreenState)(THIS_
            /* [annotation][out] */ 
            _Out_opt_  BOOL *pFullscreen,
            /* [annotation][out] */ 
            _Out_opt_  IDXGIOutput **ppTarget);
        
    STDMETHOD(GetDesc)(THIS_
            /* [annotation][out] */ 
            _Out_  DXGI_SWAP_CHAIN_DESC *pDesc);
        
    STDMETHOD(ResizeBuffers)(THIS_
            /* [in] */ UINT BufferCount,
            /* [in] */ UINT Width,
            /* [in] */ UINT Height,
            /* [in] */ DXGI_FORMAT NewFormat,
            /* [in] */ UINT SwapChainFlags);
        
    STDMETHOD(ResizeTarget)(THIS_
            /* [annotation][in] */ 
            _In_  const DXGI_MODE_DESC *pNewTargetParameters);
        
    STDMETHOD(GetContainingOutput)(THIS_
            /* [annotation][out] */ 
            _Out_  IDXGIOutput **ppOutput);
        
    STDMETHOD(GetFrameStatistics)(THIS_
            /* [annotation][out] */ 
            _Out_  DXGI_FRAME_STATISTICS *pStats);
        
    STDMETHOD(GetLastPresentCount)(THIS_
            /* [annotation][out] */ 
            _Out_  UINT *pLastPresentCount);
        
};


// -----------------------------------------------------------------------------

class HackerDXGISwapChain1 : public HackerDXGISwapChain
{
private:
	IDXGISwapChain1 *mOrigSwapChain1;

public:
	HackerDXGISwapChain1(IDXGISwapChain1 *pSwapChain, HackerDevice *pDevice, HackerContext *pContext);

	STDMETHOD(GetDesc1)(THIS_
            /* [annotation][out] */ 
            _Out_  DXGI_SWAP_CHAIN_DESC1 *pDesc);
        
	STDMETHOD(GetFullscreenDesc)(THIS_
            /* [annotation][out] */ 
            _Out_  DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pDesc);
        
	STDMETHOD(GetHwnd)(THIS_
            /* [annotation][out] */ 
            _Out_  HWND *pHwnd);
        
	STDMETHOD(GetCoreWindow)(THIS_
            /* [annotation][in] */ 
            _In_  REFIID refiid,
            /* [annotation][out] */ 
            _Out_  void **ppUnk);
        
	STDMETHOD(Present1)(THIS_
            /* [in] */ UINT SyncInterval,
            /* [in] */ UINT PresentFlags,
            /* [annotation][in] */ 
            _In_  const DXGI_PRESENT_PARAMETERS *pPresentParameters);
        
	STDMETHOD_(BOOL, IsTemporaryMonoSupported)(THIS);
        
	STDMETHOD(GetRestrictToOutput)(THIS_
            /* [annotation][out] */ 
            _Out_  IDXGIOutput **ppRestrictToOutput);
        
	STDMETHOD(SetBackgroundColor)(THIS_
            /* [annotation][in] */ 
            _In_  const DXGI_RGBA *pColor);
        
	STDMETHOD(GetBackgroundColor)(THIS_
            /* [annotation][out] */ 
            _Out_  DXGI_RGBA *pColor);
        
	STDMETHOD(SetRotation)(THIS_
            /* [annotation][in] */ 
            _In_  DXGI_MODE_ROTATION Rotation);
        
	STDMETHOD(GetRotation)(THIS_
            /* [annotation][out] */ 
            _Out_  DXGI_MODE_ROTATION *pRotation);
};

// -----------------------------------------------------------------------------

class HackerDXGIFactory : public HackerDXGIObject
{
protected:
	IDXGIFactory *mOrigFactory;
	HackerDevice *mHackerDevice;
	HackerContext *mHackerContext;

public:
	HackerDXGIFactory(IDXGIFactory *pFactory, HackerDevice *pDevice, HackerContext *pContext);


	STDMETHOD(EnumAdapters)(THIS_
		/* [in] */ UINT Adapter,
		/* [annotation][out] */
		__out IDXGIAdapter **ppAdapter);

	STDMETHOD(MakeWindowAssociation)(THIS_
		HWND WindowHandle,
		UINT Flags);

	STDMETHOD(GetWindowAssociation)(THIS_
		/* [annotation][out] */
		__out  HWND *pWindowHandle);

	STDMETHOD(CreateSwapChain)(THIS_
		/* [annotation][in] */
		__in  IUnknown *pDevice,
		/* [annotation][in] */
		__in  DXGI_SWAP_CHAIN_DESC *pDesc,
		/* [annotation][out] */
		__out  HackerDXGISwapChain **ppSwapChain);

	STDMETHOD(CreateSoftwareAdapter)(THIS_
		/* [in] */ HMODULE Module,
		/* [annotation][out] */
		__out  IDXGIAdapter **ppAdapter);

};

// -----------------------------------------------------------------------------

class HackerDXGIFactory1 : public HackerDXGIFactory
{
private:
	IDXGIFactory1 *mOrigFactory1;

public:
	HackerDXGIFactory1(IDXGIFactory1 *pFactory, HackerDevice *pDevice, HackerContext *pContext);

	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);
	STDMETHOD(QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject));

	// ******************* IDXGIFactory methods 

	STDMETHOD(EnumAdapters)(THIS_
		/* [in] */ UINT Adapter,
		/* [annotation][out] */
		__out IDXGIAdapter **ppAdapter);

	STDMETHOD(MakeWindowAssociation)(THIS_
		HWND WindowHandle,
		UINT Flags);

	STDMETHOD(GetWindowAssociation)(THIS_
		/* [annotation][out] */
		__out  HWND *pWindowHandle);

	STDMETHOD(CreateSwapChain)(THIS_
		/* [annotation][in] */
		__in  IUnknown *pDevice,
		/* [annotation][in] */
		__in  DXGI_SWAP_CHAIN_DESC *pDesc,
		/* [annotation][out] */
		__out  IDXGISwapChain **ppSwapChain);

	STDMETHOD(CreateSoftwareAdapter)(THIS_
		/* [in] */ HMODULE Module,
		/* [annotation][out] */
		__out  IDXGIAdapter **ppAdapter);

	// ******************* IDXGIFactory1 methods 

	STDMETHOD(EnumAdapters1)(THIS_
		/* [in] */ UINT Adapter,
		/* [annotation][out] */
		__out IDXGIAdapter1 **ppAdapter);

	STDMETHOD_(BOOL, IsCurrent)(THIS);
};


// -----------------------------------------------------------------------------

class HackerDXGIFactory2 : public HackerDXGIFactory1
{
private:
	IDXGIFactory2 *mOrigFactory2;

public:
	HackerDXGIFactory2(IDXGIFactory2 *pFactory, HackerDevice *pDevice, HackerContext *pContext);

	STDMETHOD_(BOOL, IsWindowedStereoEnabled)(THIS);

	STDMETHOD(CreateSwapChainForHwnd)(
		/* [annotation][in] */
		_In_  IUnknown *pDevice,
		/* [annotation][in] */
		_In_  HWND hWnd,
		/* [annotation][in] */
		_In_  const DXGI_SWAP_CHAIN_DESC1 *pDesc,
		/* [annotation][in] */
		_In_opt_  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pFullscreenDesc,
		/* [annotation][in] */
		_In_opt_  IDXGIOutput *pRestrictToOutput,
		/* [annotation][out] */
		_Out_  IDXGISwapChain1 **ppSwapChain);

	STDMETHOD(CreateSwapChainForCoreWindow)(
		/* [annotation][in] */
		_In_  IUnknown *pDevice,
		/* [annotation][in] */
		_In_  IUnknown *pWindow,
		/* [annotation][in] */
		_In_  const DXGI_SWAP_CHAIN_DESC1 *pDesc,
		/* [annotation][in] */
		_In_opt_  IDXGIOutput *pRestrictToOutput,
		/* [annotation][out] */
		_Out_  IDXGISwapChain1 **ppSwapChain);

	STDMETHOD(GetSharedResourceAdapterLuid)(
		/* [annotation] */
		_In_  HANDLE hResource,
		/* [annotation] */
		_Out_  LUID *pLuid);

	STDMETHOD(RegisterStereoStatusWindow)(
		/* [annotation][in] */
		_In_  HWND WindowHandle,
		/* [annotation][in] */
		_In_  UINT wMsg,
		/* [annotation][out] */
		_Out_  DWORD *pdwCookie);

	STDMETHOD(RegisterStereoStatusEvent)(
		/* [annotation][in] */
		_In_  HANDLE hEvent,
		/* [annotation][out] */
		_Out_  DWORD *pdwCookie);

	STDMETHOD_(void, STDMETHODCALLTYPE UnregisterStereoStatus)(
		/* [annotation][in] */
		_In_  DWORD dwCookie);

	STDMETHOD(RegisterOcclusionStatusWindow)(
		/* [annotation][in] */
		_In_  HWND WindowHandle,
		/* [annotation][in] */
		_In_  UINT wMsg,
		/* [annotation][out] */
		_Out_  DWORD *pdwCookie);

	STDMETHOD(RegisterOcclusionStatusEvent)(
		/* [annotation][in] */
		_In_  HANDLE hEvent,
		/* [annotation][out] */
		_Out_  DWORD *pdwCookie);

	STDMETHOD_(void, STDMETHODCALLTYPE UnregisterOcclusionStatus)(
		/* [annotation][in] */
		_In_  DWORD dwCookie);

	STDMETHOD(CreateSwapChainForComposition)(
		/* [annotation][in] */
		_In_  IUnknown *pDevice,
		/* [annotation][in] */
		_In_  const DXGI_SWAP_CHAIN_DESC1 *pDesc,
		/* [annotation][in] */
		_In_opt_  IDXGIOutput *pRestrictToOutput,
		/* [annotation][out] */
		_Outptr_  IDXGISwapChain1 **ppSwapChain);
};


