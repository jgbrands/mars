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

#include <cstdint>
#include <stdexcept>
#include <vector>

#include <xf86drmMode.h>
#include <iostream>

#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmEncoder.hpp>
#include <mars/drm/DrmResourcePtr.hpp>
#include <mars/drm/DrmObjectProperties.hpp>

namespace mars
{
	class DrmConnector
	{
	private:
		DrmDevice& device;
		DrmUniqueConnectorPtr connector;

	public:
		DrmConnector(DrmDevice& device, uint32_t connectorId);

		drmModeConnectorPtr operator->() const noexcept
		{
			return connector.get();
		}

		/**
		 * Writes all video modes to stdout
		 */
		void print_video_modes() const noexcept;

		/**
		 * Finds the preferred video mode if one exists
		 * @return the preferred video mode information
		 */
		[[nodiscard]] drmModeModeInfo preferred_video_mode() const;

		[[nodiscard]] drmModeModeInfo get_video_mode(int id) const;

		[[nodiscard]] DrmEncoder get_current_encoder() const;

		[[nodiscard]] std::vector<DrmEncoder> get_encoders() const;

		[[nodiscard]] uint32_t get_id() const
		{
			return connector->connector_id;
		}

		[[nodiscard]] DrmObjectProperties properties() const
		{
			return DrmObjectProperties(device, connector->connector_id, DRM_MODE_OBJECT_CONNECTOR);
		}

		void set_property(DrmAtomicRequest& req, const std::string& name, uint64_t value)
		{
			DrmObjectProperties(device, connector->connector_id, DRM_MODE_OBJECT_CONNECTOR)
					.set_value(name, value, req);
		}
	};
}