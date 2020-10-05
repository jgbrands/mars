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

#include <vector>
#include <optional>

#include <mars/drm/DrmConnector.hpp>
#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmDumbBuffer.hpp>
#include <mars/drm/DrmEncoder.hpp>
#include <mars/drm/DrmFrameBuffer.hpp>
#include <mars/drm/DrmPropertyBlob.hpp>
#include <mars/drm/DrmPlane.hpp>

namespace mars
{
	/**
	 * DrmSwapchain represents a video mode that is active on a display with attached framebuffer. DrmSwapchain can be used
	 * to blit or copy pixels to, as well as setting the hardware cursor and its position.
	 */
	class DrmSwapchain
	{
		DrmDevice& device;
		DrmEncoder encoder;
		DrmConnector connector;
		drmModeModeInfo modeInfo;
		DrmPropertyBlob modeBlob;
		DrmVideoDisplayController initialVdc;
		std::vector<DrmFrameBuffer> buffers;
		std::optional<DrmPlane> primaryPlane;
		std::optional<DrmPlane> cursorPlane;

		uint64_t frame{0};

	public:
		DrmSwapchain(DrmDevice& device, DrmConnector&& connector, DrmEncoder&& encoder, drmModeModeInfo mode);

		// Why is this required?
		DrmSwapchain(DrmSwapchain&& other) noexcept = default;

		DrmSwapchain(const DrmSwapchain& other) = delete;

		~DrmSwapchain();

		void modeset(DrmAtomicRequest& req);

		void swap_buffers();

		DrmFrameBuffer& get_current_frame_buffer();

		[[nodiscard]] const DrmEncoder& get_encoder() const
		{
			return encoder;
		}

		[[nodiscard]] const DrmConnector& get_connector() const
		{
			return connector;
		}

		[[nodiscard]] const drmModeModeInfo& get_mode_info() const
		{
			return modeInfo;
		}

		[[nodiscard]] const std::vector<DrmFrameBuffer>& get_framebuffers() const
		{
			return buffers;
		}

		[[nodiscard]] uint64_t get_frame() const
		{
			return frame;
		}
	};
}