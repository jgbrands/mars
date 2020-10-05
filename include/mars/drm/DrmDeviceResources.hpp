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