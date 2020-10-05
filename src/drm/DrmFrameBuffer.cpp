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
