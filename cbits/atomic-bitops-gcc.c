#include "atomic-bitops-gcc.h"

void mem_barrier (void) {return __sync_synchronize ();}

#define CAS_FETCH_AND_NAND(x, val)                                  \
  {                                                                 \
    __typeof__ (*(x)) tmp = *(x);                                   \
    while (!__sync_bool_compare_and_swap(x, tmp, ~(tmp & (val)))) { \
      tmp = *(x);                                                   \
    }                                                               \
    return tmp;                                                     \
  }

#define CAS_NAND_AND_FETCH(x, val)                                  \
  {                                                                 \
    while (!__sync_bool_compare_and_swap(x, *(x), ~(*(x) & (val)))) { \
      ;                                                             \
    }                                                               \
    return *(x);                                                    \
  }

// N.B. __has_builtin is only provided by clang
#if !defined(__has_builtin)
#define __has_builtin(x) 0
#endif

#if defined(__clang__) && __has_builtin(__sync_fetch_and_nand)
#define USE_SYNC_FETCH_AND_NAND 1
#define USE_SYNC_NAND_AND_FETCH 1
#else
#define USE_SYNC_FETCH_AND_NAND 0
#define USE_SYNC_NAND_AND_FETCH 0
#endif

// Otherwise this fails with -Werror
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma GCC diagnostic ignored "-Wsync-fetch-and-nand-semantics-changed"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsync-nand"
#endif

/* 8-bit */
unsigned char fetch_and_add_8 (unsigned char *p, unsigned char v) {
	return __sync_fetch_and_add (p, v);
}
unsigned char fetch_and_sub_8 (unsigned char *p, unsigned char v) {
	return __sync_fetch_and_sub (p, v);
}
unsigned char fetch_and_or_8 (unsigned char *p, unsigned char v) {
	return __sync_fetch_and_or (p, v);
}
unsigned char fetch_and_and_8 (unsigned char *p, unsigned char v) {
	return __sync_fetch_and_and (p, v);
}
unsigned char fetch_and_xor_8 (unsigned char *p, unsigned char v) {
	return __sync_fetch_and_xor (p, v);
}
unsigned char fetch_and_nand_8 (unsigned char *p, unsigned char v) {
#if USE_SYNC_FETCH_AND_NAND
	return __sync_fetch_and_nand(p, v);
#else
	CAS_FETCH_AND_NAND(p, v);
#endif
}
unsigned char add_and_fetch_8 (unsigned char *p, unsigned char v) {
	return __sync_add_and_fetch (p, v);
}
unsigned char sub_and_fetch_8 (unsigned char *p, unsigned char v) {
	return __sync_sub_and_fetch (p, v);
}
unsigned char or_and_fetch_8 (unsigned char *p, unsigned char v) {
	return __sync_or_and_fetch (p, v);
}
unsigned char and_and_fetch_8 (unsigned char *p, unsigned char v) {
	return __sync_and_and_fetch (p, v);
}
unsigned char xor_and_fetch_8 (unsigned char *p, unsigned char v) {
	return __sync_xor_and_fetch (p, v);
}
unsigned char nand_and_fetch_8 (unsigned char *p, unsigned char v) {
#if USE_SYNC_NAND_AND_FETCH
	return __sync_nand_and_fetch (p, v);
#else
	CAS_NAND_AND_FETCH(p, v);
#endif
}
unsigned int
bool_compare_and_swap_8 (unsigned char *p, unsigned char old, unsigned char new) {
	return __sync_bool_compare_and_swap (p, old, new);
}
unsigned char
val_compare_and_swap_8 (unsigned char *p, unsigned char old, unsigned char new) {
	return __sync_val_compare_and_swap (p, old, new);
}
unsigned char lock_test_and_set_8 (unsigned char *p) {
	// Only immediate 0/1 appear to be widely supported, so hardcode it
	// here
	return __sync_lock_test_and_set (p, 1);
}
void lock_release_8 (unsigned char *p) {
	// Writes a 0 to *p
	return __sync_lock_release (p);
}

/* 16-bit */
unsigned short fetch_and_add_16 (unsigned short *p, unsigned short v) {
	return __sync_fetch_and_add (p, v);
}
unsigned short fetch_and_sub_16 (unsigned short *p, unsigned short v) {
	return __sync_fetch_and_sub (p, v);
}
unsigned short fetch_and_or_16 (unsigned short *p, unsigned short v) {
	return __sync_fetch_and_or (p, v);
}
unsigned short fetch_and_and_16 (unsigned short *p, unsigned short v) {
	return __sync_fetch_and_and (p, v);
}
unsigned short fetch_and_xor_16 (unsigned short *p, unsigned short v) {
	return __sync_fetch_and_xor (p, v);
}
unsigned short fetch_and_nand_16 (unsigned short *p, unsigned short v) {
#if USE_SYNC_FETCH_AND_NAND
	return __sync_fetch_and_nand(p, v);
#else
	CAS_FETCH_AND_NAND(p, v);
#endif
}
unsigned short add_and_fetch_16 (unsigned short *p, unsigned short v) {
	return __sync_add_and_fetch (p, v);
}
unsigned short sub_and_fetch_16 (unsigned short *p, unsigned short v) {
	return __sync_sub_and_fetch (p, v);
}
unsigned short or_and_fetch_16 (unsigned short *p, unsigned short v) {
	return __sync_or_and_fetch (p, v);
}
unsigned short and_and_fetch_16 (unsigned short *p, unsigned short v) {
	return __sync_and_and_fetch (p, v);
}
unsigned short xor_and_fetch_16 (unsigned short *p, unsigned short v) {
	return __sync_xor_and_fetch (p, v);
}
unsigned short nand_and_fetch_16 (unsigned short *p, unsigned short v) {
#if USE_SYNC_NAND_AND_FETCH
	return __sync_nand_and_fetch (p, v);
#else
	CAS_NAND_AND_FETCH(p, v);
#endif
}
unsigned int
bool_compare_and_swap_16 (unsigned short *p, unsigned short old, unsigned short new) {
	return __sync_bool_compare_and_swap (p, old, new);
}
unsigned short
val_compare_and_swap_16 (unsigned short *p, unsigned short old, unsigned short new) {
	return __sync_val_compare_and_swap (p, old, new);
}
unsigned short lock_test_and_set_16 (unsigned short *p) {
	// Only immediate 0/1 appear to be widely supported, so hardcode it
	// here
	return __sync_lock_test_and_set (p, 1);
}
void lock_release_16 (unsigned short *p) {
	// Writes a 0 to *p
	return __sync_lock_release (p);
}

/* 32-bit */
unsigned int fetch_and_add_32 (unsigned int *p, unsigned int v) {
	return __sync_fetch_and_add (p, v);
}
unsigned int fetch_and_sub_32 (unsigned int *p, unsigned int v) {
	return __sync_fetch_and_sub (p, v);
}
unsigned int fetch_and_or_32 (unsigned int *p, unsigned int v) {
	return __sync_fetch_and_or (p, v);
}
unsigned int fetch_and_and_32 (unsigned int *p, unsigned int v) {
	return __sync_fetch_and_and (p, v);
}
unsigned int fetch_and_xor_32 (unsigned int *p, unsigned int v) {
	return __sync_fetch_and_xor (p, v);
}
unsigned int fetch_and_nand_32 (unsigned int *p, unsigned int v) {
#if USE_SYNC_FETCH_AND_NAND
  return __sync_fetch_and_nand(p, v);
#else
  CAS_FETCH_AND_NAND(p, v);
#endif
}
unsigned int add_and_fetch_32 (unsigned int *p, unsigned int v) {
	return __sync_add_and_fetch (p, v);
}
unsigned int sub_and_fetch_32 (unsigned int *p, unsigned int v) {
	return __sync_sub_and_fetch (p, v);
}
unsigned int or_and_fetch_32 (unsigned int *p, unsigned int v) {
	return __sync_or_and_fetch (p, v);
}
unsigned int and_and_fetch_32 (unsigned int *p, unsigned int v) {
	return __sync_and_and_fetch (p, v);
}
unsigned int xor_and_fetch_32 (unsigned int *p, unsigned int v) {
	return __sync_xor_and_fetch (p, v);
}
unsigned int nand_and_fetch_32 (unsigned int *p, unsigned int v) {
#if USE_SYNC_NAND_AND_FETCH
	return __sync_nand_and_fetch (p, v);
#else
	CAS_NAND_AND_FETCH(p, v);
#endif
}
unsigned int
bool_compare_and_swap_32 (unsigned int *p, unsigned int old, unsigned int new) {
	return __sync_bool_compare_and_swap (p, old, new);
}
unsigned int
val_compare_and_swap_32 (unsigned int *p, unsigned int old, unsigned int new) {
	return __sync_val_compare_and_swap (p, old, new);
}
unsigned int lock_test_and_set_32 (unsigned int *p) {
	// Only immediate 0/1 appear to be widely supported, so hardcode it
	// here
	return __sync_lock_test_and_set (p, 1);
}
void lock_release_32 (unsigned int *p) {
	// Writes a 0 to *p
	return __sync_lock_release (p);
}

/* 64-bit */
unsigned long long fetch_and_add_64 (unsigned long long *p, unsigned long long v) {
	return __sync_fetch_and_add (p, v);
}
unsigned long long fetch_and_sub_64 (unsigned long long *p, unsigned long long v) {
	return __sync_fetch_and_sub (p, v);
}
unsigned long long fetch_and_or_64 (unsigned long long *p, unsigned long long v) {
	return __sync_fetch_and_or (p, v);
}
unsigned long long fetch_and_and_64 (unsigned long long *p, unsigned long long v) {
	return __sync_fetch_and_and (p, v);
}
unsigned long long fetch_and_xor_64 (unsigned long long *p, unsigned long long v) {
	return __sync_fetch_and_xor (p, v);
}
unsigned long long fetch_and_nand_64 (unsigned long long *p, unsigned long long v) {
#if USE_SYNC_FETCH_AND_NAND
  return __sync_fetch_and_nand(p, v);
#else
  CAS_FETCH_AND_NAND(p, v);
#endif
}
unsigned long long add_and_fetch_64 (unsigned long long *p, unsigned long long v) {
	return __sync_add_and_fetch (p, v);
}
unsigned long long sub_and_fetch_64 (unsigned long long *p, unsigned long long v) {
	return __sync_sub_and_fetch (p, v);
}
unsigned long long or_and_fetch_64 (unsigned long long *p, unsigned long long v) {
	return __sync_or_and_fetch (p, v);
}
unsigned long long and_and_fetch_64 (unsigned long long *p, unsigned long long v) {
	return __sync_and_and_fetch (p, v);
}
unsigned long long xor_and_fetch_64 (unsigned long long *p, unsigned long long v) {
	return __sync_xor_and_fetch (p, v);
}
unsigned long long nand_and_fetch_64 (unsigned long long *p, unsigned long long v) {
#if USE_SYNC_NAND_AND_FETCH
	return __sync_nand_and_fetch (p, v);
#else
	CAS_NAND_AND_FETCH(p, v);
#endif
}
unsigned int
bool_compare_and_swap_64 (unsigned long long *p, unsigned long long old, unsigned long long new) {
	return __sync_bool_compare_and_swap (p, old, new);
}
unsigned long long
val_compare_and_swap_64 (unsigned long long *p, unsigned long long old, unsigned long long new) {
	return __sync_val_compare_and_swap (p, old, new);
}
unsigned long long lock_test_and_set_64 (unsigned long long *p) {
	// Only immediate 0/1 appear to be widely supported, so hardcode it
	// here
	return __sync_lock_test_and_set (p, 1);
}
void lock_release_64 (unsigned long long *p) {
	// Writes a 0 to *p
	return __sync_lock_release (p);
}

/* Word-sized */
unsigned long fetch_and_add_word (unsigned long *p, unsigned long v) {
	return __sync_fetch_and_add (p, v);
}
unsigned long fetch_and_sub_word (unsigned long *p, unsigned long v) {
	return __sync_fetch_and_sub (p, v);
}
unsigned long fetch_and_or_word (unsigned long *p, unsigned long v) {
	return __sync_fetch_and_or (p, v);
}
unsigned long fetch_and_and_word (unsigned long *p, unsigned long v) {
	return __sync_fetch_and_and (p, v);
}
unsigned long fetch_and_xor_word (unsigned long *p, unsigned long v) {
	return __sync_fetch_and_xor (p, v);
}
unsigned long fetch_and_nand_word (unsigned long *p, unsigned long v) {
#if USE_SYNC_FETCH_AND_NAND
	return __sync_fetch_and_nand(p, v);
#else
	CAS_FETCH_AND_NAND(p, v);
#endif
}
unsigned long add_and_fetch_word (unsigned long *p, unsigned long v) {
	return __sync_add_and_fetch (p, v);
}
unsigned long sub_and_fetch_word (unsigned long *p, unsigned long v) {
	return __sync_sub_and_fetch (p, v);
}
unsigned long or_and_fetch_word (unsigned long *p, unsigned long v) {
	return __sync_or_and_fetch (p, v);
}
unsigned long and_and_fetch_word (unsigned long *p, unsigned long v) {
	return __sync_and_and_fetch (p, v);
}
unsigned long xor_and_fetch_word (unsigned long *p, unsigned long v) {
	return __sync_xor_and_fetch (p, v);
}
unsigned long nand_and_fetch_word (unsigned long *p, unsigned long v) {
#if USE_SYNC_NAND_AND_FETCH
	return __sync_nand_and_fetch (p, v);
#else
	CAS_NAND_AND_FETCH(p, v);
#endif
}
unsigned int
bool_compare_and_swap_word (unsigned long *p, unsigned long old, unsigned long new) {
	return __sync_bool_compare_and_swap (p, old, new);
}
unsigned long
val_compare_and_swap_word (unsigned long *p, unsigned long old, unsigned long new) {
	return __sync_val_compare_and_swap (p, old, new);
}
unsigned long lock_test_and_set_word (unsigned long *p) {
	// Only immediate 0/1 appear to be widely supported, so hardcode it
	// here
	return __sync_lock_test_and_set (p, 1);
}
void lock_release_word (unsigned long *p) {
	// Writes a 0 to *p
	return __sync_lock_release (p);
}
#pragma GCC diagnostic pop
