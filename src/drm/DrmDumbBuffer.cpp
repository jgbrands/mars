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

#include <xf86drm.h>
#include <xf86drmMode.h>

#include <mars/drm/DrmDumbBuffer.hpp>
#include <sys/mman.h>
#include <cstring>

mars::DrmDumbBuffer::DrmDumbBuffer(mars::DrmDevice& device, uint32_t w, uint32_t h, uint32_t bits)
		: device(device)
{
	drm_mode_create_dumb create = {};
	create.width = w;
	create.height = h;
	create.bpp = bits;

	if (drmIoctl(device.get_fd(), DRM_IOCTL_MODE_CREATE_DUMB, &create) < 0) {
		throw std::runtime_error("cannot create dumb buffer");
	}

	// Copy the information from the buffer creation object
	handle = create.handle;
	width = create.width;
	height = create.height;
	bpp = create.bpp;
	pitch = create.pitch;
	size = create.size;
}

mars::DrmDumbBuffer::~DrmDumbBuffer()
{
	if (handle > 0) {
		drm_mode_destroy_dumb destroy = {};
		destroy.handle = handle;
		drmIoctl(device.get_fd(), DRM_IOCTL_MODE_DESTROY_DUMB, &destroy);
	}
}

mars::DrmDumbBuffer::DrmDumbBuffer(mars::DrmDumbBuffer&& other) noexcept
		: device(other.device)
{
	handle = other.handle;
	other.handle = 0;
	width = other.width;
	height = other.height;
	bpp = other.bpp;
	pitch = other.pitch;
	size = other.size;
}

mars::DrmDumbBuffer& mars::DrmDumbBuffer::operator=(mars::DrmDumbBuffer&& other) noexcept
{
	if (handle > 0) {
		drm_mode_destroy_dumb destroy = {};
		destroy.handle = handle;
		drmIoctl(device.get_fd(), DRM_IOCTL_MODE_DESTROY_DUMB, &destroy);
	}
	device = std::move(other.device);
	handle = other.handle;
	other.handle = 0;
	width = other.width;
	height = other.height;
	bpp = other.bpp;
	pitch = other.pitch;
	size = other.size;
	return *this;
}

void* mars::DrmDumbBuffer::map()
{
	drm_mode_map_dumb mreq = {};
	mreq.handle = handle;
	if (drmIoctl(device.get_fd(), DRM_IOCTL_MODE_MAP_DUMB, &mreq)) {
		return nullptr;
	}

	auto* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, device.get_fd(), mreq.offset);
	if (ptr == MAP_FAILED) {
		throw std::runtime_error("could not map buffer");
		return nullptr;
	}

	memset(ptr, 255, size);
	return ptr;
}

