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
using namespace orion::unittest::option;

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

TestSuite(OrionCore_Logger, SetupFunc{setup_logger})
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
void log_default_level(Test& t)
{
   t.assert<std::equal_to<>>(Level::NotSet, logger.level(), _src_loc);
}

void log_level_change(Test& t)
{
   logger.level(Level::Warning);

   t.assert<std::equal_to<>>(Level::Warning, logger.level(), _src_loc);
}

void log_level_output(Test& t)
{
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
   logger.level(Level::NotSet);

   LOG(Error) << "Error message";

   t.assert<std::equal_to<>>(Level::Error, output_record.level(), _src_loc);
   t.assert<std::equal_to<>>(std::string{"Error message"}, output_record.message(), _src_loc);
}

void log_fail_message(Test& t)
{
   logger.level(Level::NotSet);

   output_record.level(Level::NotSet);

   LOG_IF_FAIL(Error, 1 == 0)

   t.assert<std::equal_to<>>(Level::Error, output_record.level(), _src_loc);
   t.assert<std::equal_to<>>(std::string{"Condition failed ( 1 == 0 )"}, output_record.message(), _src_loc);
}

void log_record_output(Test& t)
{
   logger.write(Record(Level::Message, "", SourceLocation{"FileName", 99, "function name"}) << "message");

   auto sl = output_record.source_location();

   t.assert<std::equal_to<>>(Level::Message, output_record.level(), _src_loc);
   t.assert<std::equal_to<>>(std::string{"FileName"}, sl.file_name, _src_loc);
   t.assert<std::equal_to<>>(99, sl.line_number, _src_loc);
   t.assert<std::equal_to<>>(std::string{"function name"}, sl.function_name, _src_loc);
   t.assert<std::equal_to<>>(std::string{"message"}, output_record.message(), _src_loc);
}

RegisterTestCase(OrionCore_Logger, log_default_level);
RegisterTestCase(OrionCore_Logger, log_level_change);
RegisterTestCase(OrionCore_Logger, log_level_output);
RegisterTestCase(OrionCore_Logger, log_error_message);
RegisterTestCase(OrionCore_Logger, log_fail_message);
RegisterTestCase(OrionCore_Logger, log_record_output);
} // TEST_SUITE(OrionCore_Logger)
