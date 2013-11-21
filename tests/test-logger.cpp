
#include <fstream>
#include <iostream>
#include <orion/Logging.h>
#include <orion/TestUtils.h>

using namespace orion;
using namespace orion::logging;
using namespace orion::unittest;

LogRecord output_record;

//----------------------------------------------------------------------------
// Test log classes
//----------------------------------------------------------------------------
class LogRecordOutputHandler :
   public OutputHandler,
   NonCopyable
{
public:
   LogRecordOutputHandler(LogRecord& record) : _out_record(record) {}
   virtual ~LogRecordOutputHandler() {}

   virtual void write(const LogRecord& log_record)
   {
      _out_record = log_record;
   }

   virtual void flush() {}

   virtual void close() {}

   static OutputHandler::SharedPtr create(LogRecord& record)
   {
      return OutputHandler::SharedPtr(new LogRecordOutputHandler(record));
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

   EXPECT(logger.level() == Logger::All);
}

TEST(TestLogLevelChange)
{
   Logger& logger = Logger::get_logger();

   logger.level(Logger::Warning);

   EXPECT(logger.level() == Logger::Warning);
}

TEST(TestLogLevelOutput)
{
   Logger& logger = Logger::get_logger();

   logger.level(Logger::Warning);

   output_record.level(Logger::NotSet);

   LOG(Logger::Debug) << "Debug message";
   EXPECT(output_record.level() == Logger::NotSet);

   LOG(Logger::Warning) << "Warning message";
   EXPECT(output_record.level() == Logger::Warning);

   LOG(Logger::Error) << "Error message";
   EXPECT(output_record.level() == Logger::Error);
}

TEST(TestLogErrorMessage)
{
   Logger& logger = Logger::get_logger();

   logger.level(Logger::All);

   LOG(Logger::Error) << "Error message";

   EXPECT(output_record.level() == Logger::Error);
   EXPECT(output_record.message() == "Error message");
}

TEST(TestLogRecordOutput)
{
   Logger& logger = Logger::get_logger();

   logger += LogRecord(Logger::Message, "FileName", 99, "function name") << "message";

   EXPECT(output_record.level() == Logger::Message);
   EXPECT(output_record.file_name() == "FileName");
   EXPECT(output_record.line() == 99);
   EXPECT(output_record.function_name() == "function name");
   EXPECT(output_record.message() == "message");
}

//----------------------------------------------------------------------------
// Main functions
//----------------------------------------------------------------------------
void setup_logger(LogRecord& record)
{
   OutputHandler::SharedPtr out_handler = LogRecordOutputHandler::create(record);

   Logger& logger = Logger::get_logger();

   logger.level(Logger::All);
   logger.output_handlers().push_back(out_handler);
}

int main(int argc, char** argv)
{
   // Help with uncaught exceptions in g++
   //std::set_terminate(__gnu_cxx::__verbose_terminate_handler);

   setup_logger(output_record);

   LOG_START();

   int ret = run_all_tests(TestStdOutput::create(std::cout));

   LOG_END();
   return ret;
}
