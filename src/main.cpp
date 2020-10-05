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
