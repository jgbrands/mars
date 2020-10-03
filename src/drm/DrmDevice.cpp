//
// Created by Jesse on 9/27/2020.
//

#include <stdexcept>

#include <fcntl.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

#include <mars/drm/DrmDevice.hpp>
#include <iostream>

mars::DrmDevice::DrmDevice(std::string path)
		: fd(open(path.c_str(), O_RDWR | O_CLOEXEC)),
		  devicePath(std::move(path))
{
	if (fd == -1) {
		throw std::runtime_error("could not open DRM device");
	}
}

mars::DrmDevice::~DrmDevice()
{
	close(fd);
}

mars::DrmDevice& mars::DrmDevice::operator=(mars::DrmDevice&& other) noexcept
{
	if (fd != -1) {
		close(fd);
	}
	fd = other.fd;
	devicePath = other.devicePath;
	other.fd = -1;
	return *this;
}

mars::DrmDeviceCapabilities mars::DrmDevice::get_capabilities() const
{
	uint64_t value = 0;
	DrmDeviceCapabilities capabilities = {};
	// Integer values
	drmGetCap(fd, DRM_CAP_CURSOR_WIDTH, &capabilities.cursorWidth);
	drmGetCap(fd, DRM_CAP_CURSOR_HEIGHT, &capabilities.cursorHeight);
	drmGetCap(fd, DRM_CAP_DUMB_PREFERRED_DEPTH, &capabilities.dumbBufferPreferredDepth);
	// Boolean values
	capabilities.dumbBuffers = drmGetCap(fd, DRM_CAP_DUMB_BUFFER, &value) >= 0 && value;
	capabilities.dumbBufferPreferShadow = drmGetCap(fd, DRM_CAP_DUMB_PREFER_SHADOW, &value) >= 0 && value;
	capabilities.vBlankHighCRTC = drmGetCap(fd, DRM_CAP_VBLANK_HIGH_CRTC, &value) >= 0 && value;
	capabilities.prime = drmGetCap(fd, DRM_CAP_PRIME, &value) >= 0 && value;
	capabilities.vdcInVBlankEvent = drmGetCap(fd, DRM_CAP_CRTC_IN_VBLANK_EVENT, &value) >= 0 && value;
	return capabilities;
}

bool mars::DrmDevice::set_capability(uint64_t capability, uint64_t value)
{
	return !drmSetClientCap(fd, capability, value);
}

void mars::DrmDevice::poll_events()
{
	fd_set fds;
	FD_ZERO(&fds);

	timeval tv = {};
	drmEventContext ev = {};
	ev.version = 4;
	ev.page_flip_handler2 = page_flip_handler;

	FD_SET(0, &fds);
	FD_SET(fd, &fds);

	tv.tv_usec = 1000000/60;
	auto ret = select(fd + 1, &fds, nullptr, nullptr, &tv);
	if (ret < 0) {
		throw std::runtime_error("select failed");
	} else if (FD_ISSET(0, &fds)) {
		std::cout << "user input" << std::endl;
	} else if (FD_ISSET(fd, &fds)) {
		drmHandleEvent(fd, &ev);
	}
}

void mars::DrmDevice::page_flip_handler(int fd, uint32_t seq, uint32_t sec, uint32_t usec, uint32_t vdc, void* ptr)
{
	std::cout << "page flip time!!" << std::endl;
}
