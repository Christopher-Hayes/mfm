/* log.hpp
 * Modified: (07/11/18) Chris Hayes
 */
#pragma once
// BOOST - LOG
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/support/date_time.hpp>
// BOOST - LOG - UTILITY
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
// BOOST - DATE-TIME
#include <boost/date_time/posix_time/posix_time_types.hpp>
// BOOST - LOG - SINKS
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
// BOOST - SMART POINTERS
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>

#include "settings.h"

// Terminal text color highlighting.
static const std::string log_reset   = "\033[0m";
static const std::string log_black   = "\033[30m"; /* Black */
static const std::string log_red     = "\033[31m"; /* Red */
static const std::string log_green   = "\033[32m"; /* Green */
static const std::string log_yellow  = "\033[33m"; /* Yellow */
static const std::string log_blue    = "\033[34m"; /* Blue */
static const std::string log_magenta = "\033[35m"; /* Magenta */
static const std::string log_cyan    = "\033[36m"; /* Cyan */
static const std::string log_white   = "\033[37m"; /* White */

// An exception if logging fails to initialize.
class LoggingInitException : public std::exception {};

/*
  Class to set up logging as we want it in all services.

  Instantiate a Log object near the beginning of main().
*/
class Log {
   public:
    Log();
    ~Log();
};

// Logging macro that includes severity, filename, and line number.
// Copied and modified from
// https://stackoverflow.com/questions/24750218/boost-log-to-print-source-code-file-name-and-line-number
// Set attribute and return the new value
template <typename ValueType>
ValueType SetGetAttrib(const char* name, ValueType value) {
    auto attr = boost::log::attribute_cast<
        boost::log::attributes::mutable_constant<ValueType>>(
        boost::log::core::get()->get_global_attributes()[name]);
    attr.set(value);
    return attr.get();
}

// Convert file path to only the filename
std::string PathToFilename(const std::string& path);

// Shortcut to declare a log source.  To insert in each function that will call
// the LOG macro.
#define LOGGABLE                                                              \
    static boost::log::sources::severity_logger<boost::log::trivial::severity_level> \
        slg;

LOGGABLE

#define LOG(sev)                                                 \
    BOOST_LOG_STREAM_WITH_PARAMS(                                \
        (slg),                                                   \
        (SetGetAttrib("FileName", PathToFilename(__FILE__)))(    \
            SetGetAttrib("LineNumber", (unsigned int)__LINE__))( \
            ::boost::log::keywords::severity = (boost::log::trivial::sev)))

#define INFO LOG(info) << log_blue << "[INFO] "
#define WARN LOG(warning) << log_yellow << "[WARN] "
#define ERROR LOG(error) << log_red << "[ERROR] "
#define FATAL LOG(fatal) << log_red << "[FATAL] "

