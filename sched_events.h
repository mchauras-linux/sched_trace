/* SPDX-License-Identifier: GPL-2.0 */
#undef TRACE_SYSTEM
#define TRACE_SYSTEM sched

#if !defined(_SCHED_EVENTS_H) || defined(TRACE_HEADER_MULTI_READ)
#define _SCHED_EVENTS_H

#include <linux/version.h>
#include <linux/tracepoint.h>

#include "sched_tp_helpers.h"

#define RBL_LOAD_ENTRY		runnable
#define RBL_LOAD_MEMBER		runnable_avg
#define RBL_LOAD_STR		"runnable"

#define PATH_SIZE 64

TRACE_EVENT(sched_pelt_cfs,

	TP_PROTO(int cpu, char *path, const struct sched_avg *avg),

	TP_ARGS(cpu, path, avg),

	TP_STRUCT__entry(
		__field(	int,		cpu			)
		__array(	char,		path,	PATH_SIZE	)
		__field(	unsigned long,	load			)
		__field(	unsigned long,	RBL_LOAD_ENTRY		)
		__field(	unsigned long,	util			)
		__field(	unsigned long long, update_time	        )
	),

	TP_fast_assign(
		__entry->cpu		= cpu;
		strlcpy(__entry->path, path, PATH_SIZE);
		__entry->load		= avg->load_avg;
		__entry->RBL_LOAD_ENTRY	= avg->RBL_LOAD_MEMBER;
		__entry->util		= avg->util_avg;
		__entry->update_time    = avg->last_update_time;
	),

	TP_printk("cpu=%d path=%s load=%lu " RBL_LOAD_STR "=%lu util=%lu update_time=%llu",
		  __entry->cpu, __entry->path, __entry->load,
		  __entry->RBL_LOAD_ENTRY,__entry->util, __entry->update_time)
);

TRACE_EVENT(sched_update_nr_running,

	    TP_PROTO(int cpu, int change, unsigned int nr_running, int level, int d_nr),

	    TP_ARGS(cpu, change, nr_running, level, d_nr),

	    TP_STRUCT__entry(
			     __field(         int,        cpu           )
			     __field(         int,        change        )
			     __field(unsigned int,        nr_running    )
			     __field(         int,        level         )
			     __field(         int,        d_nr          )
			     ),

	    TP_fast_assign(
			   __entry->cpu        = cpu;
			   __entry->change     = change;
			   __entry->nr_running = nr_running;
			   __entry->level      = level;
			   __entry->d_nr       = d_nr;
			   ),

	    TP_printk("cpu=%d change=%d nr_running=%d domain_level=%d domain_nr=%d", __entry->cpu, __entry->change, __entry->nr_running, __entry->level, __entry->d_nr)
	    );
#endif /* _SCHED_EVENTS_H */

/* This part must be outside protection */
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .
#define TRACE_INCLUDE_FILE sched_events
#include <trace/define_trace.h>
