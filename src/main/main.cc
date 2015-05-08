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

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#ifndef NDEBUG
# include <algorithm>
# include <iterator>
#endif /* NDEBUG */

#include <fstream>
#include <iostream>
#include <vector>

#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "CL.hh"

#include "Types.hh"
namespace itypes = types;

#include "RMQ.hh"
namespace irmq = rmq;

typedef boost::tokenizer <boost::escaped_list_separator <char> > Tokenizer;


int main(int argc, char *argv[])
{
  args_t args;
  CL cl;

  if (cl.parse(argc, argv, args)) {
    return EXIT_SUCCESS;
  }

  // parse the euler circuit
  std::ifstream eulerCircuitFile;
  eulerCircuitFile.open(args.euler_circuit.c_str());
  if (!eulerCircuitFile.is_open()) {
    std::cerr << "Could not open file: " << args.euler_circuit << std::endl;
    return EXIT_FAILURE;
  }

#ifndef NDEBUG
  std::cout << "Reading the Euler Circuit..." << std::endl;
#endif /* NDEBUG */

  itypes::StringVector euler_circuit;
  std::string line;
  while (!eulerCircuitFile.eof()) {
    std::getline(eulerCircuitFile, line);
#ifndef NDEBUG
    std::cout << "Read line: " << line << std::endl;
#endif /* NDEBUG */

    if (line != "") {
      boost::algorithm::trim(line);
      euler_circuit.push_back(line);
    }
  }
  eulerCircuitFile.close();

  // parse the euler levels
  std::ifstream eulerLevelsFile;
  eulerLevelsFile.open(args.euler_levels.c_str());
  if (!eulerLevelsFile.is_open()) {
    std::cerr << "Could not open file: " << args.euler_levels << std::endl;
    return EXIT_FAILURE;
  }

#ifndef NDEBUG
  std::cout << "Reading the Euler Levels..." << std::endl;
#endif /* NDEBUG */

  itypes::IntVector euler_levels;
  while (!eulerLevelsFile.eof()) {
    std::getline(eulerLevelsFile, line);
#ifndef NDEBUG
    std::cout << "Read line: " << line << std::endl;
#endif /* NDEBUG */

    if (line != "") {
      boost::algorithm::trim(line);
      euler_levels.push_back(boost::lexical_cast<boost::uint32_t>(line));
    }
  }
  eulerLevelsFile.close();


  // parse the euler positions
  std::ifstream eulerPositionsFile;
  eulerPositionsFile.open(args.euler_positions.c_str());
  if (!eulerPositionsFile.is_open()) {
    std::cerr << "Could not open file: " << args.euler_positions << std::endl;
    return EXIT_FAILURE;
  }

#ifndef NDEBUG
  std::cout << "Reading the Euler Positions..." << std::endl;
#endif /* NDEBUG */

  itypes::StringIntMap euler_positions;
  while (!eulerPositionsFile.eof()) {
    std::getline(eulerPositionsFile, line);
#ifndef NDEBUG
    std::cout << "Read line: " << line << std::endl;
#endif /* NDEBUG */

    if (line != "") {
      itypes::StringVector tokens;
      boost::split(tokens, line, boost::is_any_of(","), boost::token_compress_on);
      if (tokens.size() != 2) {
        eulerPositionsFile.close();
        std::cerr << "Each line of the euler positions file can only contain two tokens: <key>,<value>!" << std::endl;
        return EXIT_FAILURE;
      } else {
        if (euler_positions.find(tokens[0]) != euler_positions.end()) {
          eulerPositionsFile.close();
          std::cerr << "The Euler Positions cannot contain duplicates: " << tokens[0] << std::endl;
          return EXIT_FAILURE;
        } else {
          euler_positions[tokens[0]] = boost::lexical_cast<boost::uint32_t>(tokens[1]);
        }
      }
    }
  }
  eulerPositionsFile.close();

#ifndef NDEBUG
  std::cout << "Euler Circuit: ";
  std::copy(euler_circuit.begin(), euler_circuit.end(), std::ostream_iterator<std::string>(std::cout, "\t"));
  std::cout << std::endl << "Euler Levels:  ";
  std::copy(euler_levels.begin(), euler_levels.end(), std::ostream_iterator<boost::uint32_t>(std::cout, "\t"));
  std::cout << std::endl;
#endif /* NDEBUG */

  std::ifstream verticesFile;
  verticesFile.open(args.query_set.c_str());
  if (!verticesFile.is_open()) {
    std::cerr << "Could not open file: " << args.query_set << std::endl;
    return EXIT_FAILURE;
  }

#ifndef NDEBUG
  std::cout << "Reading the vertices to compute LCAs over..." << std::endl;
#endif /* NDEBUG */

  itypes::StringVector queries;
  while (!verticesFile.eof()) {
    std::getline(verticesFile, line);
#ifndef NDEBUG
    std::cout << "Read line: " << line << std::endl;
#endif /* NDEBUG */

    if (line != "") {
      boost::algorithm::trim(line);
      queries.push_back(line);
    }
  }
  verticesFile.close();

  // perform the queries
  irmq::RMQ rmq;
  rmq.initialise(euler_levels);

  std::string outFile = args.results_dir + "/lca.csv";
  std::ofstream out(outFile.c_str(), std::ios::out);

  #pragma omp parallel for shared(out, queries, euler_circuit, euler_positions, euler_levels, rmq) default(none)
  for (boost::uint32_t i = 0; i < (queries.size() - 1); ++i) {
    itypes::StringStringMap lcas;
    for (boost::uint32_t j = i + 1; j < queries.size(); ++j) {
      std::string pair = queries[i] + "," + queries[j];
      std::string left = euler_positions[queries[i]] < euler_positions[queries[j]] ? queries[i] : queries[j];
      std::string right = euler_positions[queries[j]] >= euler_positions[queries[i]] ? queries[j] : queries[i];

#ifndef NDEBUG
      std::cout << "ID: " << left << " -> " << right << std::endl;
      std::cout << "Pos: " << euler_positions[left] << " -> " << euler_positions[right] << std::endl;
#endif /* NDEBUG */

      std::string lca = euler_circuit[rmq.query(euler_levels, 1, 0, euler_levels.size() -1, euler_positions[left], euler_positions[right])];
      lcas[pair] = lca;

#ifndef NDEBUG
      std::cout << pair << ": " << lca << std::endl;
#endif /* NDEBUG */
    }
    #pragma omp critical
    {
      itypes::StringStringMap::iterator it_begin = lcas.begin(), it_end = lcas.end();
      for (; it_begin != it_end; it_begin++) {
        out << it_begin->first << "," << it_begin->second << std::endl;
      }
    }
  }
  out.close();

  return EXIT_SUCCESS;
}
