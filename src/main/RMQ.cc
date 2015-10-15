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

/** @file RMQ.cc
 * Implementation of the RMQ routines
 *
 * @author Dominik Dahlem
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#ifndef NDEBUG
# include <algorithm>
# include <iostream>
# include <iterator>
#endif /* NDEBUG */

#include <cmath>
#include <cstring>

#include <boost/cstdint.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include "RMQ.hh"

namespace rmq
{

RMQ::RMQ() {}

RMQ::~RMQ()
{
  if (m_M) { delete [] m_M; }
}

void RMQ::initialise(itypes::DoubleVector p_A)
    throw (RMQException)
{
  try {
    boost::uint32_t N = boost::numeric_cast<boost::uint32_t>(p_A.size());
    boost::uint32_t x = boost::numeric_cast<boost::uint32_t>(std::ceil(log2(N))); // height of segment tree
    boost::uint32_t size = 2 * std::pow(2, x) - 1; // max size of segment tree
    m_M = new boost::int32_t[size];
    std::memset(m_M, -1, size * sizeof(boost::int32_t));

#ifndef NDEBUG
    std::cout << "Initialise M: " << std::endl;
    std::copy(m_M, m_M+size, std::ostream_iterator<boost::int32_t>(std::cout, "\t"));
    std::cout << std::endl;
#endif /* NDEBUG */

    create(p_A, 1, 0, N-1);
  } catch(boost::numeric::negative_overflow& e) {
    throw RMQException(RMQException::BAD_CAST);
  } catch(boost::numeric::positive_overflow& e) {
    throw RMQException(RMQException::BAD_CAST);
  }
}

boost::int32_t RMQ::query(itypes::DoubleVector p_A, boost::uint32_t p_node, boost::uint32_t p_lb,
                           boost::uint32_t p_ub, boost::uint32_t p_i, boost::uint32_t p_j)
{
  boost::int32_t p1, p2;

#ifndef NDEBUG
  itypes::DoubleVector::iterator b = p_A.begin(), e = p_A.begin();
  std::advance(b, p_lb);
  std::advance(e, p_ub+1);
  std::cout << "Current node: " << p_node << "=" << m_M[p_node] << std::endl;
  std::cout << "Query range [" << p_i << ", " << p_j << "] within array [" << p_lb << ", " << p_ub << "]: ";
  std::copy(b, e, std::ostream_iterator<boost::uint32_t>(std::cout, "\t"));
  std::cout << std::endl;
#endif /* NDEBUG */

  // query interval does not intersect
  if (p_i > p_ub || p_j < p_lb) {
#ifndef NDEBUG
    std::cout << "Query interval does not intersect" << std::endl;
#endif /* NDEBUG */
    return -1;
  }

  // the current interval is included in the query interval
  if (p_i <= p_lb && p_j >= p_ub) {
#ifndef NDEBUG
    std::cout << "The current interval is included in the query interval" << std::endl;
#endif /* NDEBUG */
    return m_M[p_node];
  }

  // compute the minimum position in the
  // left and right part of the interval
  boost::uint32_t midPoint = (p_lb + p_ub) / 2;
  p1 = query(p_A, 2 * p_node, p_lb, midPoint, p_i, p_j);
  p2 = query(p_A, 2 * p_node + 1, midPoint + 1, p_ub, p_i, p_j);

#ifndef NDEBUG
  std::cout << "p1=" << p1 << ", p2=" << p2 << std::endl;
#endif /* NDEBUG */

  // return minimum
  if (p1 == -1) { return p2; }
  if (p2 == -1) { return p1; }
  if (p_A[p1] <= p_A[p2]) {return p1; }

  return p2;
}

void RMQ::create(itypes::DoubleVector p_A, boost::uint32_t p_node, boost::uint32_t p_lb, boost::uint32_t p_ub)
{
  if (p_lb == p_ub) {
    m_M[p_node] = p_lb;
  } else {
    // create the left and right subtrees
    boost::uint32_t midPoint = (p_lb + p_ub) / 2;
    create(p_A, 2 * p_node, p_lb, midPoint);
    create(p_A, 2 * p_node + 1, midPoint + 1, p_ub);

    // search for the minimum value in the first and
    // second half of the interval
    if (p_A[m_M[2 * p_node]] <= p_A[m_M[2 * p_node + 1]]) {
      m_M[p_node] = m_M[2 * p_node];
    } else {
      m_M[p_node] = m_M[2 * p_node + 1];
    }
  }
}


}
