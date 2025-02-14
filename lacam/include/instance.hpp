/*
 * instance definition
 */
#pragma once
#include <random>

#include "graph.hpp"
#include "utils.hpp"

struct Instance {
  const Graph G;  // graph
  Config starts;  // initial configuration
  Config goals;   // goal configuration
  std::vector<std::vector<Vertex*>>
      goal_sequences;  // agent id -> goal sequence
  const uint N;        // number of agents

  // for testing
  Instance(const std::string& map_filename,
           const std::vector<int>& start_indexes,
           const std::vector<int>& goal_indexes);
  Instance(const std::string& map_filename,
           const std::vector<int>& start_indexes,
           const std::vector<std::vector<int>>& goal_index_sequences);
  // for MAPF benchmark
  Instance(const std::string& scen_filename, const std::string& map_filename,
           const int _N = 1);
  // random instance generation
  Instance(const std::string& map_filename, std::mt19937* MT, const int _N = 1);
  ~Instance() {}

  // simple feasibility check of instance
  bool is_valid(const int verbose = 0) const;

  int get_total_goals() const;

  bool is_goal_config(const Config& C) const;

  std::vector<int> calculate_goal_indices(const Config& c,
                                          const Config& c_prev) const;
};

// solution: a sequence of configurations
using Solution = std::vector<Config>;
