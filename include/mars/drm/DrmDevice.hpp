//
// Created by Jesse on 9/26/2020.
//

#pragma once

#include <string>
#include <set>

namespace mars
{
	struct DrmDeviceCapabilities
	{
		bool dumbBuffers;
		uint64_t dumbBufferPreferredDepth;
		bool dumbBufferPreferShadow;
		bool vBlankHighCRTC;
		bool vdcInVBlankEvent;
		uint64_t cursorWidth;
		uint64_t cursorHeight;
		bool prime;
	};

	class DrmDevice
	{
		int fd;
		std::string devicePath;

	public:
		explicit DrmDevice(std::string path = "/dev/dri/card0");

		~DrmDevice();

		DrmDevice(const DrmDevice& other) = delete;

		DrmDevice(DrmDevice&& other) noexcept
		{
			fd = other.fd;
			other.fd = -1;
			devicePath = std::move(other.devicePath);
		}

		DrmDevice& operator=(const DrmDevice& rhs) = delete;

		DrmDevice& operator=(DrmDevice&& other) noexcept;

		bool set_capability(uint64_t capability, uint64_t value);

		[[nodiscard]] DrmDeviceCapabilities get_capabilities() const;

		[[nodiscard]] int get_fd() const
		{
			return fd;
		}
	};
}