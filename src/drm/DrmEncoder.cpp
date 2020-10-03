//
// Created by Jesse on 9/27/2020.
//

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
