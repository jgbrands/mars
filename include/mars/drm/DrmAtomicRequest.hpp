//
// Created by Jesse on 10/3/2020.
//

#pragma once

#include <xf86drmMode.h>

#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmResourcePtr.hpp>

namespace mars
{
	class DrmAtomicRequest
	{
		DrmDevice& device;
		DrmUniqueAtomicRequestPtr req;
		void* userData;

	public:
		explicit DrmAtomicRequest(DrmDevice& device, void* userData = nullptr);

		int commit(uint32_t flags);

		[[nodiscard]] drmModeAtomicReq* get() const noexcept
		{
			return req.get();
		}
	};
}