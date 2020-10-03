#include <iostream>
#include <cstdlib>

#include <libinput.h>
#include <systemd/sd-daemon.h>
#include <zconf.h>
#include <sdbus-c++/sdbus-c++.h>

#include <mars/session/LogindSession.hpp>
#include <mars/drm/DrmPresentationSystem.hpp>

int main()
{
	if (geteuid() == 0) {
		std::cerr << "mars: cannot be run as root" << std::endl;
		return EXIT_FAILURE;
	}

	if (!sd_booted()) {
		std::cerr << "mars: session not found, exiting" << std::endl;
		return EXIT_FAILURE;
	}

	mars::LogindSession session;

	try {
		// Open up the DRM device and check capabilities
		mars::DrmPresentationSystem presentationSystem("/dev/dri/card0");
		for (int i = 0; i < 1; i++) {
			presentationSystem.poll_events();
		}
	} catch (std::exception& e) {
		std::cerr << "mars has encountered an unexpected error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
