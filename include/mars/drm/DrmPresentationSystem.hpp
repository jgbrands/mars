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

#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmSwapchain.hpp>

namespace mars
{
	/**
	 * The DrmPresentationSystem class wraps one or more frame buffers and handles the creation of them, setting them up with
	 * correct properties and handling swapping between them. Essentially, this is the structure we will deal with the
	 * most out of the DRM layer and is a direct implementation of mars::IPresenter.
	 */
	class DrmPresentationSystem
	{
		DrmDevice m_device;
		std::shared_ptr<mars::Log> m_log;
		std::vector<mars::DrmSwapchain> m_swapchains;

		static void page_flip_handler(int fd, uint32_t seq, uint32_t s, uint32_t us, uint32_t vdc, void* ptr);

	public:
		explicit DrmPresentationSystem(std::shared_ptr<mars::Log> log, const std::string& devicePath);

		void poll_events();
	};
}