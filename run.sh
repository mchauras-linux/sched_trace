#! /bin/bash
cleanup() {
	rmmod sched_trace.ko
}
git pull

make KERNEL_SRC=$PWD/../linux-mchauras
insmod sched_trace.ko

echo 1 > /sys/kernel/debug/tracing/events/sched/sched_update_nr_running/enable
sleep 1
echo 0 > /sys/kernel/debug/tracing/events/sched/sched_update_nr_running/enable
cat /sys/kernel/debug/tracing/trace

trap cleanup EXIT
