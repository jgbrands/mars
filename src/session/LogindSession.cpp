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

#include <memory>

#include <zconf.h>

#include <mars/session/LogindSession.hpp>

std::string mars::LogindSession::get_display_session()
{
	char* xdgSessionId = getenv("XDG_SESSION_ID");
	if (xdgSessionId) {
		return xdgSessionId;
	}

	// From here on, we'll need a proxy
	auto proxy = sdbus::createProxy("org.freedesktop.login1", "/org/freedesktop/login1");
	sdbus::ObjectPath sessionPath;
	proxy->callMethod("GetSessionByPID")
			.onInterface("org.freedesktop.login1.Manager")
			.withArguments(static_cast<unsigned int>(getpid()))
			.storeResultsTo(sessionPath);

	if (!sessionPath.empty()) {
		auto sessionProxy = sdbus::createProxy("org.freedesktop.login1", sessionPath);
		return sessionProxy->getProperty("Id")
				.onInterface("org.freedesktop.login1.Session")
				.get<std::string>();
	}

	return std::string();
}

mars::LogindSession::LogindSession(const std::string& sessionId)
		: managerProxy(sdbus::createProxy("org.freedesktop.login1", "/org/freedesktop/login1"))
{
	sdbus::ObjectPath sessionPath;
	managerProxy->callMethod("GetSession")
			.onInterface("org.freedesktop.login1.Manager")
			.withArguments(sessionId)
			.storeResultsTo(sessionPath);

	if (sessionPath.empty()) {
		throw std::runtime_error("could not get session");
	}

	sessionProxy = sdbus::createProxy("org.freedesktop.login1", sessionPath);
	auto seat = sessionProxy->getProperty("Seat")
			.onInterface("org.freedesktop.login1.Session")
			.get<sdbus::Struct<std::string, sdbus::ObjectPath>>();

	if (seat.get<0>().empty()) {
		std::vector<sdbus::Struct<std::string, sdbus::ObjectPath>> seats;
		managerProxy->callMethod("ListSeats")
				.onInterface("org.freedesktop.login1.Manager")
				.storeResultsTo(seats);

		for (auto& s: seats) {
			auto p = sdbus::createProxy("org.freedesktop.login1", s.get<1>());
			bool canGraphical = p->getProperty("CanGraphical")
					.onInterface("org.freedesktop.login1.Seat")
					.get<bool>();

			if (canGraphical) {
				seatProxy = std::move(p);
				managerProxy->callMethod("ActivateSessionOnSeat")
						.onInterface("org.freedesktop.login1.Session")
						.withArguments(sessionId, s.get<0>())
						.dontExpectReply();
				break;
			}
		}
	} else {
		seatProxy = sdbus::createProxy("org.freedesktop.login", seat.get<1>());
	}

	// Last, get the user
	auto u = sessionProxy->getProperty("User")
			.onInterface("org.freedesktop.login1.Session")
			.get<sdbus::Struct<unsigned int, sdbus::ObjectPath>>();

	if (u.get<0>() == 0 || u.get<1>() == "/") {
		userProxy = sdbus::createProxy("org.freedesktop.login1", u.get<1>());
	}
}
