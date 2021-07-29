// Copyright (c) 2020 Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#if PLATFORM_WINDOWS || PLATFORM_HOLOLENS
#include "OpenXRCommon.h"
#include "OpenXRCore.h"
#include "IOpenXRARModule.h"
#include "IOpenXRARTrackedGeometryHolder.h"

#include "HeadMountedDisplayTypes.h"
#include "ARTypes.h"
#include "ARSessionConfig.h"
#include "WindowsMixedRealityInteropUtility.h"

#include "IXRTrackingSystem.h"
#include "TrackedGeometryCollision.h"

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/AllowWindowsPlatformAtomics.h"
#include "Windows/PreWindowsApi.h"

#include <mutex>
#include <map>

#include <winrt/windows.foundation.Collections.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <unknwn.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Perception.Spatial.h>
#include <winrt/Windows.Perception.Spatial.Surfaces.h>

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformAtomics.h"
#include "Windows/HideWindowsPlatformTypes.h"

#include "Misc/EngineVersionComparison.h"

namespace MicrosoftOpenXR
{
	class WMRAnchorLocalizationData : public TSharedFromThis<WMRAnchorLocalizationData>
	{
	public:
		WMRAnchorLocalizationData(XrSpace AnchorSpace, winrt::Windows::Perception::Spatial::SpatialCoordinateSystem CoordinateSystem)
			: AnchorSpace(AnchorSpace)
			, CoordinateSystem(CoordinateSystem)
		{
		}

		~WMRAnchorLocalizationData()
		{
			xrDestroySpace(AnchorSpace);
		}

		XrSpace AnchorSpace;
		winrt::Windows::Perception::Spatial::SpatialCoordinateSystem CoordinateSystem;
	};

	struct MeshLocalizationData
	{
		winrt::Windows::Foundation::DateTime UpdateTime = winrt::Windows::Foundation::DateTime::min();
		FTransform LastKnownTransform = FTransform::Identity;
		EARTrackingState LastKnownTrackingState = EARTrackingState::NotTracking;
		winrt::Windows::Perception::Spatial::SpatialCoordinateSystem CoordinateSystem;
		TrackedGeometryCollision CollisionInfo;
	};

	class FSpatialMappingPlugin : public IOpenXRExtensionPlugin, public IOpenXRCustomCaptureSupport
	{
	public:
		void Register();
		void Unregister();

		bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;

		const void* OnCreateSession(XrInstance InInstance, XrSystemId InSystem, const void* InNext) override;
		const void* OnBeginSession(XrSession InSession, const void* InNext) override;
		void UpdateDeviceLocations(XrSession InSession, XrTime DisplayTime, XrSpace TrackingSpace) override;

		bool OnToggleARCapture(const bool On) override;
		TArray<FARTraceResult> OnLineTraceTrackedObjects(const TSharedPtr<FARSupportInterface, ESPMode::ThreadSafe> ARCompositionComponent, const FVector Start, const FVector End, const EARLineTraceChannels TraceChannels) override;
		IOpenXRCustomCaptureSupport* GetCustomCaptureSupport(const EARCaptureType CaptureType) override;

	private:
		std::mutex MeshRefsLock;
		winrt::event_token OnChangeEventToken;

		bool bShouldStartSpatialMapping = false;
		bool bGenerateSRMeshes = false;
		float VolumeSize = 20.0f;
		float TriangleDensity = 500.0f;
		unsigned int MeshToLocalizeThisFrame = 0;

		const int WarnAfterThisManyMeshes = 100;

		PFN_xrCreateSpatialAnchorFromPerceptionAnchorMSFT xrCreateSpatialAnchorFromPerceptionAnchorMSFT;
		PFN_xrCreateSpatialAnchorSpaceMSFT xrCreateSpatialAnchorSpaceMSFT;
		PFN_xrDestroySpatialAnchorMSFT xrDestroySpatialAnchorMSFT;

		winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Perception::Spatial::SpatialPerceptionAccessStatus> RequestAccessOperation = nullptr;

		// Map of all observed mesh ID's to their coordinate systems.  Used for localizing previously observed meshes.
		std::map<FGuid, MeshLocalizationData> UniqueMeshes;
		// Map of anchor data to use when localizing new spatial mapping meshes.
		std::map<FGuid, TSharedPtr<WMRAnchorLocalizationData>> AnchorLocalizationData;

		IXRTrackingSystem* XRTrackingSystem = nullptr;
		IOpenXRARTrackedMeshHolder* TrackedMeshHolder = nullptr;
		winrt::Windows::Perception::Spatial::Surfaces::SpatialSurfaceObserver SurfaceObserver = nullptr;
		winrt::Windows::Perception::Spatial::Surfaces::SpatialSurfaceMeshOptions MeshOptions = nullptr;

		void OnStartARSession(class UARSessionConfig* SessionConfig) override;

		// Returns true if spatial mapping is desired and ready to start but not yet enabled or started.
		bool ShouldStartSpatialMapping();
		bool IsSpatialMappingStartingOrStarted();
		bool StartMeshObserver();
		void StopMeshObserver();

		bool FindMeshTransform(XrSpace AnchorSpace, XrTime DisplayTime, XrSpace TrackingSpace, FTransform MeshToCachedAnchorTransform, FTransform& Transform, bool& IsTracking);
		bool GetTransformBetweenCoordinateSystems(winrt::Windows::Perception::Spatial::SpatialCoordinateSystem From, winrt::Windows::Perception::Spatial::SpatialCoordinateSystem To, FTransform& Transform);
		bool LocateSpatialMeshInTrackingSpace(const FGuid& MeshID, winrt::Windows::Perception::Spatial::SpatialCoordinateSystem MeshCoordinateSystem, XrSession InSession, XrTime DisplayTime, XrSpace TrackingSpace, FTransform& Transform);

		void UpdateBoundingVolume();
		void CopyMeshData(FOpenXRMeshUpdate* MeshUpdate, winrt::Windows::Perception::Spatial::Surfaces::SpatialSurfaceMesh SurfaceMesh);
		void OnSurfacesChanged(winrt::Windows::Perception::Spatial::Surfaces::SpatialSurfaceObserver Observer, winrt::Windows::Foundation::IInspectable);
	};
}	 // namespace MicrosoftOpenXR

#endif //PLATFORM_WINDOWS || PLATFORM_HOLOLENS
