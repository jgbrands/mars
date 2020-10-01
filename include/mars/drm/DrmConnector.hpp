//
// Created by Jesse on 9/26/2020.
//

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

		void set_property(drmModeAtomicReq* req, const std::string& name, uint64_t value)
		{
			DrmObjectProperties(device, connector->connector_id, DRM_MODE_OBJECT_CONNECTOR)
					.set_value(name, value, req);
		}
	};
}