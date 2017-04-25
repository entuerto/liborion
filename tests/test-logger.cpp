//  test-logger.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/Logging.h>
#include <orion/TestUtils.h>

using namespace orion;
using namespace orion::logging;
using namespace orion::unittest;

LogRecord output_record;

//----------------------------------------------------------------------------
// Test log classes
//----------------------------------------------------------------------------
class LogRecordOutputHandler : public OutputHandler
{
public:
   NO_COPY(LogRecordOutputHandler)
   NO_MOVE(LogRecordOutputHandler)
   
   LogRecordOutputHandler(LogRecord& record) : _out_record(record) {}
   virtual ~LogRecordOutputHandler() {}

   virtual void write(const LogRecord& log_record)
   {
      _out_record = log_record;
   }

   virtual void flush() {}

   virtual void close() {}

   static std::unique_ptr<OutputHandler> create(LogRecord& record)
   {
      return std::make_unique<LogRecordOutputHandler>(record);
   }

private:
   LogRecord& _out_record;
};

TestSuite(OrionCore)
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
void log_default_level(Test& t)
{
   Logger& logger = Logger::get_logger();

   t.assert<std::equal_to<>>(Level::NotSet, logger.level(), _src_loc);
}

void log_level_change(Test& t)
{
   Logger& logger = Logger::get_logger();

   logger.level(Level::Warning);

   t.assert<std::equal_to<>>(Level::Warning, logger.level(), _src_loc);
}

void log_level_output(Test& t)
{
   Logger& logger = Logger::get_logger();

   logger.level(Level::Warning);

   output_record.level(Level::NotSet);

   LOG(Debug) << "Debug message";
   t.assert<std::equal_to<>>(Level::NotSet, output_record.level(), _src_loc);

   output_record.level(Level::NotSet);

   LOG(Warning) << "Warning message";
   t.assert<std::equal_to<>>(Level::Warning, output_record.level(), _src_loc);

   output_record.level(Level::NotSet);

   LOG(Error) << "Error message";
   t.assert<std::equal_to<>>(Level::Error, output_record.level(), _src_loc);
}

void log_error_message(Test& t)
{
   Logger& logger = Logger::get_logger();

   logger.level(Level::NotSet);

   LOG(Error) << "Error message";

   t.assert<std::equal_to<>>(Level::Error, output_record.level(), _src_loc);
   t.assert<std::equal_to<>>(std::string{"Error message"}, output_record.message(), _src_loc);
}

void log_fail_message(Test& t)
{
   Logger& logger = Logger::get_logger();

   logger.level(Level::NotSet);

   output_record.level(Level::NotSet);

   LOG_IF_FAIL(Error, 1 == 0)

   t.assert<std::equal_to<>>(Level::Error, output_record.level(), _src_loc);
   t.assert<std::equal_to<>>(std::string{"Condition failed ( 1 == 0 )"}, output_record.message(), _src_loc);
}

void log_record_output(Test& t)
{
   Logger& logger = Logger::get_logger();

   logger += LogRecord(Level::Message, "FileName", 99, "function name") << "message";

   t.assert<std::equal_to<>>(Level::Message, output_record.level(), _src_loc);
   t.assert<std::equal_to<>>(std::string{"FileName"}, output_record.file_name(), _src_loc);
   t.assert<std::equal_to<>>(99, output_record.line(), _src_loc);
   t.assert<std::equal_to<>>(std::string{"function name"}, output_record.function_name(), _src_loc);
   t.assert<std::equal_to<>>(std::string{"message"}, output_record.message(), _src_loc);
}

RegisterTestCase(OrionCore, log_default_level);
RegisterTestCase(OrionCore, log_level_change);
RegisterTestCase(OrionCore, log_level_output);
RegisterTestCase(OrionCore, log_error_message);
RegisterTestCase(OrionCore, log_fail_message);
RegisterTestCase(OrionCore, log_record_output);
} // TEST_SUITE(OrionCore)

void setup_logger(LogRecord& record)
{
   auto out_handler = LogRecordOutputHandler::create(record);

   Logger& logger = Logger::get_logger();

   logger.level(Level::NotSet);
   logger.output_handlers().push_back(std::move(out_handler));
}
