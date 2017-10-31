//
// Connection.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#ifndef ORION_NET_CONNECTION_H
#define ORION_NET_CONNECTION_H

#include <orion/Orion-Stddefs.h>

#include <orion/detail/AsyncTypes.h>

#include <orion/net/EndPoint.h>

#include <chrono>
#include <system_error>

namespace orion
{
namespace net
{
//! This class provides a generic network connection
/*!
   Connection is a generic stream-oriented network connection.
 */
class API_EXPORT Connection
{
public:
   NO_COPY(Connection)

   Connection(IOService& io_service);
   virtual ~Connection();

   /// Close closes the connection.
   /// Any blocked Read or Write operations will be unblocked and return errors.
   virtual void close() =0;

   /// Returns the local network address.
   const EndPoint& local_endpoint() const;

   /// Set the local network address.
   void local_endpoint(const EndPoint& value);

   /// Returns the remote network address.
   const EndPoint& remote_endpoint() const;

   /// Set the remote network address.
   void remote_endpoint(const EndPoint& value);

   /// Sets the read and write deadlines associated
   /// with the connection. It is equivalent to calling both
   /// set_read_deadline and set_write_deadline.
   ///
   /// A deadline is an absolute time after which I/O operations
   /// fail with a timeout instead of blocking.
   ///
   /// A zero value for t means I/O operations will not time out.
   std::error_code deadline(const std::chrono::seconds& sec);

   /// Get the current value of the read and write deadline.
   std::chrono::seconds deadline() const;

   /// Sets the deadline for future Read calls.
   /// A zero value for t means Read will not time out.
   std::error_code read_deadline(const std::chrono::seconds& sec);

   /// Get the current value of the read deadline.
   std::chrono::seconds read_deadline() const;

   /// Sets the deadline for future Write calls.
   /// A zero value for t means Write will not time out.
   std::error_code write_deadline(const std::chrono::seconds& sec);

   /// Get the current value of the write deadline.
   std::chrono::seconds write_deadline() const;

   /// Handle timeout
   void on_read_timeout(const std::error_code& ec);
   void on_write_timeout(const std::error_code& ec);

   void start_read_timer();
   void start_write_timer();

   SteadyTimer& read_deadline_timer();
   SteadyTimer& write_deadline_timer();

private:
   EndPoint _local_endpoint;
   EndPoint _remote_endpoint;

   std::chrono::seconds _read_deadline;
   std::chrono::seconds _write_deadline;

   SteadyTimer _read_deadline_timer;
   SteadyTimer _write_deadline_timer;
};

} // net
} // orion
#endif
