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
