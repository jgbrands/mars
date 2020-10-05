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

#include <stdexcept>

#include <xf86drmMode.h>

#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmResourcePtr.hpp>
#include "DrmObjectProperties.hpp"

namespace mars
{
	/**
	 * DrmVideoDisplayController wraps what is called a CRTC in xf86drmMode.h, an antiquated term that means Cathode-Ray
	 * Tube Controller, this library prefers the more generic name for such chips: Video Display Controller.
	 */
	class DrmVideoDisplayController
	{
		DrmDevice& device;
		DrmUniqueVideoDisplayControllerPtr crtc;
		uint32_t id;
		int index;

	public:
		/**
		 * Constructs a DrmVideoDisplayController instance. This should never be called manually, but queried from
		 * DrmDeviceResources instead.
		 * @param device The device this VDC belongs to
		 * @param id The identifier of the VDC
		 * @param index The index of the VDC in the resource array
		 */
		DrmVideoDisplayController(DrmDevice& device, uint32_t id, int index)
				: device(device), id(id), index(index),
				  crtc(drmModeGetCrtc(device.get_fd(), id))
		{
			if (crtc == nullptr) {
				throw std::runtime_error("could not get DRM video display controller");
			}
		}

		DrmVideoDisplayController(DrmDevice& device, uint32_t id);

		drmModeCrtc* operator->() const noexcept
		{
			return crtc.get();
		}

		[[nodiscard]] uint32_t get_id() const
		{
			return id;
		}

		[[nodiscard]] int get_index() const
		{
			return index;
		}

		[[nodiscard]] DrmObjectProperties properties() const {
			return DrmObjectProperties(device, crtc->crtc_id, DRM_MODE_OBJECT_CRTC);
		}

		void set_property(DrmAtomicRequest& req, const std::string& name, uint64_t value)
		{
			DrmObjectProperties(device, crtc->crtc_id, DRM_MODE_OBJECT_CRTC)
					.set_value(name, value, req);
		}
	};
}