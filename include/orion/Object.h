/*
 * Object.h
 *
 * Copyright 2013 tomas <tomasp@videotron.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef ORION_OBJECT_H
#define ORION_OBJECT_H

#include <string>
#include <orion/Orion-Stddefs.h>
#include <orion/Uuid.h>

namespace orion
{
//! Base object
/*!
 */
class API_EXPORT Object 
{
public:
   Object();
   Object(const Object& rhs);
   virtual ~Object();

   virtual Uuid id() const;
   virtual void id(const Uuid& value);

   virtual std::string to_string() const;

   Object& operator=(const Object& rhs);

private:
   Uuid _uuid;
};

} // namespace orion
#endif 
