//
// Created by Jesse on 9/30/2020.
//

#pragma once

#include <stdexcept>

#include <xf86drmMode.h>

#include <mars/drm/DrmResourcePtr.hpp>
#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmObjectProperties.hpp>

namespace mars
{
	class DrmPlane
	{
		DrmDevice& device;
		DrmUniquePlanePtr plane;

	public:
		DrmPlane(DrmDevice& device, uint32_t id)
				: device(device),
				  plane(drmModeGetPlane(device.get_fd(), id))
		{
			if (plane == nullptr) {
				throw std::runtime_error("could not get DRM device plane");
			}
		}

		[[nodiscard]] bool is_compatible_with_vdc(const DrmVideoDisplayController& vdc) const noexcept
		{
			return plane->possible_crtcs & (1 << vdc.get_index());
		}

		[[nodiscard]] DrmObjectProperties properties() const {
			return DrmObjectProperties(device, plane->plane_id, DRM_MODE_OBJECT_PLANE);
		}

		void set_property(DrmAtomicRequest& req, const std::string& name, uint64_t value)
		{
			DrmObjectProperties(device, plane->plane_id, DRM_MODE_OBJECT_PLANE)
					.set_value(name, value, req);
		}
	};
}