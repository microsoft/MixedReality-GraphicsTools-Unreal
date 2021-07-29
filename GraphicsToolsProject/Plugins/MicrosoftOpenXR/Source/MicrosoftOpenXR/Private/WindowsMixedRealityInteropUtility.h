// Copyright (c) 2020 Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#if PLATFORM_WINDOWS || PLATFORM_HOLOLENS
#include "CoreMinimal.h"
#include "HAL\UnrealMemory.h"

#include "Windows/WindowsHWrapper.h"
#include "HeadMountedDisplayTypes.h"

#include <DirectXMath.h>
#include <unknwn.h>
#include <winrt/base.h>


namespace MicrosoftOpenXR
{
	class WMRUtility
	{
	public:
		// Convert between DirectX XMMatrix to Unreal FMatrix.
		static FORCEINLINE FMatrix ToFMatrix(DirectX::XMMATRIX& M)
		{
			DirectX::XMFLOAT4X4 dst;
			DirectX::XMStoreFloat4x4(&dst, M);

			return FMatrix(
				FPlane(dst._11, dst._21, dst._31, dst._41),
				FPlane(dst._12, dst._22, dst._32, dst._42),
				FPlane(dst._13, dst._23, dst._33, dst._43),
				FPlane(dst._14, dst._24, dst._34, dst._44));
		}

		static FORCEINLINE FMatrix ToFMatrix(DirectX::XMFLOAT4X4& M)
		{
			return FMatrix(
				FPlane(M._11, M._21, M._31, M._41),
				FPlane(M._12, M._22, M._32, M._42),
				FPlane(M._13, M._23, M._33, M._43),
				FPlane(M._14, M._24, M._34, M._44));
		}

		static FORCEINLINE FTransform FromMixedRealityTransform(const DirectX::XMMATRIX& M, float InScale = 1.0f)
		{
			DirectX::XMVECTOR Scale;
			DirectX::XMVECTOR Rotation;
			DirectX::XMVECTOR Translation;
			DirectX::XMMatrixDecompose(&Scale, &Rotation, &Translation, M);

			return FTransform(FromXMVectorRotation(Rotation), FromXMVectorTranslation(Translation, InScale), FromXMVectorScale(Scale));
		}

		static FORCEINLINE FVector FromMixedRealityVector(DirectX::XMFLOAT3 pos)
		{
			return FVector(
				-1.0f * pos.z,
				pos.x,
				pos.y);
		}
		
		static FORCEINLINE DirectX::XMFLOAT3 ToMixedRealityVector(FVector pos)
		{
			return DirectX::XMFLOAT3(
				pos.Y,
				pos.Z,
				-1.0f * pos.X);
		}

		static FORCEINLINE FVector FromMixedRealityScale(DirectX::XMFLOAT3 pos)
		{
			return FVector(
				pos.z,
				pos.x,
				pos.y);
		}

		static FORCEINLINE FVector FromMixedRealityScale(winrt::Windows::Foundation::Numerics::float3 pos)
		{
			return FVector(
				pos.z,
				pos.x,
				pos.y);
		}

		static FORCEINLINE DirectX::XMFLOAT3 ToMixedRealityScale(FVector pos)
		{
			return DirectX::XMFLOAT3(
				pos.Y,
				pos.Z,
				pos.X);
		}


		static FORCEINLINE FQuat FromMixedRealityQuaternion(DirectX::XMFLOAT4 rot)
		{
			FQuat quaternion(
				-1.0f * rot.z,
				rot.x,
				rot.y,
				-1.0f * rot.w);
			quaternion.Normalize();

			return quaternion;
		}

		static FORCEINLINE DirectX::XMFLOAT4 ToMixedRealityQuaternion(FQuat rot)
		{
			// Windows api IsNormalized checks fail on a negative identity quaternion.
			if (rot == FQuat::Identity)
			{
				return DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
			}

			DirectX::XMVECTOR v = DirectX::XMVectorSet(
				rot.Y,
				rot.Z,
				-1.0f * rot.X,
				-1.0f * rot.W);
			DirectX::XMQuaternionNormalize(v);

			DirectX::XMFLOAT4 quatf4out;
			DirectX::XMStoreFloat4(&quatf4out, v);
			return quatf4out;
		}

		static FORCEINLINE FVector FromFloat3(winrt::Windows::Foundation::Numerics::float3 pos, float scale = 1.0f)
		{
			return FVector(
				-1.0f * pos.z,
				pos.x,
				pos.y) * scale;
		}

		static FORCEINLINE FVector FromXMVectorTranslation(DirectX::XMVECTOR InValue, float scale = 1.0f)
		{
			InValue = DirectX::XMVectorMultiply(InValue, DirectX::XMVectorSet(scale, scale, -1 * scale, scale));
			InValue = DirectX::XMVectorSwizzle(InValue, 2, 0, 1, 3);

			DirectX::XMFLOAT3 Dest;
			DirectX::XMStoreFloat3(&Dest, InValue);
			return FVector(Dest.x, Dest.y, Dest.z);
		}

		static FORCEINLINE FQuat FromXMVectorRotation(DirectX::XMVECTOR InValue)
		{
			DirectX::XMFLOAT4 Dest;
			DirectX::XMStoreFloat4(&Dest, InValue);

			return FromMixedRealityQuaternion(Dest);
		}

		static FORCEINLINE FVector FromXMVectorScale(DirectX::XMVECTOR InValue)
		{
			InValue = DirectX::XMVectorSwizzle(InValue, 2, 0, 1, 3);

			DirectX::XMFLOAT3 Dest;
			DirectX::XMStoreFloat3(&Dest, InValue);
			return FVector(Dest.x, Dest.y, Dest.z);
		}

		static FORCEINLINE FVector2D FromFloat2(winrt::Windows::Foundation::Numerics::float2 pos)
		{
			return FVector2D(pos.x, pos.y);
		}



		static FORCEINLINE FGuid GUIDToFGuid(const winrt::guid& InGuid)
		{
			check(sizeof(FGuid) == sizeof(winrt::guid));

			FGuid OutGuid;
			FMemory::Memcpy(&OutGuid, &InGuid, sizeof(FGuid));
			return OutGuid;
		}
	};
}


#endif

