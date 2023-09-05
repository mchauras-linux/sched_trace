#include <linux/module.h>
#include <linux/sched.h>

#include <trace/events/sched.h>

#include "sched_events.h"

void sched_trace_pelt_cfs(void *data, struct cfs_rq *cfs_rq) {
	const struct sched_avg *avg;
	char path[PATH_SIZE];
	int cpu;

	if (trace_sched_pelt_cfs_enabled()) {
		avg = sched_tp_cfs_rq_avg(cfs_rq);
		sched_tp_cfs_rq_path(cfs_rq, path, PATH_SIZE);
		cpu = sched_tp_cfs_rq_cpu(cfs_rq);

		trace_sched_pelt_cfs(cpu, path, avg);
	}
}

static int sched_trace_init(void) {
	register_trace_pelt_cfs_tp(sched_trace_pelt_cfs, NULL);
//	register_trace_sched_update_nr_running_tp(sched_update_nr_running, NULL);
	return 0;
}

static void sched_trace_exit(void) {
	unregister_trace_pelt_cfs_tp(sched_trace_pelt_cfs, NULL);
//	unregister_trace_sched_update_nr_running_tp(sched_trace_update_nr_running, NULL);
}

module_init(sched_trace_init);
module_exit(sched_trace_exit);

MODULE_AUTHOR("Mukesh Kumar Chaurasiya");
MODULE_DESCRIPTION("Pelt CFS tracing and NR Running");
MODULE_LICENSE("GPL");

