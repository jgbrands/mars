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

#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <fcntl.h>
#include <zconf.h>

#include <mars/session/LogindSession.hpp>
#include <mars/Log.hpp>
#include <utility>


constexpr const char* managerInterfaceName = "org.freedesktop.login1.Manager";
constexpr const char* sessionInterfaceName = "org.freedesktop.login1.Session";
constexpr const char* seatInterfaceName = "org.freedesktop.login1.Seat";
constexpr const char* userInterfaceName = "org.freedesktop.login1.User";

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
			.onInterface(managerInterfaceName)
			.withArguments(static_cast<unsigned int>(getpid()))
			.storeResultsTo(sessionPath);

	if (!sessionPath.empty()) {
		auto sessionProxy = sdbus::createProxy("org.freedesktop.login1", sessionPath);
		return sessionProxy->getProperty("Id")
				.onInterface(sessionInterfaceName)
				.get<std::string>();
	}

	return std::string();
}

mars::LogindSession::LogindSession(std::shared_ptr<Log> log, const std::string& sessionId)
		: m_log(std::move(log)),
		  m_managerProxy(sdbus::createProxy("org.freedesktop.login1", "/org/freedesktop/login1")),
		  m_sessionId(sessionId)
{
	sdbus::ObjectPath sessionPath;
	m_managerProxy->callMethod("GetSession")
			.onInterface(managerInterfaceName)
			.withArguments(sessionId)
			.storeResultsTo(sessionPath);

	if (sessionPath.empty()) {
		throw std::runtime_error("could not get session");
	}

	m_sessionProxy = sdbus::createProxy("org.freedesktop.login1", sessionPath);
	auto seat = m_sessionProxy->getProperty("Seat")
			.onInterface(sessionInterfaceName)
			.get<sdbus::Struct<std::string, sdbus::ObjectPath>>();

	if (seat.get<0>().empty()) {
		std::vector<sdbus::Struct<std::string, sdbus::ObjectPath>> seats;
		m_managerProxy->callMethod("ListSeats")
				.onInterface(managerInterfaceName)
				.storeResultsTo(seats);

		for (auto& s: seats) {
			auto p = sdbus::createProxy("org.freedesktop.login1", s.get<1>());
			bool canGraphical = p->getProperty("CanGraphical")
					.onInterface(seatInterfaceName)
					.get<bool>();

			if (canGraphical) {
				m_seatProxy = std::move(p);
				break;
			}
		}
	} else {
		m_seatProxy = sdbus::createProxy("org.freedesktop.login1", seat.get<1>());
	}

	if (m_seatProxy) {
		m_seatId = m_seatProxy->getProperty("Id")
				.onInterface(seatInterfaceName)
				.get<std::string>();
	}

	// Last, get the user
	auto u = m_sessionProxy->getProperty("User")
			.onInterface(sessionInterfaceName)
			.get<sdbus::Struct<unsigned int, sdbus::ObjectPath>>();

	if (u.get<0>() != 0 && u.get<1>() != "/") {
		m_userProxy = sdbus::createProxy("org.freedesktop.login1", u.get<1>());
		m_username = m_userProxy->getProperty("Name")
				.onInterface(userInterfaceName)
				.get<std::string>();
	}

	m_log->send(fmt::format("Active on session {} as user {} ({})", m_sessionId, m_username, m_seatId));
}

void mars::LogindSession::activate()
{
	if (m_seatProxy && m_sessionProxy) {
		m_sessionProxy->callMethod("ActivateSessionOnSeat")
				.onInterface(managerInterfaceName)
				.withArguments(m_sessionId, m_seatId)
				.dontExpectReply();
	} else if (m_sessionProxy) {
		m_sessionProxy->callMethod("Activate")
				.onInterface(sessionInterfaceName)
				.dontExpectReply();
	} else {
		throw std::runtime_error("no session or seat configured");
	}
}

void mars::LogindSession::take_control(bool force)
{
	m_sessionProxy->callMethod("TakeControl")
			.onInterface(sessionInterfaceName)
			.withArguments(force)
			.dontExpectReply();
}

int mars::LogindSession::take_device(std::string_view devicePath)
{
	struct stat st = {};
	if (stat(devicePath.data(), &st) < 0) {
		throw std::runtime_error("could not open device");
	}

	sdbus::UnixFd fd;
	bool paused = false;

	m_sessionProxy->callMethod("TakeDevice")
			.onInterface(sessionInterfaceName)
			.withArguments(major(st.st_rdev), minor(st.st_rdev))
			.storeResultsTo(fd, paused);

	return fcntl(fd.get(), F_DUPFD_CLOEXEC, 0);
}

void mars::LogindSession::release_device(int fd)
{
	struct stat st = {};
	if (fstat(fd, &st) < 0) {
		return; // already closed
	}

	m_sessionProxy->callMethod("ReleaseDevice")
			.onInterface(sessionInterfaceName)
			.withArguments(major(st.st_rdev), minor(st.st_rdev))
			.dontExpectReply();

	close(fd);
}



