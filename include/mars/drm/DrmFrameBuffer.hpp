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
		uint64_t frame;

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
