extern "C"
{
#define BASE_UNITY
#include "base/base_include.h"
#undef BASE_UNITY
}

#include "libres/resources.h"
#include "libres/resources.cc"

/*
 * Test case:
 * test if the retrieved disks aren't 0
 *
 * */
internal void
test_partition_count()
{
    mem_arena *arena = arena_create(MiB(8));

    disk *disk = disk_create(arena);
    disk_read(disk, arena);

    test((disk->part_count != 0) && "Failed to retrieve partition count");
}

/*
 * Test case:
 * test if the partitions get allocated
 * */
internal void
test_partition_creation()
{
    mem_arena *arena = arena_create(MiB(8));

    disk *disk = disk_create(arena);
    disk_read(disk, arena);

    test((disk->partitions != NULL));
}

/*
 * Test case:
 * test if the partition capacity is bigger then the partition count
 * */
internal void
test_partition_capacity_bigger_then_count()
{
    mem_arena *arena = arena_create(MiB(8));

    disk *disk = disk_create(arena);
    disk_read(disk, arena);

    test((disk->part_capacity >= disk->part_count));
}

/*
 * Test case:
 * test if the partition capacity is bigger then the partition count
 * */
internal void
test_individual_partition_non_null()
{
    mem_arena *arena = arena_create(MiB(8));

    disk *disk = disk_create(arena);
    disk_read(disk, arena);

    for (
    size_t parition_index = 0;
    parition_index < disk->part_count;
    ++parition_index)
    {
        /*
     * TODO(nasr): iterate over partitions and check if not null
     * */
    }
}

int
main()
{
    test_partition_count();
    test_partition_creation();
    test_partition_capacity_bigger_then_count();
}
