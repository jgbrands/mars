#include <iostream>
#include <cstdlib>

#include <mars/drm/DrmPresentationBackend.hpp>

int main()
{
	try {
		// Open up the DRM device and check capabilities
		mars::DrmPresentationBackend presentationBackend("/dev/dri/card0");
		for (int i = 0; i < 1000000; i++) {
			presentationBackend.poll_events();
		}
	} catch (std::exception& e) {
		std::cerr << "mars has encountered an unexpected error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
