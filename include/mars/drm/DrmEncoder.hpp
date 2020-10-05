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