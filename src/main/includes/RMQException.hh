// Copyright (C) 2015 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file RMQException.hh
 * Declaration of the RMQ exceptions.
 *
 * @author Dominik Dahlem
 */
#ifndef __RMQ_EXCEPTION_HH__
#define __RMQ_EXCEPTION_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>

#include <exception>
using std::exception;


namespace rmq
{

/**
 * This exception specifies errors that may occur in the least common
 * ancestor implementation.
 *
 * @author <a href="mailto:Dominik.Dahlem@gmail.com">Dominik Dahlem</a>
 */
class RMQException: public exception
{
 public:
  RMQException(boost::uint8_t p_code);

  const char* what() const throw();
  boost::uint8_t errorCode() const;

  enum Errors {
    BAD_CAST = 0,
    NUM_ERRORS
  };

 private:
  static const char* m_errorStrings[NUM_ERRORS];
  boost::uint8_t m_code;
};

}

#endif
