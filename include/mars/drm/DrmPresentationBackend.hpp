//
// Created by Jesse on 9/29/2020.
//

#pragma once

#include <vector>

#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmSwapchain.hpp>

namespace mars
{
	/**
	 * The DrmPresentationBackend class wraps one or more frame buffers and handles the creation of them, setting them up with
	 * correct properties and handling swapping between them. Essentially, this is the structure we will deal with the
	 * most out of the DRM layer and is a direct implementation of mars::IPresenter.
	 */
	class DrmPresentationBackend
	{
		DrmDevice device;
		std::vector<mars::DrmSwapchain> swapchains;

		static void page_flip_handler(int fd, uint32_t seq, uint32_t s, uint32_t us, uint32_t vdc, void* ptr);

	public:
		explicit DrmPresentationBackend(const std::string& devicePath);

		void poll_events();
	};
}