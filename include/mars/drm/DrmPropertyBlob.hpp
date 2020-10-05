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

#pragma once

#include <mars/drm/DrmDevice.hpp>

namespace mars
{
	class DrmPropertyBlob
	{
		DrmDevice& device;
		uint32_t id{0};

	public:
		template<typename T>
		DrmPropertyBlob(DrmDevice& device, T* blob) : device(device)
		{
			if (drmModeCreatePropertyBlob(device.get_fd(), blob, sizeof(T), &id)) {
				throw std::runtime_error("could not create DRM property blob");
			}
		}

		~DrmPropertyBlob()
		{
			drmModeDestroyPropertyBlob(device.get_fd(), id);
		}

		DrmPropertyBlob(const DrmPropertyBlob& other) = delete;

		DrmPropertyBlob(DrmPropertyBlob&& other) noexcept
				: device(other.device), id(other.id)
		{
			other.id = 0;
		}

		DrmPropertyBlob& operator=(const DrmPropertyBlob& other) = delete;

		DrmPropertyBlob& operator=(DrmPropertyBlob&& other) noexcept
		{
			if (id == 0) {
				drmModeDestroyPropertyBlob(device.get_fd(), id);
			}
			device = std::move(other.device);
			id = other.id;
			other.id = 0;
			return *this;
		}

		[[nodiscard]] uint32_t get_id() const
		{
			return id;
		}
	};
}