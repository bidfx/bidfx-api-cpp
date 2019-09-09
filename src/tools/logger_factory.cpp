/**  Copyright 2019 BidFX

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

            http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
 */

#include <iostream>
#include "include/tools/logger_factory.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/ostream_sink.h"
#include "spdlog/sinks/null_sink.h"

namespace bidfx_public_api::tools
{

spdlog::level::level_enum LoggerFactory::level_(spdlog::level::info);

std::optional<spdlog::level::level_enum> LoggerFactory::flush_severity_({});

std::vector<spdlog::sink_ptr> LoggerFactory::sinks_ = std::vector<spdlog::sink_ptr>();

std::set<std::shared_ptr<spdlog::logger>> LoggerFactory::loggers_ = std::set<std::shared_ptr<spdlog::logger>>();

spdlog::sink_ptr LoggerFactory::AddRotatingFileLogger(std::string base_filename, size_t max_size, size_t max_files)
{
    spdlog::sink_ptr sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(base_filename, max_size, max_files);
    AddSink(sink);
    return sink;
}

spdlog::sink_ptr LoggerFactory::AddDailyFileLogger(std::string base_filename)
{
    spdlog::sink_ptr sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(base_filename, 0, 0);
    AddSink(sink);
    return sink;
}

spdlog::sink_ptr LoggerFactory::AddStdoutLogger(bool colors)
{
    if (colors)
    {
        return AddColoredStdoutLogger();
    }
    else
    {
        return AddBasicStdoutLogger();
    }
}

spdlog::sink_ptr LoggerFactory::AddColoredStdoutLogger()
{
    spdlog::sink_ptr sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    AddSink(sink);
    return sink;
}

spdlog::sink_ptr LoggerFactory::AddBasicStdoutLogger()
{
    spdlog::sink_ptr sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
    AddSink(sink);
    return sink;
}

spdlog::sink_ptr LoggerFactory::AddOstreamLogger(std::ostream& ostream)
{
    spdlog::sink_ptr sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(ostream);
    AddSink(sink);
    return sink;
}

spdlog::sink_ptr LoggerFactory::AddNullLogger()
{
    spdlog::sink_ptr sink = std::make_shared<spdlog::sinks::null_sink_mt>();
    AddSink(sink);
    return sink;
}

void LoggerFactory::AddSink(spdlog::sink_ptr& sink)
{
    sinks_.push_back(sink);
    for(auto logger : loggers_)
    {
        logger->sinks().push_back(sink);
    }
}

void LoggerFactory::SetLogLevel(spdlog::level::level_enum level)
{
    level_ = level;
    for(auto logger : loggers_)
    {
        logger->set_level(level);
    }
}

void LoggerFactory::SetFlushSeverity(spdlog::level::level_enum level)
{
    flush_severity_ = level;
    for (auto logger : loggers_)
    {
        logger->flush_on(level);
    }
}

template <typename Rep, typename Period>
void LoggerFactory::SetFlushInterval(std::chrono::duration<Rep, Period> duration)
{
    spdlog::flush_every(std::chrono::duration_cast<std::chrono::seconds>(duration));
}

std::shared_ptr<spdlog::logger> LoggerFactory::GetLogger(std::string log_id)
{
    auto logger = std::make_shared<spdlog::logger>(log_id, begin(sinks_), end(sinks_));
    if (flush_severity_)
    {
        logger->flush_on(*flush_severity_);
    }
    logger->set_level(level_);
    loggers_.insert(logger);
    return logger;
}

}