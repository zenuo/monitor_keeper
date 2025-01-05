#include "gtest/gtest.h"
#include "monitor_keeper.h"
#include <nlohmann/json.hpp>

TEST(CONFIG, FROM_JSON) {
  json j = R"({
  "videoFilePaths": ["/mnt/videos"],
  "diskKeepPercent":80,
  "dryRun":false
})"_json;

  mk::Config c;

  mk::Config::fromJson(j, c);

  EXPECT_EQ(c.diskKeepPercent,80);

  std::vector<std::string> paths;
  paths.push_back("/mnt/videos");
  EXPECT_EQ(c.videoFilePaths, paths);

  EXPECT_EQ(false, c.dryRun);
}

TEST(CONFIG, FROM_JSON_DEFAULT) {
  json j = R"({
  "videoFilePaths": ["/mnt/videos"]
})"_json;

  mk::Config c;

  mk::Config::fromJson(j, c);

  EXPECT_EQ(c.diskKeepPercent,90);

  std::vector<std::string> paths;
  paths.push_back("/mnt/videos");
  EXPECT_EQ(c.videoFilePaths, paths);

  EXPECT_EQ(true, c.dryRun);
}
