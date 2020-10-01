//
// Created by Jesse on 9/29/2020.
//

#pragma once

#include <vector>

#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmConnector.hpp>
#include <mars/drm/DrmSurface.hpp>

namespace mars
{
	/**
	 * The DrmPresenter class wraps one or more frame buffers and handles the creation of them, setting them up with
	 * correct properties and handling swapping between them. Essentially, this is the structure we will deal with the
	 * most out of the DRM layer and is a direct implementation of mars::IPresenter.
	 */
	class DrmPresenter
	{
		std::vector<mars::DrmSurface> surfaces;

	public:
		DrmPresenter(DrmDevice& device, DrmConnector& connector);
	};
}