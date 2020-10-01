//
// Created by Jesse on 9/30/2020.
//

#include <mars/drm/DrmVideoDisplayController.hpp>
#include <mars/drm/DrmDeviceResources.hpp>

mars::DrmVideoDisplayController::DrmVideoDisplayController(mars::DrmDevice& device, uint32_t id)
		: device(device), id(id),
		  crtc(drmModeGetCrtc(device.get_fd(), id))
{
	if (crtc == nullptr) {
		throw std::runtime_error("could not get DRM video display controller");
	}
	DrmDeviceResources resources(device);
	for (auto& vdc: resources.get_controllers()) {
		if (vdc.get_id() == id) {
			index = vdc.get_index();
			return;
		}
	}
	throw std::runtime_error("cannot find matching id for DRM video display controller");
}
