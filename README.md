## monitor keeper

## 是什么
存储监控视频到硬盘中，当硬盘占用达到某个设定的阈值时，删除最早的视频以满足这个阈值

## 构建

需要:

- cmake
- c++

执行命令:

```
mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DSPDLOG_FMT_EXTERNAL_HO=ON .. && make
```

## 附带脚本
todo

## 参考

- https://documentation.suse.com/smart/systems-management/html/systemd-working-with-timers/index.html
