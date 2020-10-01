//
// Created by Jesse on 9/29/2020.
//

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