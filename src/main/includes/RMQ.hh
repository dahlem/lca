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

/** @file CL.hh
 * Declaration of the interface to range minimum queries.
 *
 * @author Dominik Dahlem
 */
#ifndef __MAIN_LCA_HH__
#define __MAIN_LCA_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>

#include "RMQException.hh"

#include "Types.hh"
namespace itypes = types;

namespace rmq
{

/** @class RMQ
 *
 * This class implements range minimum queries in linear
 * pre-processing time and logarithmic query time. Internally, the RMQ
 * uses segment trees to store the intervals.
 */
class RMQ
{
 public:
  /**
   * Construct an RMQ object with the underlying size N of the array.
   *
   * @param boost::uint32_t the size N of the array
   */
  RMQ();
  ~RMQ();

  /** @fn initialise(boost::uint32_t, boost::uint32_t, boost::uint32_t, itypes::IntVector, boost::uint32_t)
   *
   * Create the segment tree given an array. A segment tree allows
   * update/query operations in logarithmic time. The tree is created
   * in a recursive manner:
   *  - the first node will hold the information for the interval [i, j]
   *  - if i<j the left and right ild will hold the information for the
   *    intervals [i, (i+j)/2] and [(i+j)/2+1, j] respectively.
   *
   * @param itypes::IntVector the integer vector over which we create the segment tree
   * @throws RMQException if an error in the initialisation occurs
  */
  void initialise(itypes::IntVector) throw (RMQException);

 /** @fn query(itypes::IntVector, boost::uint32_t p_node, boost::uint32_t p_lb, boost::uint32_t p_up)
  *
  * Query the RMQ in the segment tree.
  * @param IntVector the integer vector over which we created the segment ree
  * @param boost::uint32_t the node ID
  * @param boost::uint32_t the lower bound of the interval
  * @param boost::uint32_t the upper bound of the interval
  * @param boost::uint32_t the lower range
  * @param boost::uint32_t the upper range
  */
  boost::int32_t query(itypes::IntVector, boost::uint32_t, boost::uint32_t, boost::uint32_t, boost::uint32_t, boost::uint32_t);

private:
 /** @fn create(itypes::IntVector, boost::uint32_t p_node, boost::uint32_t p_lb, boost::uint32_t p_up)
  *
  * Private method to create the segment tree.
  * @param IntVector the integer vector over which we create the segment ree
  * @param boost::uint32_t the node ID
  * @param boost::uint32_t the lower bound of the interval
  * @param boost::uint32_t the upper bound of the interval
  */
  void create(itypes::IntVector, boost::uint32_t, boost::uint32_t, boost::uint32_t);

  boost::int32_t *m_M;
};

}



#endif
