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

#include <stdexcept>

#include <fcntl.h>
#include <unistd.h>
#include <xf86drm.h>

#include <mars/drm/DrmDevice.hpp>
#include <iostream>
#include <memory>

mars::DrmDevice::DrmDevice(std::shared_ptr<Log> log, std::string path)
		: m_log(std::move(log)),
		  m_fd(open(path.c_str(), O_RDWR | O_CLOEXEC)),
		  m_devicePath(std::move(path))
{
	if (m_fd == -1) {
		throw std::runtime_error("could not open DRM device");
	}

	m_log->send("Opened DRM device '{}'", m_devicePath);
}

mars::DrmDevice::~DrmDevice()
{
	close(m_fd);
}

mars::DrmDevice& mars::DrmDevice::operator=(mars::DrmDevice&& other) noexcept
{
	if (m_fd != -1) {
		close(m_fd);
	}
	m_log = std::move(other.m_log);
	m_fd = other.m_fd;
	m_devicePath = other.m_devicePath;
	other.m_fd = -1;
	return *this;
}

mars::DrmDeviceCapabilities mars::DrmDevice::get_capabilities() const
{
	uint64_t value = 0;
	DrmDeviceCapabilities capabilities = {};
	// Integer values
	drmGetCap(m_fd, DRM_CAP_CURSOR_WIDTH, &capabilities.cursorWidth);
	drmGetCap(m_fd, DRM_CAP_CURSOR_HEIGHT, &capabilities.cursorHeight);
	drmGetCap(m_fd, DRM_CAP_DUMB_PREFERRED_DEPTH, &capabilities.dumbBufferPreferredDepth);
	// Boolean values
	capabilities.dumbBuffers = drmGetCap(m_fd, DRM_CAP_DUMB_BUFFER, &value) >= 0 && value;
	capabilities.dumbBufferPreferShadow = drmGetCap(m_fd, DRM_CAP_DUMB_PREFER_SHADOW, &value) >= 0 && value;
	capabilities.vBlankHighCRTC = drmGetCap(m_fd, DRM_CAP_VBLANK_HIGH_CRTC, &value) >= 0 && value;
	capabilities.prime = drmGetCap(m_fd, DRM_CAP_PRIME, &value) >= 0 && value;
	capabilities.vdcInVBlankEvent = drmGetCap(m_fd, DRM_CAP_CRTC_IN_VBLANK_EVENT, &value) >= 0 && value;
	return capabilities;
}

bool mars::DrmDevice::set_capability(uint64_t capability, uint64_t value)
{
	return !drmSetClientCap(m_fd, capability, value);
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
	FD_SET(m_fd, &fds);

	tv.tv_usec = 1000000 / 60;
	auto ret = select(m_fd + 1, &fds, nullptr, nullptr, &tv);
	if (ret < 0) {
		throw std::runtime_error("select failed");
	} else if (FD_ISSET(0, &fds)) {
		std::cout << "user input" << std::endl;
	} else if (FD_ISSET(m_fd, &fds)) {
		drmHandleEvent(m_fd, &ev);
	}
}

void mars::DrmDevice::page_flip_handler(int fd, uint32_t seq, uint32_t sec, uint32_t usec, uint32_t vdc, void* ptr)
{
	std::cout << "page flip time!!" << std::endl;
}
