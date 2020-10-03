//
// Created by Jesse on 10/3/2020.
//

#pragma once

#include <string>

#include <sdbus-c++/sdbus-c++.h>

namespace mars
{
	class LogindSession
	{
		std::unique_ptr<sdbus::IProxy> managerProxy;
		std::unique_ptr<sdbus::IProxy> sessionProxy;
		std::unique_ptr<sdbus::IProxy> seatProxy;
		std::unique_ptr<sdbus::IProxy> userProxy;

	public:
		explicit LogindSession(const std::string& sessionId = get_display_session());

		static std::string get_display_session();

	};
}