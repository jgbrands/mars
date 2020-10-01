//
// Created by Jesse on 9/26/2020.
//

#pragma once

#include <cstdint>

#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmDumbBuffer.hpp>

namespace mars
{
	class DrmFrameBuffer
	{
		DrmDevice& device;
		DrmDumbBuffer buffer;
		uint32_t handle;

	public:
		DrmFrameBuffer(DrmDevice& device, drmModeModeInfo mode);
		~DrmFrameBuffer();
		DrmFrameBuffer(const DrmFrameBuffer& other) = delete;
		DrmFrameBuffer(DrmFrameBuffer&& other) noexcept;
		DrmFrameBuffer& operator=(const DrmFrameBuffer& other) = delete;
		DrmFrameBuffer& operator=(DrmFrameBuffer&& other) noexcept;

		[[nodiscard]] uint32_t get_id() const
		{
			return handle;
		}

		[[nodiscard]] DrmDumbBuffer& get_buffer()
		{
			return buffer;
		}
	};
}
