#include <argparse/argparse.hpp>
#include <lacam.hpp>

int main(int argc, char* argv[])
{
  // arguments parser
  argparse::ArgumentParser program("early-exit-lacam", "0.1.0");
  program.add_argument("-m", "--map").help("map file").required();
  program.add_argument("-i", "--scen")
      .help("scenario file")
      .default_value(std::string(""));
  program.add_argument("-N", "--num").help("number of agents").required();
  program.add_argument("-s", "--seed")
      .help("seed")
      .default_value(std::string("0"));
  program.add_argument("-v", "--verbose")
      .help("verbose")
      .default_value(std::string("0"));
  program.add_argument("-t", "--time_limit_sec")
      .help("time limit sec")
      .default_value(std::string("10"));
  program.add_argument("-o", "--output")
      .help("output file")
      .default_value(std::string("./build/result.txt"));
  program.add_argument("-l", "--log_short")
      .default_value(false)
      .implicit_value(true);
  program.add_argument("-T", "--threshold")
      .help(
          "number of reached goals necessary to terminate. If -1, all agents "
          "must reach their goals.")
      .default_value(std::string("-1"));
  program.add_argument("-f", "--allow_following")
      .help("allow following conflicts")
      .default_value(false)
      .implicit_value(true);
  program.add_argument("--skip_post_processing")
      .help(
          "Skip potentially time-consuming post processing such as calculating "
          "sum of costs.")
      .default_value(false)
      .implicit_value(true);

  try {
    program.parse_known_args(argc, argv);
  } catch (const std::runtime_error& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(1);
  }

  // setup instance
  const auto verbose = std::stoi(program.get<std::string>("verbose"));
  const auto time_limit_sec =
      std::stoi(program.get<std::string>("time_limit_sec"));
  const auto scen_name = program.get<std::string>("scen");
  const auto seed = std::stoi(program.get<std::string>("seed"));
  auto MT = std::mt19937(seed);
  const auto map_name = program.get<std::string>("map");
  const auto output_name = program.get<std::string>("output");
  const auto log_short = program.get<bool>("log_short");
  const auto N = std::stoi(program.get<std::string>("num"));
  auto user_threshold = std::stoi(program.get<std::string>("threshold"));
  std::optional<int> threshold = std::nullopt;
  if (user_threshold == 0 || user_threshold > N) {
    info(0, verbose, "invalid threshold");
    return 1;
  } else if (user_threshold > 0) {
    threshold = user_threshold;
  }
  const auto allow_following = program.get<bool>("allow_following");
  const auto skip_post_processing = program.get<bool>("skip_post_processing");
  const auto ins = scen_name.size() > 0 ? Instance(scen_name, map_name, N)
                                        : Instance(map_name, &MT, N);
  if (!ins.is_valid(1)) return 1;

  // solve
  const auto deadline = Deadline(time_limit_sec * 1000);
  const auto solution =
      solve(ins, verbose - 1, &deadline, &MT, threshold, allow_following);
  const auto comp_time_ms = deadline.elapsed_ms();

  // failure
  if (solution.empty()) info(1, verbose, "failed to solve");

  // check feasibility
  if (!is_feasible_solution(ins, solution, verbose, threshold,
                            allow_following)) {
    info(0, verbose, "invalid solution");
    return 1;
  }

  // post processing
  if (!skip_post_processing) print_stats(verbose, ins, solution, comp_time_ms);
  make_log(ins, solution, output_name, comp_time_ms, map_name, seed, log_short,
           skip_post_processing);
  return 0;
}
