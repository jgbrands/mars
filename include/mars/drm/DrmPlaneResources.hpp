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

#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmResourcePtr.hpp>
#include <mars/drm/DrmPlane.hpp>

namespace mars
{


	class DrmPlaneResources
	{
		DrmDevice& device;
		DrmUniquePlaneResourcesPtr resources;

	public:
		explicit DrmPlaneResources(DrmDevice& device)
				: device(device),
				  resources(drmModeGetPlaneResources(device.get_fd()))
		{
			if (resources == nullptr) {
				throw std::runtime_error("could not get plane resources for DRM device");
			}
		}

		std::vector<DrmPlane> get_planes() {
			std::vector<DrmPlane> planes;
			planes.reserve(resources->count_planes);
			for (int i = 0; i < resources->count_planes; ++i) {
				planes.emplace_back(device, resources->planes[i]);
			}
			return planes;
		}
	};
}