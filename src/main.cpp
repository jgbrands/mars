#include <iostream>
#include <cstdlib>

#include <xf86drmMode.h>

#include <mars/drm/DrmConnector.hpp>
#include <mars/drm/DrmDeviceResources.hpp>
#include <mars/drm/DrmFrameBuffer.hpp>
#include <mars/drm/DrmSurface.hpp>
#include <unistd.h>
#include <cmath>

int main()
{
	try {
		// Open up the DRM device and check capabilities
		mars::DrmDevice device;
		if (!device.set_capability(DRM_CLIENT_CAP_ATOMIC, true)) {
			std::cerr << "Could not set atomic capability on DRM device" << std::endl;
			return EXIT_FAILURE;
		}

		auto capabilities = device.get_capabilities();
		if (!capabilities.dumbBuffers) {
			std::cerr << "DRM device does not support dumb buffers" << std::endl;
			return EXIT_FAILURE;
		}

		if (!capabilities.vdcInVBlankEvent) {
			std::cerr << "DRM device does not support atomic KMS API" << std::endl;
			return EXIT_FAILURE;
		}

		// We got the device, lets see what connectors this card has
		std::vector<mars::DrmSurface> surfaces;
		mars::DrmDeviceResources resources(device);
		for (auto& connector: resources.get_connectors()) {
			if (connector->connection != DRM_MODE_CONNECTED) {
				continue;
			}

			auto encoder = connector.get_current_encoder();
			auto mode = connector.preferred_video_mode();
			if (!encoder.is_available()) {
				std::cerr << "DRM device connector has no available encoder" << std::endl;
				return EXIT_FAILURE;
			}

			mars::DrmSurface surface(device, connector, encoder, mode);
		}
	} catch ( std::exception& e) {
		std::cerr << "mars has encountered an unexpected error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
