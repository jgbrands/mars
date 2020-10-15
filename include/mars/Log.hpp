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

#include <fmt/core.h>
#include <fmt/color.h>

namespace mars
{
	/**
	 * mars::Log is the logging utility of mars, which is mostly a light wrapper around sd-journal but can optionally
	 * also write strings to stdout when the program is executed with the -Verbose argument.
	 */
	class Log
	{
		int m_fd;
		std::unique_ptr<FILE, decltype(&fclose)> m_log;
		bool m_verbose;
		bool m_timestamp{true};
		bool m_color{true};
		std::string m_timestampFormat{"%H:%M:%S"};

	public:
		explicit Log(const std::string& identifier = "Mars", bool verbose = false);

		~Log();

		Log(const Log& other) = delete;

		Log& operator=(const Log& other) = delete;

		void out(const std::string& msg, FILE* file = stdout, const fmt::text_style& style = fmt::text_style(),
		         const std::string& prefix = "");

		void send(const std::string& message);

		template<typename S, typename ...Args>
		void send(const S& format, Args&&... args)
		{
			out(fmt::format(format, args...));
		}

		void error(const std::exception& e);

		void set_verbose(bool verbose)
		{
			m_verbose = verbose;
		}

		void set_timestamp(bool timestamp)
		{
			m_timestamp = timestamp;
		}

		void colorize_output(bool color)
		{
			m_color = color;
		}

		void set_timestamp_format(const std::string& timestampFormat)
		{
			m_timestampFormat = timestampFormat;
		}
	};
}
