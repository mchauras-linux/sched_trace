#ifndef SCHED_TP_HELPERS_H
#define SCHED_TP_HELPERS_H

/* Required for struct irq_work which is defined in struct root_domain */
//#include <linux/irq_work.h>

//#include <linux/cgroup.h>

#include "vmlinux.h"

static inline const struct sched_avg *sched_tp_cfs_rq_avg(struct cfs_rq *cfs_rq)
{
#ifdef CONFIG_SMP
	return cfs_rq ? &cfs_rq->avg : NULL;
#else
	return NULL;
#endif
}

static inline bool task_group_is_autogroup(struct task_group *tg)
{
#ifdef CONFIG_SCHED_AUTOGROUP
	return !!tg->autogroup;
#else
	return false;
#endif
}

int autogroup_path(struct task_group *tg, char *buf, int buflen)
{
#ifdef CONFIG_SCHED_AUTOGROUP
	if (!task_group_is_autogroup(tg))
		return 0;

	return snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
#else
	return 0;
#endif
}

#ifdef CONFIG_FAIR_GROUP_SCHED
static inline struct rq *rq_of(struct cfs_rq *cfs_rq)
{
	return cfs_rq->rq;
}
#define entity_is_task(se)	(!se->my_q)
#else
static inline struct rq *rq_of(struct cfs_rq *cfs_rq)
{
	return container_of(cfs_rq, struct rq, cfs);
}
#define entity_is_task(se)	1
#endif

static inline int cpu_of(struct rq *rq)
{
#ifdef CONFIG_SMP
	return rq->cpu;
#else
	return 0;
#endif
}

static inline void cfs_rq_tg_path(struct cfs_rq *cfs_rq, char *path, int len)
{
	if (!path)
		return;

#ifdef CONFIG_FAIR_GROUP_SCHED
	if (cfs_rq && task_group_is_autogroup(cfs_rq->tg))
		autogroup_path(cfs_rq->tg, path, len);
	else if (cfs_rq && cfs_rq->tg->css.cgroup)
		cgroup_path(cfs_rq->tg->css.cgroup, path, len);
	else
#endif
		strlcpy(path, "(null)", len);
}

static inline char *sched_tp_cfs_rq_path(struct cfs_rq *cfs_rq, char *str, int len)
{
	if (!cfs_rq) {
		if (str)
			strlcpy(str, "(null)", len);
		else
			return NULL;
	}

	cfs_rq_tg_path(cfs_rq, str, len);
	return str;
}

static inline int sched_tp_cfs_rq_cpu(struct cfs_rq *cfs_rq)
{
	return cfs_rq ? cpu_of(rq_of(cfs_rq)) : -1;
}

static inline int sched_tp_rq_cpu(struct rq *rq)
{
	return rq ? cpu_of(rq) : -1;
}

static inline int sched_tp_rq_nr_running(struct rq *rq)
{
	return rq ? rq->nr_running : -1;
}

#endif /* SCHED_TP_HELPERS */
