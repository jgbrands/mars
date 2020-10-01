//
// Created by Jesse on 9/30/2020.
//

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