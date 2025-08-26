
#ifndef USPINLOCK_H
#define USPINLOCK_H

typedef struct {
	volatile uint32_t lock;
} uspinlock_t;

#if CONFIG_IS_ENABLED(SMP)
void u_spin_lock(uspinlock_t *lock);
void u_spin_unlock(uspinlock_t *lock);
#else
static inline void u_spin_lock(uspinlock_t *lock) {}
static inline void u_spin_unlock(uspinlock_t *lock) {}
#endif

#endif