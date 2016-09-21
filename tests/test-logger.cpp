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

//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TEST(TestLogDefaultLevel)
{
   Logger& logger = Logger::get_logger();

   EXPECT(logger.level() == Level::NotSet);
}

TEST(TestLogLevelChange)
{
   Logger& logger = Logger::get_logger();

   logger.level(Level::Warning);

   EXPECT(logger.level() == Level::Warning);
}

TEST(TestLogLevelOutput)
{
   Logger& logger = Logger::get_logger();

   logger.level(Level::Warning);

   output_record.level(Level::NotSet);

   LOG(Debug) << "Debug message";
   EXPECT(output_record.level() == Level::NotSet);

   output_record.level(Level::NotSet);

   LOG(Warning) << "Warning message";
   EXPECT(output_record.level() == Level::Warning);

   output_record.level(Level::NotSet);

   LOG(Error) << "Error message";
   EXPECT(output_record.level() == Level::Error);
}

TEST(TestLogErrorMessage)
{
   Logger& logger = Logger::get_logger();

   logger.level(Level::NotSet);

   LOG(Error) << "Error message";

   EXPECT(output_record.level() == Level::Error);
   EXPECT(output_record.message() == "Error message");
}

TEST(TestLogFailMessage)
{
   Logger& logger = Logger::get_logger();

   logger.level(Level::NotSet);

   output_record.level(Level::NotSet);

   LOG_IF_FAIL(Error, 1 == 0)

   EXPECT(output_record.level() == Level::Error);
   EXPECT(output_record.message() == "Condition failed ( 1 == 0 )");
}

TEST(TestLogRecordOutput)
{
   Logger& logger = Logger::get_logger();

   logger += LogRecord(Level::Message, "FileName", 99, "function name") << "message";

   EXPECT(output_record.level() == Level::Message);
   EXPECT(output_record.file_name() == "FileName");
   EXPECT(output_record.line() == 99);
   EXPECT(output_record.function_name() == "function name");
   EXPECT(output_record.message() == "message");
}

void setup_logger(LogRecord& record)
{
   auto out_handler = LogRecordOutputHandler::create(record);

   Logger& logger = Logger::get_logger();

   logger.level(Level::NotSet);
   logger.output_handlers().push_back(std::move(out_handler));
}
