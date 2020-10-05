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
#include <vector>

#include <xf86drmMode.h>

#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmResourcePtr.hpp>
#include <iostream>
#include "DrmAtomicRequest.hpp"

namespace mars
{
	class DrmObjectProperties
	{
		DrmDevice& device;
		uint32_t objectId;
		DrmUniqueObjectPropertiesPtr properties;

	public:
		DrmObjectProperties(DrmDevice& device, uint32_t objectId, uint32_t type)
				: device(device), objectId(objectId),
				  properties(drmModeObjectGetProperties(device.get_fd(), objectId, type))
		{
			if (properties == nullptr) {
				throw std::runtime_error("could not retrieve object properties");
			}
		}

		[[nodiscard]] DrmUniquePropertyPtr get(int32_t propertyId) const
		{
			return DrmUniquePropertyPtr(drmModeGetProperty(device.get_fd(), propertyId));
		}

		[[nodiscard]] DrmUniquePropertyPtr get(const std::string& name) const
		{
			for (int i = 0; i < properties->count_props; ++i) {
				auto prop = get(properties->props[i]);
				if (name == prop->name) {
					return prop;
				}
			}
			return nullptr;
		}

		[[nodiscard]] int64_t get_value(const std::string& name) const {
			for (int i = 0; i < properties->count_props; ++i) {
				auto prop = get(properties->props[i]);
				if (name == prop->name) {
					return properties->prop_values[i];
				}
			}
			return -1;
		}

		void set_value(const std::string& name, uint64_t value, DrmAtomicRequest& req)
		{
			for (int i = 0; i < properties->count_props; ++i) {
				auto prop = get(properties->props[i]);
				if (name == prop->name) {
					if (drmModeAtomicAddProperty(req.get(), objectId, prop->prop_id, value) < 0) {
						throw std::runtime_error("failed to set property on object");
					}
					return;
				}
			}
			throw std::runtime_error("could not find property on object");
		}

		[[nodiscard]] std::vector<DrmUniquePropertyPtr> get_all() const
		{
			std::vector<DrmUniquePropertyPtr> propInfo;
			propInfo.reserve(properties->count_props);
			for (int i = 0; i < properties->count_props; ++i) {
				propInfo.emplace_back(drmModeGetProperty(device.get_fd(), properties->props[i]));
			}
			return propInfo;
		}

		void print() const
		{
			int i = 0;
			for (auto& prop: get_all()) {
				std::cout << (i + 1) << ". "
				          << prop->name << ": "
				          << properties->prop_values[i]
				          << std::endl;
				i++;
			}
		}
	};
}