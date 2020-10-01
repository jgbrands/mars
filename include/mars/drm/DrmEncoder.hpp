//
// Created by Jesse on 9/26/2020.
//

#pragma once

#include <cstdint>
#include <xf86drmMode.h>

#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmResourcePtr.hpp>
#include <mars/drm/DrmVideoDisplayController.hpp>

namespace mars
{
	class DrmEncoder
	{
		DrmDevice& device;
		uint32_t encoderId;
		DrmUniqueEncoderPtr encoder;

	public:
		DrmEncoder(DrmDevice& device, uint32_t encoderId);

		drmModeEncoderPtr operator->() const noexcept
		{
			return encoder.get();
		}

		[[nodiscard]] DrmVideoDisplayController get_default_vdc();

		/**
		 * Checks whether a VDC is compatible with the encoder.
		 * @param index
		 * @return
		 */
		[[nodiscard]] bool compatible_with_vdc(const DrmVideoDisplayController& vdc) const noexcept;

		[[nodiscard]] uint32_t get_encoder_id() const noexcept
		{
			return encoderId;
		}

		[[nodiscard]] bool is_available() const noexcept
		{
			return encoder != nullptr;
		}
	};
};