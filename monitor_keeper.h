#ifndef MONITOR_KEEPER_H
#define MONITOR_KEEPER_H

#include "spdlog/spdlog.h"
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <stdexcept>
#include <sys/stat.h>
#include <cstdint>

using json = nlohmann::json;

namespace mk {

namespace fs = std::filesystem;

class Config{

public:
  // 视频存储的位置
  std::vector<std::string> videoFilePaths;
  // 若超过则删除最早的视频，直到达到这个百分比
  unsigned short diskKeepPercent = 90;
  // 是否试运行
  bool dryRun = true;

  void check() {
    if (diskKeepPercent < 0 || diskKeepPercent > 99) {
      throw std::invalid_argument("invalid diskKeepPercent");
    }
    for (std::string p : videoFilePaths) {
      if (!fs::exists(p)) {
        throw std::invalid_argument("path not exists:" + p);
      }
      if (!fs::is_directory(p)) {
        throw std::invalid_argument("path is not directory:" + p);
      }
    }
  }

  static void fromJson(const json& j, Config& c) {
    // https://github.com/nlohmann/json?tab=readme-ov-file#basic-usage
    if (j.contains("videoFilePaths")) {
      j.at("videoFilePaths").get_to(c.videoFilePaths);
    }
    if (j.contains("diskKeepPercent")) {
      j.at("diskKeepPercent").get_to(c.diskKeepPercent);
    }
    if (j.contains("dryRun")) {
      j.at("dryRun").get_to(c.dryRun);
    }
  }

  static void fromFile(const std::string path, Config& c) {
    std::ifstream f(path);
    json data = json::parse(f);
    fromJson(data, c);
  }
};

class VideoKeeper {
public:
  static void doKeep(Config& config, std::string path) {
    fs::space_info info = fs::space(path);

    // 理想的可用
    uintmax_t targetAvaiable = info.capacity * ((100 - config.diskKeepPercent) / 100.0);

    // 与理想的差值
    if (info.available >= targetAvaiable) {
      return;
    }

    uintmax_t diff = targetAvaiable-info.available;
    spdlog::info("targetAvaiable:{},diff:{}", targetAvaiable, diff);

    std::vector<fs::directory_entry> filesToBeDelete;

    for(const auto& f: listFile(path)) {
      fs::file_status s = f.status();
      if (s.type() == fs::file_type::regular){
        filesToBeDelete.push_back(f);
        if (diff <= f.file_size()) {
          break;
        }
        diff -= f.file_size();
        spdlog::info("file:{},size:{},diff:{}", f.path().c_str(), f.file_size(), diff);
      }
    }

    for(const auto& f: filesToBeDelete) {
      if (!config.dryRun) {
        fs::remove(f.path());
      }
      spdlog::info("file deleted: {}", f.path().c_str());
    }

  }

private:
  // 根据创建时间升序排列
  static std::vector<fs::directory_entry> listFile(std::string path) {
    std::vector<fs::directory_entry> files;

    for (const auto& f : fs::directory_iterator(path)) {
      files.push_back(f);
    }

    std::sort(files.begin(), files.end(),
              [](const fs::directory_entry& a, const fs::directory_entry& b) {
                struct stat statA,statB;

                stat(a.path().c_str(), &statA);
                stat(b.path().c_str(), &statB);

                return statA.st_ctime < statB.st_ctime;
              });

    return files;
  }
};

}

#endif // MONITOR_KEEPER_H
