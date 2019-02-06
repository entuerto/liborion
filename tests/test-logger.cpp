//
//  test-logger.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/Test.h>

using namespace orion;
using namespace orion::log;
using namespace orion::unittest;

Record output_record;
Logger& logger = default_logger();

//----------------------------------------------------------------------------
// Test log classes
//----------------------------------------------------------------------------
class RecordOutputHandler : public OutputHandler
{
public:
   NO_COPY(RecordOutputHandler)
   NO_MOVE(RecordOutputHandler)
   
   RecordOutputHandler(Record& record) : _out_record(record) {}
   virtual ~RecordOutputHandler() {}

   virtual void write(const Record& record) override
   {
      _out_record.level(record.level());
      _out_record.message(record.message());
      _out_record.source_location(record.source_location());
   }

   virtual void flush() override {}

   virtual void close() override {}

private:
   Record& _out_record;
};

static void setup_logger()
{
   auto out_handler = std::make_unique<RecordOutputHandler>(output_record);

   logger.level(Level::NotSet);
   logger.add_output_handler(std::move(out_handler));

   logger.start([] { return ""; });
}

Section(OrionCore_Logger, Label{"Logger"}, SetupFunc{::setup_logger})
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TestCase("Log default level")
{
   check_eq(Level::NotSet, logger.level());
}

TestCase("Log level changed")
{
   logger.level(Level::Warning);

   check_eq(Level::Warning, logger.level());
}

TestCase("Log level output")
{
   logger.level(Level::Warning);

   output_record.level(Level::NotSet);

   LOG(Debug) << "Debug message";

   check_eq(Level::NotSet, output_record.level());

   output_record.level(Level::NotSet);

   LOG(Warning) << "Warning message";

   check_eq(Level::Warning, output_record.level());

   output_record.level(Level::NotSet);

   LOG(Error) << "Error message";

   check_eq(Level::Error, output_record.level());
}

TestCase("Log error message")
{
   logger.level(Level::NotSet);

   LOG(Error) << "Error message";

   check_eq(Level::Error, output_record.level());
   check_eq(std::string{"Error message"}, output_record.message());
}

TestCase("Log fail message")
{
   logger.level(Level::NotSet);

   output_record.level(Level::NotSet);

   LOG_IF_FAIL(Error, 1 == 0)

   check_eq(Level::Error, output_record.level());
   check_eq(std::string{"Condition failed ( 1 == 0 )"}, output_record.message());
}

TestCase("Log output record")
{
   logger.write(Record(Level::Message, "", SourceLocation{"FileName", 99, "function name"}) << "message");

   auto sl = output_record.source_location();

   check_eq(Level::Message, output_record.level());
   check_eq(std::string{"FileName"}, sl.file_name());
   check_eq(99, sl.line_number());
   check_eq(std::string{"function name"}, sl.function_name());
   check_eq(std::string{"message"}, output_record.message());
}

} // TestSuite(OrionCore_Logger)
