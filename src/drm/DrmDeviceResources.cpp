//
// Created by Jesse on 9/27/2020.
//

#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmDeviceResources.hpp>

mars::DrmDeviceResources::DrmDeviceResources(mars::DrmDevice& device)
		: device(device),
		  resources(drmModeGetResources(device.get_fd()))
{
	if (!resources) {
		throw std::runtime_error("could not get DRM device resources");
	}

	// Start enumerating our connectors
	connectors.reserve(resources->count_connectors);
	for (int i = 0; i < resources->count_connectors; ++i) {
		connectors.emplace_back(device, resources->connectors[i]);
	}

	encoders.reserve(resources->count_encoders);
	for (int i = 0; i < resources->count_encoders; ++i) {
		encoders.emplace_back(device, resources->encoders[i]);
	}

	controllers.reserve(resources->count_crtcs);
	for (int i =0; i < resources->count_crtcs; ++i) {
		controllers.emplace_back(device, resources->crtcs[i], i);
	}
}
