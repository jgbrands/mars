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

#include <sdbus-c++/sdbus-c++.h>
#include <mars/Log.hpp>

namespace mars
{
	class LogindSession
	{
		std::shared_ptr<Log> m_log;
		std::string m_sessionId;
		std::string m_seatId;
		std::string m_username;

		std::unique_ptr<sdbus::IProxy> m_managerProxy;
		std::unique_ptr<sdbus::IProxy> m_sessionProxy;
		std::unique_ptr<sdbus::IProxy> m_seatProxy;
		std::unique_ptr<sdbus::IProxy> m_userProxy;

	public:
		explicit LogindSession(std::shared_ptr<Log> log, const std::string& sessionId = get_display_session());

		void activate();

		void take_control(bool force = false);

		int take_device(std::string_view devicePath);

		void release_device(int fd);

		static std::string get_display_session();
	};
}