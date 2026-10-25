/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	zassert_ok(rb_push(1), "Push must succeed");
	rb_init(4);
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_zero_capacity)
{
	zassert_equal(rb_init(0), -EINVAL, "Zero capacity must return -EINVAL");
}

ZTEST(ring_buf_init, test_max_capacity)
{
	zassert_equal(rb_init(RING_BUF_MAX_CAPACITY+1), -EINVAL, "Higher than max capacity must return -EINVAL");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
    int pop;
	zassert_ok(rb_push(42), "Push must succeed");
    zassert_ok(rb_pop(&pop), "Pop must succeed");
    zassert_equal(pop, 42);
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
    int pop;
	zassert_ok(rb_push(1), "Push must succeed");
	zassert_ok(rb_push(2), "Push must succeed");
	zassert_ok(rb_push(3), "Push must succeed");
    zassert_ok(rb_pop(&pop), "Pop must succeed");
    zassert_equal(pop, 1);
    zassert_ok(rb_pop(&pop), "Pop must succeed");
    zassert_equal(pop, 2);
    zassert_ok(rb_pop(&pop), "Pop must succeed");
    zassert_equal(pop, 3);
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	zassert_ok(rb_push(1), "Push must succeed");
	zassert_ok(rb_push(2), "Push must succeed");
	zassert_ok(rb_push(3), "Push must succeed");
	zassert_ok(rb_push(4), "Push must succeed");
	zassert_equal(rb_push(5), -ENOSPC, "Exceed capacity must return -ENOSPC");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
    int peek;
	zassert_ok(rb_push(1), "Push must succeed");
	zassert_equal(rb_count(), 1, "Only 1 pushed");
	zassert_ok(rb_peek(&peek), "Peek must succeed");
	zassert_equal(rb_count(), 1, "Peek must not consume");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	zassert_equal(rb_pop(NULL), -EINVAL, "NULL pointer must return -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	zassert_ok(rb_push(1), "Push must succeed");
	zassert_ok(rb_push(2), "Push must succeed");
	zassert_ok(rb_push(3), "Push must succeed");
	zassert_ok(rb_push(4), "Push must succeed");
	zassert_true(rb_is_full(), "RB must be full");
}
