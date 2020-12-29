#!/bin/sh
pid=`../run_scripts/thread-name.sh | grep 'GpsRead' | awk {'print $3'}`
echo 'gps -- thread id '${pid}
kill -18 ${pid}

