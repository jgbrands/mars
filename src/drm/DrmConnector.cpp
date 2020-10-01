//
// Created by Jesse on 9/26/2020.
//

#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmConnector.hpp>

mars::DrmConnector::DrmConnector(mars::DrmDevice& device, uint32_t connectorId)
		: device(device),
		  connector(drmModeGetConnector(device.get_fd(), connectorId))
{
	if (!connector) {
		throw std::runtime_error("could not retrieve DRM connector");
	}
}


void mars::DrmConnector::print_video_modes() const noexcept
{
	for (int i = 0; i < connector->count_modes; ++i) {
		std::cout << connector->modes[i].name << "\n";
	}
}

drmModeModeInfo mars::DrmConnector::preferred_video_mode() const
{
	if (connector->count_modes == 0) {
		throw std::runtime_error("cannot get preferred mode; no modes for connector");
	}

	for (int i = 0; i < connector->count_modes; ++i) {
		if (std::string(connector->modes[i].name) == "preferred") {
			return connector->modes[i];
		}
	}

	// TODO: Find best mode instead
	return connector->modes[0]; // Just the first, then
}

mars::DrmEncoder mars::DrmConnector::get_current_encoder() const
{
	return mars::DrmEncoder(device, connector->encoder_id);
}

drmModeModeInfo mars::DrmConnector::get_video_mode(int id) const
{
	return connector->modes[id];
}

std::vector<mars::DrmEncoder> mars::DrmConnector::get_encoders() const
{
	std::vector<DrmEncoder> encoders;
	encoders.reserve(connector->count_encoders);
	for (int i = 0; i < connector->count_encoders; ++i) {
		encoders.emplace_back(device, connector->encoders[i]);
	}

	return encoders;
}
