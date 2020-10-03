//
// Created by Jesse on 9/30/2020.
//

#pragma once

#include <xf86drm.h>
#include <xf86drmMode.h>

#include <functional>
#include <memory>

namespace mars
{
	template <auto fn>
	using DrmDeleterFn = std::integral_constant<decltype(fn), fn>;

	template<typename T, void F(T*)>
	using DrmUniqueResourcePtr = std::unique_ptr<T, DrmDeleterFn<F>>;

	using DrmUniqueDeviceResourcesPtr = DrmUniqueResourcePtr<drmModeRes, drmModeFreeResources>;
	using DrmUniqueConnectorPtr = DrmUniqueResourcePtr<drmModeConnector, drmModeFreeConnector>;
	using DrmUniqueEncoderPtr = DrmUniqueResourcePtr<drmModeEncoder, drmModeFreeEncoder>;
	using DrmUniquePlanePtr = DrmUniqueResourcePtr<drmModePlane, drmModeFreePlane>;
	using DrmUniquePlaneResourcesPtr = DrmUniqueResourcePtr<drmModePlaneRes, drmModeFreePlaneResources>;
	using DrmUniqueObjectPropertiesPtr = DrmUniqueResourcePtr<drmModeObjectProperties, drmModeFreeObjectProperties>;
	using DrmUniquePropertyPtr = DrmUniqueResourcePtr<drmModePropertyRes, drmModeFreeProperty>;
	using DrmUniqueVideoDisplayControllerPtr = DrmUniqueResourcePtr<drmModeCrtc, drmModeFreeCrtc>;
	using DrmUniqueAtomicRequestPtr = DrmUniqueResourcePtr<drmModeAtomicReq, drmModeAtomicFree>;
}