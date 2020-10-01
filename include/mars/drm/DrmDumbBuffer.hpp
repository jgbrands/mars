//
// Created by Jesse on 9/27/2020.
//

#pragma once

#include <mars/drm/DrmDevice.hpp>

namespace mars
{
	/**
	 * Encapsulates a DRM Dumb Buffer
	 */
	class DrmDumbBuffer
	{
		DrmDevice& device;
		uint32_t handle;
		uint32_t width;
		uint32_t height;
		uint32_t bpp;
		uint32_t pitch;
		uint64_t size;

	public:
		DrmDumbBuffer(DrmDevice& device, uint32_t width, uint32_t height, uint32_t bpp);

		~DrmDumbBuffer();

		DrmDumbBuffer(const DrmDumbBuffer& other) = delete;

		DrmDumbBuffer(DrmDumbBuffer&& other) noexcept;

		DrmDumbBuffer& operator=(const DrmDumbBuffer& other) = delete;

		DrmDumbBuffer& operator=(DrmDumbBuffer&& other) noexcept;

		void* map();

		[[nodiscard]] uint32_t get_handle() const
		{
			return handle;
		}

		[[nodiscard]] uint32_t get_width() const
		{
			return width;
		}

		[[nodiscard]] uint32_t get_height() const
		{
			return height;
		}

		[[nodiscard]] uint32_t get_bpp() const
		{
			return bpp;
		}

		[[nodiscard]] uint32_t get_pitch() const
		{
			return pitch;
		}

		[[nodiscard]] uint64_t get_size() const
		{
			return size;
		}
	};
}