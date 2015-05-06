// Copyright (C) 2015 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file Types.hh
 * Declaration of some common types
 *
 * @author Dominik Dahlem
 */
#ifndef __TYPES_HH__
#define __TYPES_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <map>
#include <string>
#include <vector>

#include <boost/cstdint.hpp>


namespace types
{

typedef std::vector<std::string> StringVector;
typedef std::vector<boost::uint32_t> IntVector;
typedef std::map<std::string, boost::uint32_t> StringIntMap;
typedef std::map<std::string, std::string> StringStringMap;

}


#endif
