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

#include <systemd/sd-daemon.h>
#include <unistd.h>

#include <mars/drm/DrmPresentationSystem.hpp>
#include <mars/session/LogindSession.hpp>
#include <mars/Log.hpp>
#include <libudev.h>

int main()
{
	auto* udev = udev_new();
	auto* udevEnum = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(udevEnum, "drm");
	udev_enumerate_add_match_property(udevEnum, "DEVTYPE", "drm_minor");
	udev_enumerate_scan_devices(udevEnum);

	auto* devices = udev_enumerate_get_list_entry(udevEnum);
	struct udev_list_entry* entry = nullptr;

	udev_list_entry_foreach(entry, devices) {
		const char* path = udev_list_entry_get_name(entry);
		auto* device = udev_device_new_from_device_id(udev, path);

		auto* devNode = udev_device_get_devnode(device);
		std::cout << path;

		if (devNode) {
			std::cout << ": " << devNode << std::endl;
		} else {
			std::cout << std::endl;
		}
	}

	if (!sd_booted()) {
		std::cerr << "mars: session not found, exiting" << std::endl;
		return EXIT_FAILURE;
	}

	if (geteuid() == 0) {
		std::cerr << "mars: cannot be run as root" << std::endl;
		return EXIT_FAILURE;
	}

	auto log = std::make_shared<mars::Log>("mars", true);

	try {
		mars::LogindSession session(log);

		// Open up the DRM device and check capabilities
		mars::DrmPresentationSystem presentationSystem(log, "/dev/dri/card0");
		for (int i = 0; i < 1; i++) {
			presentationSystem.poll_events();
		}
	} catch (std::exception& e) {
		log->error(e);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
