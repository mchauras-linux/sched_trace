#include "sched_tp_helpers.h"
#include <linux/module.h>
#include <linux/sched.h>

#include <trace/events/sched.h>

#define CREATE_TRACE_POINTS
#include "sched_events.h"

#define MAX_CPU 128

int per_cpu_nr_running[MAX_CPU];

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

static void sched_trace_update_nr_running(void *data, struct rq *rq, int change)
{
	if (trace_sched_update_nr_running_enabled()) {
		int cpu = sched_tp_rq_cpu(rq);
		int nr_running = sched_tp_rq_nr_running(rq);
		per_cpu_nr_running[cpu] = nr_running;
		struct sched_domain *sd_it = sched_tp_rq_sd(rq);
		while (sd_it != NULL) {
			int level = sched_tp_sd_level(sd_it);
			int domain_nr_running = 0;
			int cpu_it;
			int j = 0;
			for_each_cpu(cpu_it,sched_domain_span(sd_it)) {
				domain_nr_running += per_cpu_nr_running[cpu_it];
				j++;
			}
			trace_sched_update_nr_running(cpu, j, nr_running, level, domain_nr_running);
			sd_it = sd_it->parent;
		}
	}
}

static int sched_trace_init(void) {
	for(int i = 0; i < MAX_CPU; i++) {
		per_cpu_nr_running[i] = 0;
	}
	register_trace_pelt_cfs_tp(sched_trace_pelt_cfs, NULL);
	register_trace_sched_update_nr_running_tp(sched_trace_update_nr_running, NULL);
	return 0;
}

static void sched_trace_exit(void) {
	unregister_trace_pelt_cfs_tp(sched_trace_pelt_cfs, NULL);
	unregister_trace_sched_update_nr_running_tp(sched_trace_update_nr_running, NULL);
}

module_init(sched_trace_init);
module_exit(sched_trace_exit);

MODULE_AUTHOR("Mukesh Kumar Chaurasiya");
MODULE_DESCRIPTION("Pelt CFS tracing and NR Running");
MODULE_LICENSE("GPL");

