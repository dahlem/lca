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
 * Declaration of the command line parameters.
 *
 * @author Dominik Dahlem
 */
#ifndef __MAIN_CL_HH__
#define __MAIN_CL_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>

#include <boost/program_options/options_description.hpp>
namespace po = boost::program_options;

#include <boost/scoped_ptr.hpp>


/**
 * const variables specifying the allowed options.
 */
const std::string HELP = "help";
const std::string VERS = "version";

const std::string RESULTS_DIR = "results";
const std::string EULER_CIRCUIT = "euler_circuit";
const std::string EULER_LEVELS = "euler_levels";
const std::string EULER_POSITIONS = "euler_positions";
const std::string QUERY_SET = "query_set";


/** @struct
 * structure specifying the command line variables.
 */
struct args_t {
  std::string results_dir;        /* directory name for the results */
  std::string euler_circuit;      /* Filename containing the euler circuit */
  std::string euler_levels;       /* Levels of the vertices in the euler circuit */
  std::string euler_positions;    /* Positions of the vertices in the euler circuit */
  std::string query_set;          /* Query set of vertices to compute LCA over */

  args_t(args_t const &args)
      : results_dir(args.results_dir), euler_circuit(args.euler_circuit),
        euler_levels(args.euler_levels), euler_positions(args.euler_positions),
        query_set(args.query_set)
  {}

  args_t()
      : results_dir(""), euler_circuit(""), euler_levels(""), euler_positions(""), query_set("")
  {}

  friend std::ostream& operator <<(std::ostream &p_os, const args_t &p_args)
  {
    p_os << "Parameters" << std::endl << std::endl;
    p_os << "Results directory: " << p_args.results_dir << std::endl
         << "Euler Circuit:     " << p_args.euler_circuit << std::endl
         << "Euler Levels:      " << p_args.euler_levels << std::endl
         << "Euler Positions:   " << p_args.euler_positions << std::endl
         << "Query set:         " << p_args.query_set << std::endl
         << std::endl;

    return p_os;
  }
};



/** @class CL
 * This class uses the boost program-options library to parse the command-line
 * parameters for the main routine of the discrete event simulator.
 */
class CL
{
 public:
  CL();

  /** @fn parse(int argc, char *argv[], args_t);
   * Parse the command-line parameters and store the relevant information
   * in a shared pointer of a structure.
   *
   * @param int number of command-line arguments
   * @param char** the command-line arguments
   * @param args_t a reference to the structure of the command-line
   *        arguments
   * @return either success or failure. In case of a failure then the help
   *        message was requested.
   */
  int parse(int, char **, args_t &);

 private:

  /**
   * A scoped pointer to the description of the command-line arguments.
   */
  boost::scoped_ptr<po::options_description> m_opt_desc;
};



#endif
