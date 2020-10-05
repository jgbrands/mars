/**
 *    Copyright 2020 Jesse Gerard Brands
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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