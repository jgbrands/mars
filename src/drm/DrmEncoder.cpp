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

#include <stdexcept>

#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmEncoder.hpp>
#include <mars/drm/DrmDeviceResources.hpp>

mars::DrmEncoder::DrmEncoder(mars::DrmDevice& device, uint32_t encoderId)
		: device(device),
		  encoderId(encoderId),
		  encoder(drmModeGetEncoder(device.get_fd(), encoderId))
{
	if (encoder == nullptr) {
		throw std::runtime_error("could not get encoder");
	}
}

bool mars::DrmEncoder::compatible_with_vdc(const mars::DrmVideoDisplayController& vdc) const noexcept
{
	return encoder->possible_crtcs & (1 << vdc.get_index());
}

mars::DrmVideoDisplayController mars::DrmEncoder::get_default_vdc()
{
	return DrmVideoDisplayController(device, encoder->crtc_id);
}
