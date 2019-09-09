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

#ifndef PUBLIC_API_CPP_TOOLS_LOGGER_FACTORY_H_
#define PUBLIC_API_CPP_TOOLS_LOGGER_FACTORY_H_

#include <set>
#include <lib/logging/spdlog/spdlog.h>
#include <lib/logging/spdlog/logger.h>
#include <lib/logging/spdlog/async.h>
#include <lib/logging/spdlog/sinks/basic_file_sink.h>
#include "src/tools/optional.h"

namespace bidfx_public_api::tools
{

/**
 * This class provides a method for getting a logger based on a given name (id/thread etc)
 * @author Frazer Bennett Wilford
 */
class LoggerFactory
{
private:
    static spdlog::level::level_enum level_;
    static std::optional<spdlog::level::level_enum> flush_severity_;
    static std::vector<spdlog::sink_ptr> sinks_;
    static std::set<std::shared_ptr<spdlog::logger>> loggers_;

    static spdlog::sink_ptr AddColoredStdoutLogger();
    static spdlog::sink_ptr AddBasicStdoutLogger();
    static void AddSink(spdlog::sink_ptr& sink);
public:
    /**
     * Add a target for log entries that writes entries to a file, rolling the files as they reach a certain size
     * @param base_filename the root name of the files to be written to. Ensure the logging directory already exists
     * @param max_size the maximum size a file will grow to before files are rolled
     * @param max_files the maximum number of files to keep
     * @return a shared pointer to the created sink, allowing for custom flush and threshold levels
     */
    static spdlog::sink_ptr AddRotatingFileLogger(std::string base_filename, size_t max_size, size_t max_files);

    /**
     * Add a target for log entries that writes entries to a file, rolling the files at midnight
     * @param base_filename the root name fo the files to be written to. Ensure the logging directory already exists
     * @return a shared pointer to the created sink, allowing for custom flush and threshold levels
     */
    static spdlog::sink_ptr AddDailyFileLogger(std::string base_filename);

    /**
     * Add a target for log entries that writes entries to Stdout
     * @param colors whether colors should be used when writing to stdout
     * @return a shared pointer to the created sink, allowing for custom flush and threshold levels
     */
    static spdlog::sink_ptr AddStdoutLogger(bool colors);

    /**
     * Add a target for log entries that writes entries to an output stream
     * @param ostream the output stream to write entries to
     * @return a shared pointer to the created sink, allowing for custom flush and threshold levels
     */
    static spdlog::sink_ptr AddOstreamLogger(std::ostream& ostream);

    /**
     * Add a target for log entries that discards the log entries
     * @return a shared pointer to the created sink, allowing for custom flush and threshold levels
     */
    static spdlog::sink_ptr AddNullLogger();

    /**
     * Set the minimum level of log entries that get logged.
     * @param level the finest level of logging
     */
    static void SetLogLevel(spdlog::level::level_enum level);
    
    /**
     * Set a log level that a flush should always be called immediately after
     * @param level
     */
    static void SetFlushSeverity(spdlog::level::level_enum level);
    
    /**
     * Set an interval after which log entries should be automatically flushed
     * @param duration 
     */
    template <typename Rep, typename Period>
        static void SetFlushInterval(std::chrono::duration<Rep, Period> duration);

    /**
     * Create a logger that can be used to write log entries to the configured sinks
     * @param log_id the name of the logger, will be included in log entries
     * @return a shared pointer to the created logger
     */
    static std::shared_ptr<spdlog::logger> GetLogger(std::string log_id);
};

}

#endif //PUBLIC_API_CPP_TOOLS_LOGGER_FACTORY_H_
