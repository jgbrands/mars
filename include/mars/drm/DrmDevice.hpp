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

#include <string>
#include <set>
#include <memory>

#include <mars/Log.hpp>

namespace mars
{
	struct DrmDeviceCapabilities
	{
		bool dumbBuffers;
		uint64_t dumbBufferPreferredDepth;
		bool dumbBufferPreferShadow;
		bool vBlankHighCRTC;
		bool vdcInVBlankEvent;
		uint64_t cursorWidth;
		uint64_t cursorHeight;
		bool prime;
	};

	class DrmDevice
	{
		int m_fd;
		std::string m_devicePath;
		std::shared_ptr<Log> m_log;

		static void page_flip_handler(int fd, uint32_t seq, uint32_t sec, uint32_t usec, uint32_t vdc, void* ptr);

	public:
		explicit DrmDevice(std::shared_ptr<Log> log, std::string path);

		~DrmDevice();

		DrmDevice(const DrmDevice& other) = delete;

		DrmDevice(DrmDevice&& other) noexcept
		{
			m_fd = other.m_fd;
			other.m_fd = -1;
			m_devicePath = std::move(other.m_devicePath);
		}

		DrmDevice& operator=(const DrmDevice& rhs) = delete;

		DrmDevice& operator=(DrmDevice&& other) noexcept;

		bool set_capability(uint64_t capability, uint64_t value);

		[[nodiscard]] DrmDeviceCapabilities get_capabilities() const;

		[[nodiscard]] int get_fd() const
		{
			return m_fd;
		}

		void poll_events();

		[[nodiscard]] std::shared_ptr<Log> log() const
		{
			return m_log;
		}
	};
}