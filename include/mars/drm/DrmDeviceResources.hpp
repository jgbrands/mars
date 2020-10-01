//
// Created by Jesse on 9/27/2020.
//

#pragma once

#include <vector>

#include <xf86drmMode.h>

#include <mars/drm/DrmConnector.hpp>
#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmEncoder.hpp>
#include <mars/drm/DrmVideoDisplayController.hpp>

namespace mars
{
	class DrmDeviceResources
	{
		DrmDevice& device;
		DrmUniqueDeviceResourcesPtr resources;
		std::vector<DrmConnector> connectors;
		std::vector<DrmEncoder> encoders;
		std::vector<DrmVideoDisplayController> controllers;

	public:
		explicit DrmDeviceResources(DrmDevice& device);

		[[nodiscard]] std::vector<DrmConnector>& get_connectors()
		{
			return connectors;
		}

		[[nodiscard]] const std::vector<DrmEncoder>& get_encoders() const
		{
			return encoders;
		}

		[[nodiscard]] const std::vector<DrmVideoDisplayController>& get_controllers() const
		{
			return controllers;
		}
	};
}