/* This file is encoded in UTF-8,  */
/* but BOM has not been added to accommodate environments not compatible with BOM. */
/*
 StApi C
*/
/*!
	@ingroup	StApiC_PublicImpl
	@file StApi_C.h
	@~english	@details
	@~japanese	@details
*/

#if !defined(STAPI_C_DEF_H__)
#define STAPI_C_DEF_H__

#ifdef __BORLANDC__
#include <_stddef.h>
#endif /* __BORLANDC__ */

#include <stdio.h>

#if (defined(_MSC_VER) && (1600 <= _MSC_VER)) || (!defined(_WIN32))
/*VC++2010- */
#include <stdint.h>
#else
#ifndef _STDINT
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#endif /*_STDINT */
#endif /* 1600 <= _MSC_VER */

#include <GenICamVersion.h>
#include <StApi_TLDef.h>
#include <StApi_IPDef.h>
#include <StApi_GUIDef.h>
#include <StGenTL_Def.h>
#include <StApi_PixelFormat.h>
#include <StGenApi_Def.h>

#ifdef STAPI_C_EXPORTS
#   define STAPI_API_EXPORTS /* __declspec(dllexport) */
#   define STAPI_CALLTYPE __stdcall
#else
#   if defined(_WIN32)
#       if defined(_MSC_VER)
            /* MSVC */
#           define STAPI_API_EXPORTS
#       elif defined(__BORLANDC__)
            /* BCC */
#           define STAPI_API_EXPORTS __declspec(dllimport)	
#       elif defined(__GNUC__)
            /* gcc(MinGW) */
#           define STAPI_API_EXPORTS __declspec(dllimport)	
#       endif
#       define STAPI_CALLTYPE __stdcall
#   else
        /* non windows */
#       define STAPI_API_EXPORTS __attribute__((visibility("default")))
#       define STAPI_CALLTYPE 
#   endif
#endif





#ifdef __cplusplus
extern "C" {
	using namespace GenTL;
	using namespace StApi;
#endif /* __cplusplus */




	/*************************************************************************/
	/* Typedef */
	/*************************************************************************/
	/*************************************************************************/
	/* Enumerator */
	/*************************************************************************/
	/*!
		@ingroup	StApiC_PublicImpl
		@enum EStApiCError_t
		@~english	@brief	Enumerator for error code.
		@~english	@details	Enumerator for error code.
		@~japanese	@brief	???????????????????????????
		@~japanese	@details	???????????????????????????
	*/
	typedef enum _EStApiCError_t
	{
		/*!
			@~english	@details	Successed.
			@~japanese	@details	????????????
		*/
		StApiCError_NoError = 0x00000000,

		/*!
			@~english	@details	Error.
			@~japanese	@details	?????????
		*/
		StApiCError_Error = 0xC0000000,

		/*!
			@~english	@details	Bad allocation.
			@~japanese	@details	?????????????????????????????????
		*/
		StApiCError_BadAlloc = 0xC0000001,

		/*!
			@~english	@details	Argument is invalid.
			@~japanese	@details	???????????????
		*/
		StApiCError_InvalidArgument = 0xC0000002,

		/*!
			@~english	@details	Argument is out of range.
			@~japanese	@details	??????????????????
		*/
		StApiCError_OutOfRange = 0xC0000003,

		/*!
			@~english	@details	Property access fails.
			@~japanese	@details	????????????????????????????????????
		*/
		StApiCError_Property = 0xC0000004,

		/*!
			@~english	@details	Runtime error.
			@~japanese	@details	????????????????????????
		*/
		StApiCError_Runtime = 0xC0000005,

		/*!
			@~english	@details	Logical error.
			@~japanese	@details	???????????????
		*/
		StApiCError_LogicalError = 0xC0000006,

		/*!
			@~english	@details	Access error.
			@~japanese	@details	?????????????????????
		*/
		StApiCError_AccessError = 0xC0000007,

		/*!
			@~english	@details	Timeout error.
			@~japanese	@details	???????????????????????????
		*/
		StApiCError_Timeout = 0xC0000008,

		/*!
			@~english	@details	Dynamic cast was zero.
			@~japanese	@details	???????????????????????????????????????
		*/
		StApiCError_DynamicCast = 0xC0000009,

		/*!
			@~english	@details	GenTL error.
			@~japanese	@details	GenTL?????????
		*/
		StApiCError_GenTLError = 0xC000000A,

#if defined(_WIN32)
		/*!
			@~english	@details	Win32 error.
			@~japanese	@details	Win32?????????
		*/
		StApiCError_Win32Error = 0xC000000B,
#elif defined(__linux__)
		/*!
			@~english	@details	Linux error.
			@~japanese	@details	Linux?????????
		*/
		StApiCError_LinuxError = 0xC000000B,
#elif defined(__APPLE__)
		/*!
			@~english	@details	MacOS error.
			@~japanese	@details	MacOS?????????
		*/
		StApiCError_OSXError = 0xC000000B,
#endif
	}EStApiCError_t, *PEStApiCError_t;

#define STAPI_CALL STAPI_API_EXPORTS EStApiCError_t STAPI_CALLTYPE
#define STAPI_PCALL(function) typedef  EStApiCError_t (STAPI_CALLTYPE *function)

	/*!
		@ingroup	StApiC_PublicImpl
		@enum EApiHandleType
		@~english	@brief	Handle type.
		@~english	@details	Handle type.
		@~japanese	@brief	?????????????????????
		@~japanese	@details	?????????????????????
	*/
	typedef enum _EApiHandleType
	{
		/*!
			@~english	@details	Unknown handle.
			@~japanese	@details	?????????????????????
		*/
		ApiHandleType_IUnknown,

		/*************************************************************************/
		/* GenICam */
		/*************************************************************************/
		/*!
			@~english	@details	gcstring handle.
			@~japanese	@details	gcstring ????????????
		*/
		/*ApiHandleType_GenICam_gcstring = 0x1000, */

		/*!
			@~english	@details	gcstring_vector handle.
			@~japanese	@details	gcstring_vector ????????????
		*/
		/*ApiHandleType_GenICam_gcstring_vector, */

		/*************************************************************************/
		/* GenApi */
		/*************************************************************************/
		ApiHandleType_GenApi_INodeMap = 0x2000,
		ApiHandleType_GenApi_IDeviceInfo,
		ApiHandleType_GenApi_IUserData,
		ApiHandleType_GenApi_ILock,
		
		ApiHandleType_GenApi_IBase,
		ApiHandleType_GenApi_INode,
		ApiHandleType_GenApi_IPort,
		ApiHandleType_GenApi_IChunkPort,
		ApiHandleType_GenApi_IValue,

		ApiHandleType_GenApi_IBoolean,
		ApiHandleType_GenApi_ICategory,
		ApiHandleType_GenApi_ICommand,
		ApiHandleType_GenApi_IEnumEntry,
		ApiHandleType_GenApi_IEnumeration,
		ApiHandleType_GenApi_IFloat,
		ApiHandleType_GenApi_IInteger,
		ApiHandleType_GenApi_IRegister,
		ApiHandleType_GenApi_ISelector,
		ApiHandleType_GenApi_IString,

		
	    /*	
		ApiHandleType_GenApi_IDestroy,
		ApiHandleType_GenApi_IPortRecorder,
		ApiHandleType_GenApi_IPortReplay,
		ApiHandleType_GenApi_IPortWriteList,
        */

		/*************************************************************************/
		/* StApi TL */
		/*************************************************************************/
		/*!
			@~english	@details	IStPort handle.
			@~japanese	@details	IStPort ????????????
		*/
		ApiHandleType_StApi_IStPort = 0x3000,

		/*!
			@~english	@details	IStPortURLInfo handle.
			@~japanese	@details	IStPortURLInfo ????????????
		*/
		ApiHandleType_StApi_IStPortURLInfo,

		/*!
			@~english	@details	IStPortInfo handle.
			@~japanese	@details	IStPortInfo ????????????
		*/
		ApiHandleType_StApi_IStPortInfo,

		/*!
			@~english	@details	IStSystem handle.
			@~japanese	@details	IStSystem ????????????
		*/
		ApiHandleType_StApi_IStSystem,

		/*!
			@~english	@details	IStSystemInfo handle.
			@~japanese	@details	IStSystemInfo ????????????
		*/
		ApiHandleType_StApi_IStSystemInfo,

		/*!
			@~english	@details	IStInterface handle.
			@~japanese	@details	IStInterface ????????????
		*/
		ApiHandleType_StApi_IStInterface,

		/*!
			@~english	@details	IStInterfaceInfo handle.
			@~japanese	@details	IStInterfaceInfo ????????????
		*/
		ApiHandleType_StApi_IStInterfaceInfo,


		/*!
		@~english	@details	IStDevice handle.
		@~japanese	@details	IStDevice ????????????
		*/
		ApiHandleType_StApi_IStDevice,

		/*!
			@~english	@details	IStDeviceInfo handle.
			@~japanese	@details	IStDeviceInfo ????????????
		*/
		ApiHandleType_StApi_IStDeviceInfo,

		/*!
			@~english	@details	IStDataStream handle.
			@~japanese	@details	IStDataStream ????????????
		*/
		ApiHandleType_StApi_IStDataStream,

		/*!
			@~english	@details	IStDataStreamInfo handle.
			@~japanese	@details	IStDataStreamInfo ????????????
		*/
		ApiHandleType_StApi_IStDataStreamInfo,

		/*!
			@~english	@details	IStStreamBuffer handle.
			@~japanese	@details	IStStreamBuffer ????????????
		*/
		ApiHandleType_StApi_IStStreamBuffer,

		/*!
			@~english	@details	IStStreamBufferInfo handle.
			@~japanese	@details	IStStreamBufferInfo ????????????
		*/
		ApiHandleType_StApi_IStStreamBufferInfo,

		/*!
			@~english	@details	IStImage handle.
			@~japanese	@details	IStImage ????????????
		*/
		ApiHandleType_StApi_IStImage,

		/*!
			@~english	@details	IStImageBuffer handle.
			@~japanese	@details	IStImageBuffer ????????????
		*/
		ApiHandleType_StApi_IStImageBuffer,

		/*!
			@~english	@details	IStPixelFormatInfo handle.
			@~japanese	@details	IStPixelFormatInfo ????????????
		*/
		ApiHandleType_StApi_IStPixelFormatInfo,

		/*!
			@~english	@details	IStPixelComponentInfo handle.
			@~japanese	@details	IStPixelComponentInfo ????????????
		*/
		ApiHandleType_StApi_IStPixelComponentInfo,

		/*!
		@~english	@details	IStPixelComponentValue handle.
		@~japanese	@details	IStPixelComponentValue ????????????
		*/
		ApiHandleType_StApi_IStPixelComponentValue,

		/*!
			@~english	@details	IStFeatureBag handle.
			@~japanese	@details	IStFeatureBag ????????????
		*/
		ApiHandleType_StApi_IStFeatureBag,

		/*!
			@~english	@details	IStAllocator handle.
			@~japanese	@details	IStAllocator ????????????
		*/
		ApiHandleType_StApi_IStAllocator,

		/*!
			@~english	@details	IStRegisteredCallback handle.
			@~japanese	@details	IStRegisteredCallback ????????????
		*/
		ApiHandleType_StApi_IStRegisteredCallback,

		/*!
			@~english	@details	IStCallbackParamBase handle.
			@~japanese	@details	IStCallbackParamBase ????????????
		*/
		ApiHandleType_StApi_IStCallbackParamBase,

		/*!
			@~english	@details	IStSpecificTypeAllocator handle.
			@~japanese	@details	IStSpecificTypeAllocator ????????????
		*/
		ApiHandleType_StApi_IStSpecificTypeAllocator,

		/*************************************************************************/
		/* StApi IP */
		/*************************************************************************/
		/*!
			@~english	@details	IStConverter handle.
			@~japanese	@details	IStConverter ????????????
		*/
		ApiHandleType_StApi_IStConverter = 0x4000,

		/*!
			@~english	@details	IStConverterInfo handle.
			@~japanese	@details	IStConverterInfo ????????????
		*/
		ApiHandleType_StApi_IStConverterInfo,

		/*!
			@~english	@details	IStFilter handle.
			@~japanese	@details	IStFilter ????????????
		*/
		ApiHandleType_StApi_IStFilter,

		/*!
			@~english	@details	IStFilterInfo handle.
			@~japanese	@details	IStFilterInfo ????????????
		*/
		ApiHandleType_StApi_IStFilterInfo,

		/*!
		@~english	@details	IStFiler handle.
		@~japanese	@details	IStFiler ????????????
		*/
		ApiHandleType_StApi_IStFiler,

		/*!
		@~english	@details	IStFilerInfo handle.
		@~japanese	@details	IStFilerInfo ????????????
		*/
		ApiHandleType_StApi_IStFilerInfo,

		/*!
			@~english	@details	IStPixelFormatConverter handle.
			@~japanese	@details	IStPixelFormatConverter ????????????
		*/
		ApiHandleType_StApi_IStPixelFormatConverter,

		/*!
			@~english	@details	IStReverseConverter handle.
			@~japanese	@details	IStReverseConverter ????????????
		*/
		ApiHandleType_StApi_IStReverseConverter,

		/*!
			@~english	@details	IStBalanceRatioFilter handle.
			@~japanese	@details	IStBalanceRatioFilter ????????????
		*/
		ApiHandleType_StApi_IStBalanceRatioFilter,

		/*!
			@~english	@details	IStChromaSuppressionFilter handle.
			@~japanese	@details	IStChromaSuppressionFilter ????????????
		*/
		ApiHandleType_StApi_IStChromaSuppressionFilter,

		/*!
			@~english	@details	IStColorTransformationFilter handle.
			@~japanese	@details	IStColorTransformationFilter ????????????
		*/
		ApiHandleType_StApi_IStColorTransformationFilter,

		/*!
			@~english	@details	IStEdgeEnhancementFilter handle.
			@~japanese	@details	IStEdgeEnhancementFilter ????????????
		*/
		ApiHandleType_StApi_IStEdgeEnhancementFilter,

		/*!
			@~english	@details	IStFlatFieldCorrectionFilter handle.
			@~japanese	@details	IStFlatFieldCorrectionFilter ????????????
		*/
		ApiHandleType_StApi_IStFlatFieldCorrectionFilter,

		/*!
			@~english	@details	IStGammaCorrectionFilter handle.
			@~japanese	@details	IStGammaCorrectionFilter ????????????
		*/
		ApiHandleType_StApi_IStGammaCorrectionFilter,

		/*!
			@~english	@details	IStNoiseReductionFilter handle.
			@~japanese	@details	IStNoiseReductionFilter ????????????
		*/
		ApiHandleType_StApi_IStNoiseReductionFilter,

		/*!
			@~english	@details	IStSNMeasurementFilter handle.
			@~japanese	@details	IStSNMeasurementFilter ????????????
		*/
		ApiHandleType_StApi_IStSNMeasurementFilter,

		/*!
			@~english	@details	IStSNMeasurementResult handle.
			@~japanese	@details	IStSNMeasurementResult ????????????
		*/
		ApiHandleType_StApi_IStSNMeasurementResult,

		/*!
			@~english	@details	IStStillImageFiler handle.
			@~japanese	@details	IStStillImageFiler ????????????
		*/
		ApiHandleType_StApi_IStStillImageFiler,

		/*!
			@~english	@details	IStVideoFiler handle.
			@~japanese	@details	IStVideoFiler ????????????
		*/
		ApiHandleType_StApi_IStVideoFiler,

		/*!
			@~english	@details	IStGraphData handle.
			@~japanese	@details	IStGraphData ????????????
		*/
		ApiHandleType_StApi_IStGraphData,

		/*!
			@~english	@details	IStGraphDataBuffer handle.
			@~japanese	@details	IStGraphDataBuffer ????????????
		*/
		ApiHandleType_StApi_IStGraphDataBuffer,

		/*!
			@~english	@details	IStGraphDataBufferResizable handle.
			@~japanese	@details	IStGraphDataBufferResizable ????????????
		*/
		ApiHandleType_StApi_IStGraphDataBufferResizable,

		/*!
			@~english	@details	IStGraphDataBufferList handle.
			@~japanese	@details	IStGraphDataBufferList ????????????
		*/
		ApiHandleType_StApi_IStGraphDataBufferList,

		/*!
			@~english	@details	IStGraphDataBufferListResizable handle.
			@~japanese	@details	IStGraphDataBufferListResizable ????????????
		*/
		ApiHandleType_StApi_IStGraphDataBufferListResizable,

		/*!
			@~english	@details	IStGraphDataFilter handle.
			@~japanese	@details	IStGraphDataFilter ????????????
		*/
		ApiHandleType_StApi_IStGraphDataFilter,

		/*!
			@~english	@details	IStImageAveragingFilter handle.
			@~japanese	@details	IStImageAveragingFilter ????????????
		*/
		ApiHandleType_StApi_IStImageAveragingFilter,

		/*!
			@~english	@details	IStDefectivePixelDetectionFilter handle.
			@~japanese	@details	IStDefectivePixelDetectionFilter ????????????
		*/
		ApiHandleType_StApi_IStDefectivePixelDetectionFilter,

		/*************************************************************************/
		/* StApi GUI */
		/*************************************************************************/
		/*!
			@~english	@details	IStWnd handle.
			@~japanese	@details	IStWnd ????????????
		*/
		ApiHandleType_StApi_IStWnd = 0x5000,

		
		/*!
			@~english	@details	IStWndInf handle.
			@~japanese	@details	IStWndInf ????????????
		*/
		ApiHandleType_StApi_IStWndInfo,
		
		/*!
			@~english	@details	IStDeviceSelectionWnd handle.
			@~japanese	@details	IStDeviceSelectionWnd ????????????
		*/
		ApiHandleType_StApi_IStDeviceSelectionWnd,
		
		/*!
			@~english	@details	IStNodeMapDisplayWnd handle.
			@~japanese	@details	IStNodeMapDisplayWnd ????????????
		*/
		ApiHandleType_StApi_IStNodeMapDisplayWnd,
		
		/*!
			@~english	@details	IStImageDisplayWnd handle.
			@~japanese	@details	IStImageDisplayWnd ????????????
		*/
		ApiHandleType_StApi_IStImageDisplayWnd,
		
		/*!
			@~english	@details	RegisteredINode handle.
			@~japanese	@details	RegisteredINode ????????????
		*/
		ApiHandleType_StApi_RegisteredINode,
		
		/*!
			@~english	@details	IStDrawingTool handle.
			@~japanese	@details	IStDrawingTool ????????????
		*/
		ApiHandleType_StApi_IStDrawingTool,
		
		/*!
			@~english	@details	IStGraphDisplayWnd handle.
			@~japanese	@details	IStGraphDisplayWnd ????????????
		*/
		ApiHandleType_StApi_IStGraphDisplayWnd,
	}EApiHandleType, *PEApiHandleType;

	/*************************************************************************/
	/* struct StApiHandle_t */
	/*************************************************************************/
	/*!
		@ingroup StApiC_PublicImpl
		@struct StApiHandle_t
		@~english	@brief	Struct for genaral handle.
		@~english	@details	Struct for genaral handle.
		@~japanese	@brief	????????????????????????????????????
		@~japanese	@details	????????????????????????????????????
	*/
	typedef struct _StApiHandle_t
	{

		/*!
			@~english	@details	Handle.
			@~japanese	@details	????????????
		*/
		void *Handle;

		/*!
			@~english	@details	Handle type.
			@~japanese	@details	?????????????????????
		*/
		EApiHandleType	HandleType;
	} StApiHandle_t, *PStApiHandle_t;

	typedef void (STAPI_CALLTYPE fNodeCallbackFunc_t)(PStApiHandle_t psINodeHandle, void* pvContext);
	typedef void (STAPI_CALLTYPE fStCallbackFunc_t)(PStApiHandle_t, void*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STAPI_C_DEF_H__ */
