// CoPYRight (C) 2015 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

#include "RMQException.hh"


namespace rmq
{


const char* RMQException::m_errorStrings[NUM_ERRORS] =
{
    "Error casting object.",
};

RMQException::RMQException(boost::uint8_t p_code) : exception(), m_code(p_code) {}

const char* RMQException::what() const throw() {
    return m_errorStrings[m_code];
}

boost::uint8_t RMQException::errorCode() const {
    return m_code;
}

}
