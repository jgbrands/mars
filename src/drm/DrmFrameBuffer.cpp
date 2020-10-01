//
// Created by Jesse on 9/27/2020.
//

#include <array>
#include <stdexcept>

#include <drm_fourcc.h>
#include <xf86drmMode.h>

#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmFrameBuffer.hpp>

mars::DrmFrameBuffer::DrmFrameBuffer(mars::DrmDevice& device, drmModeModeInfo mode)
		: device(device),
		  buffer(device, mode.hdisplay, mode.vdisplay, 32),
		  handle(0)
{
	std::array<uint32_t, 4> handles = {0};
	std::array<uint32_t, 4> pitches = {0};
	std::array<uint32_t, 4> offsets = {0};

	handles[0] = buffer.get_handle();
	pitches[0] = buffer.get_pitch();

	if (drmModeAddFB2(device.get_fd(), buffer.get_width(), buffer.get_height(), DRM_FORMAT_XRGB8888,
	                  handles.data(), pitches.data(), offsets.data(), &handle, 0)) {
		throw std::runtime_error("cannot create frame buffer");
	}
}

mars::DrmFrameBuffer::~DrmFrameBuffer()
{
	drmModeRmFB(device.get_fd(), handle);
}

mars::DrmFrameBuffer::DrmFrameBuffer(mars::DrmFrameBuffer&& other) noexcept
		: device(other.device),
		  buffer(std::move(other.buffer)),
		  handle(other.handle)
{
	other.handle = 0;
}

mars::DrmFrameBuffer& mars::DrmFrameBuffer::operator=(mars::DrmFrameBuffer&& other) noexcept
{
	if (handle > 0) {
		drmModeRmFB(device.get_fd(), handle);
	}
	device = std::move(other.device);
	buffer = std::move(other.buffer);
	handle = other.handle;
	other.handle = 0;
	return *this;
}
