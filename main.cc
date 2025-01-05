#include "monitor_keeper.h"
#include <iostream>

using json = nlohmann::json;

int main(int argc, char** argv) {
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^---%L---%$] [pid %P thread %t] %v");

  spdlog::info("running");

  if(argc != 2) {
    std::cerr << "Usage: monitor_keeper path_to_config.json" << std::endl;
    return 1;
  }

  mk::Config config;

  std::string configPath(argv[1]);
  spdlog::info("configPath: {}", configPath);
  mk::Config::fromFile(configPath, config);
  config.check();

  spdlog::info("diskKeepPercent,{}", config.diskKeepPercent);
  spdlog::info("dryRun,{}", config.dryRun);

  for (std::string p : config.videoFilePaths) {
    spdlog::info("videoFilePath,{}", p);
    mk::VideoKeeper::doKeep(config, p);
  }

  return 0;
}
