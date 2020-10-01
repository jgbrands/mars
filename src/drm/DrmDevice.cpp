//
// Created by Jesse on 9/27/2020.
//

#include <stdexcept>

#include <fcntl.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

#include <mars/drm/DrmDevice.hpp>

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
