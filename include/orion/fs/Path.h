//  Path.h
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#ifndef ORION_FS_PATH_H
#define ORION_FS_PATH_H

#include <string>

#include <orion/Orion-Stddefs.h>

#ifdef _WIN32
  #define OS_SEPARATOR '\\'
#else
  #define OS_SEPARATOR '/'
#endif

namespace orion
{
namespace fs
{

class API_EXPORT Path
{
public:
   using value_type = char;
   static value_type slash;
   static value_type back_slash;
   static value_type separator;

   Path();
   Path(const std::string& str);
   Path(const Path& Other);
   Path(Path&& Other);
   virtual ~Path();

   Path& operator=(const Path& Rhs);
   Path& operator=(Path&& Rhs);

   Path& operator/=(const Path& p);
   Path& operator/=(const std::string& str);

   void join(const Path& p);
   void join(const std::string& str);

   bool empty() const;

   Path&  normalize(value_type s = separator);

   // Compare
   int compare(const Path& p) const;
   int compare(const std::string& s) const;

   // Cast to string
   explicit operator std::string();
   explicit operator const std::string() const;

   // Cast to wstring
   explicit operator std::wstring();
   explicit operator const std::wstring() const;

   // Decomposition  
   Path root_path() const; 
   Path root_name() const;         
   Path root_directory() const;    
   Path relative_path() const;
   Path parent_path() const;
   Path filename() const;          
   Path stem() const;              
   Path extension() const;  

private:
   std::string _pathname;
};

//---------------------------------------------------------------------------------------
inline bool operator==(const Path& lhs, const Path& rhs)        {return lhs.compare(rhs) == 0;}
inline bool operator==(const Path& lhs, const std::string& rhs) {return lhs.compare(rhs) == 0;} 
inline bool operator==(const std::string& lhs, const Path& rhs) {return rhs.compare(rhs) == 0;} 

inline bool operator!=(const Path& lhs, const Path& rhs)        {return lhs.compare(rhs) != 0;}
inline bool operator!=(const Path& lhs, const std::string& rhs) {return lhs.compare(rhs) != 0;} 
inline bool operator!=(const std::string& lhs, const Path& rhs) {return rhs.compare(rhs) != 0;} 

inline bool operator<(const Path& lhs, const Path& rhs)  {return lhs.compare(rhs) < 0;}
inline bool operator>(const Path& lhs, const Path& rhs)  {return lhs.compare(rhs) > 0;}

//---------------------------------------------------------------------------------------

inline Path operator/(const Path& lhs, const Path& rhs) { return Path(lhs) /= rhs; }

//---------------------------------------------------------------------------------------
inline std::ostream& operator<< (std::ostream& out, const Path& p) 
{ 
   out << static_cast<std::string>(p); 
   return out;
}

} // fs
} // orion

namespace std
{
   inline std::string to_string(const orion::fs::Path& p)
   {
      return static_cast<std::string>(p);
   }
}
#endif
