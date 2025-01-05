#!/usr/bin/env bash

ffmpeg -hide_banner -y \
    -loglevel error \
    -use_wallclock_as_timestamps 1 \
    -rtsp_transport tcp \
    -i $MONITOR_DUMP_URL \
    -vcodec copy \
    -acodec copy \
    -f segment \
    -reset_timestamps 1 \
    -segment_time 600 \
    -segment_atclocktime 1 \
    -segment_format mkv \
    -strftime 1 \
    "./videos/%Y%m%dT%H%M%S.mkv"
