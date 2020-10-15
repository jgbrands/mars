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

#include <chrono>
#include <stdexcept>

#include <memory>

#include <systemd/sd-daemon.h>
#include <systemd/sd-journal.h>
#include <unistd.h>
#include <date/date.h>
#include <fmt/format.h>

#include <mars/Log.hpp>

mars::Log::Log(const std::string& identifier, bool verbose)
		: m_fd(sd_journal_stream_fd(identifier.c_str(), LOG_INFO, true)),
		  m_log(fdopen(m_fd, "w"), &fclose),
		  m_verbose(verbose)
{
	if (m_fd < 0) {
		throw std::runtime_error("could not open systemd journal");
	}

	if (m_log == nullptr) {
		close(m_fd);
		throw std::runtime_error("could not open log file stream");
	}
}

mars::Log::~Log()
{
	close(m_fd);
}

void mars::Log::send(const std::string& message)
{
	out(message);
}

void mars::Log::error(const std::exception& e)
{
	out(e.what());
}

void mars::Log::out(const std::string& msg, FILE* file, const fmt::text_style& style, const std::string& prefix)
{
	// Log to the systemd journal
	if (m_log) {
		// TODO: Enable logging priority
		fprintf(m_log.get(), "%s\n", msg.c_str());
	}

	// Log to stdout (file) if this was enabled
	if (m_verbose) {
		if (file == nullptr) {
			return;
		}

		if (m_timestamp) {
			auto t = std::chrono::system_clock::now();
			auto formattedTime = date::format(m_timestampFormat, t);

			if (m_color) {
				fmt::print(file, fmt::emphasis::bold, "[{}] ", formattedTime);
			} else {
				fmt::print(file, "[{}] ", formattedTime);
			}
		}

		if (m_color) {
			fmt::print(file, style, "{}\n", msg);
		} else {
			fmt::print(file, "{}\n", msg);
		}
	}
}


