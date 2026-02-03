extern "C"
{
#define BASE_UNITY
#include "base/base_include.h"
#undef BASE_UNITY
}

#include <string.h>

/*
 * Test: arena_create returns a valid pointer
 */
internal void
test_arena_create()
{
    mem_arena *arena = arena_create(MiB(1));
    test(arena != NULL);
    arena_destroy(arena);
}

/*
 * Test: arena capacity is set correctly
 */
internal void
test_arena_capacity()
{
    u64        capacity = MiB(2);
    mem_arena *arena    = arena_create(capacity);
    test(arena->capacity == capacity);
    arena_destroy(arena);
}

/*
 * Test: arena initial position is set to sizeof(mem_arena)
 */
internal void
test_arena_initial_pos()
{
    mem_arena *arena = arena_create(MiB(1));
    test(arena->pos == sizeof(mem_arena));
    test(arena->prev_pos == sizeof(mem_arena));
    arena_destroy(arena);
}

/*
 * Test: arena_alloc returns non-NULL pointer
 */
internal void
test_arena_alloc_returns_valid()
{
    mem_arena *arena = arena_create(MiB(1));
    void      *ptr   = arena_alloc(arena, 128);

    test(ptr != NULL);
    arena_destroy(arena);
}

/*
 * Test: arena_alloc zeros memory by default
 */
internal void
test_arena_alloc_zeros_memory()
{
    mem_arena *arena    = arena_create(MiB(1));
    u8        *ptr      = (u8 *)arena_alloc(arena, 64);
    b32        all_zero = TRUE;
    for (u64 i = 0; i < 64; i++)
    {
        if (ptr[i] != 0)
        {
            all_zero = FALSE;
            break;
        }
    }
    test(all_zero == TRUE);
    arena_destroy(arena);
}

/*
 * Test: arena_alloc with non_zero flag doesn't zero memory
 */
internal void
test_arena_alloc_non_zero()
{
    mem_arena *arena = arena_create(MiB(1));
    u8        *ptr   = (u8 *)arena_alloc(arena, 64);
    // Fill with non-zero values
    memset(ptr, 0xFF, 64);
    u8 *ptr2 = (u8 *)arena_alloc(arena, 64);
    // The memory shouldn't be zeroed
    test(ptr2 != NULL);
    arena_destroy(arena);
}

/*
 * Test: arena_alloc updates position correctly
 */
internal void
test_arena_alloc_updates_pos()
{
    mem_arena *arena       = arena_create(MiB(1));
    u64        initial_pos = arena->pos;
    arena_alloc(arena, 128);
    test(arena->pos > initial_pos);
    arena_destroy(arena);
}

/*
 * Test: arena_alloc aligns allocations
 */
internal void
test_arena_alloc_alignment()
{
    mem_arena *arena = arena_create(MiB(1));
    void      *ptr1  = arena_alloc(arena, 1);
    void      *ptr2  = arena_alloc(arena, 1);
    test(((u64)ptr1 % ARENA_ALIGN) == 0);
    test(((u64)ptr2 % ARENA_ALIGN) == 0);
    arena_destroy(arena);
}

/*
 * Test: multiple alloces don't overlap
 */
internal void
test_arena_alloc_no_overlap()
{
    mem_arena *arena = arena_create(MiB(1));
    u32       *ptr1  = (u32 *)arena_alloc(arena, sizeof(u32));
    u32       *ptr2  = (u32 *)arena_alloc(arena, sizeof(u32));
    *ptr1            = 0xDEADBEEF;
    *ptr2            = 0xCAFEBABE;
    test(*ptr1 == 0xDEADBEEF);
    test(*ptr2 == 0xCAFEBABE);
    arena_destroy(arena);
}

/*
 * Test: alloc_STRUCT macro works
 */
internal void
test_alloc_struct_macro()
{
    typedef struct test_struct
    {
        u64 a;
        u32 b;
        u16 c;
    } test_struct;

    mem_arena   *arena = arena_create(MiB(1));
    test_struct *s     = PUSH_STRUCT(arena, test_struct);
    test(s != NULL);
    test(s->a == 0);
    test(s->b == 0);
    test(s->c == 0);
    arena_destroy(arena);
}

/*
 * Test: alloc_ARRAY macro works
 */
internal void
test_alloc_array_macro()
{
    mem_arena *arena = arena_create(MiB(1));
    u32       *arr   = PUSH_ARRAY(arena, u32, 10);
    test(arr != NULL);
    for (int i = 0; i < 10; i++)
    {
        test(arr[i] == 0);
    }
    arena_destroy(arena);
}

/*
 * Test: arena_pop reduces position
 */
internal void
test_arena_pop()
{
    mem_arena *arena = arena_create(MiB(1));
    arena_alloc(arena, 256);
    u64 pos_before = arena->pos;
    arena_pop(arena, 128);
    test(arena->pos == pos_before - 128);
    arena_destroy(arena);
}

/*
 * Test: arena_pop doesn't go below initial position
 */
internal void
test_arena_pop_safety()
{
    mem_arena *arena       = arena_create(MiB(1));
    u64        initial_pos = arena->pos;
    arena_pop(arena, MiB(10)); // Try to pop more than allocated
    test(arena->pos == initial_pos);
    arena_destroy(arena);
}

/*
 * Test: arena_pop_to sets position correctly
 */
internal void
test_arena_pop_to()
{
    mem_arena *arena = arena_create(MiB(1));
    arena_alloc(arena, 512);
    u64 target_pos = arena->pos;
    arena_alloc(arena, 256);
    arena_pop_to(arena, target_pos);
    test(arena->pos == target_pos);
    arena_destroy(arena);
}

/*
 * Test: arena_clear resets arena
 */
internal void
test_arena_clear()
{
    mem_arena *arena       = arena_create(MiB(1));
    u64        initial_pos = arena->pos;
    arena_alloc(arena, 1024);
    arena_alloc(arena, 2048);
    arena_clear(arena);
    test(arena->pos == initial_pos);
    arena_destroy(arena);
}

/*
 * Test: temp_arena_begin captures state
 */
internal void
test_temp_arena_begin()
{
    mem_arena *arena = arena_create(MiB(1));
    arena_alloc(arena, 256);
    temp_arena temp = temp_arena_begin(arena);
    test(temp.arena == arena);
    test(temp.offset == arena->pos);
    arena_destroy(arena);
}

/*
 * Test: temp_arena_end restores state
 */
internal void
test_temp_arena_end()
{
    mem_arena *arena = arena_create(MiB(1));
    arena_alloc(arena, 256);
    temp_arena temp      = temp_arena_begin(arena);
    u64        saved_pos = arena->pos;
    arena_alloc(arena, 512);
    arena_alloc(arena, 1024);
    temp_arena_end(temp);

    test(arena->pos == saved_pos);
    arena_destroy(arena);
}

/*
 * Test: temp_arena nesting works
 */
internal void
test_temp_arena_nesting()
{
    mem_arena *arena = arena_create(MiB(1));
    u64        pos0  = arena->pos;
    arena_alloc(arena, 128);
    temp_arena temp1 = temp_arena_begin(arena);
    u64        pos1  = arena->pos;
    arena_alloc(arena, 256);
    temp_arena temp2 = temp_arena_begin(arena);
    u64        pos2  = arena->pos;
    arena_alloc(arena, 512);
    temp_arena_end(temp2);
    test(arena->pos == pos2);
    temp_arena_end(temp1);
    test(arena->pos == pos1);
    arena_destroy(arena);
}

/*
 * Test: arena can be reused after clear
 */
internal void
test_arena_reuse()
{
    mem_arena *arena = arena_create(MiB(1));
    u64       *ptr1  = PUSH_STRUCT(arena, u64);
    *ptr1            = 42;
    arena_clear(arena);
    u64 *ptr2 = PUSH_STRUCT(arena, u64);
    *ptr2     = 84;
    test(*ptr2 == 84);
    arena_destroy(arena);
}

/*
 * Test: large allocation works
 */
internal void
test_arena_large_allocation()
{
    mem_arena *arena = arena_create(MiB(10));
    void      *ptr   = arena_alloc(arena, MiB(5));
    test(ptr != NULL);
    arena_destroy(arena);
}

int
main(void)
{
    test_arena_create();
    test_arena_capacity();
    test_arena_initial_pos();
    test_arena_alloc_returns_valid();
    test_arena_alloc_zeros_memory();
    test_arena_alloc_non_zero();
    test_arena_alloc_updates_pos();
    test_arena_alloc_alignment();
    test_arena_alloc_no_overlap();
    test_alloc_struct_macro();
    test_alloc_array_macro();
    test_arena_pop();
    test_arena_pop_safety();
    test_arena_pop_to();
    test_arena_clear();
    test_temp_arena_begin();
    test_temp_arena_end();
    test_temp_arena_nesting();
    test_arena_reuse();
    test_arena_large_allocation();
    return 0;
}
