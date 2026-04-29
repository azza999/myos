#include "heap.h"
#include "paging.h"
#include "screen.h"

#define KERNEL_HEAP_START 0x00400000

static u32_t heap_start = KERNEL_HEAP_START;
static u32_t heap_end = KERNEL_HEAP_START;
static u32_t heap_cursor = KERNEL_HEAP_START;


void heap_init(void) {
	char buf[0x10] = {0,};
	print("heap_start: ");
	itohex(heap_start,buf);
	print(buf);
	print(" heap_end: ");
	itohex(heap_end,buf);
	print(buf);
	print(" heap_cursor: ");
	itohex(heap_cursor,buf);
	println(buf);


	return;
}

static u32_t align_up(u32_t value, u32_t align)
{
    return (value + align - 1) & ~(align - 1);
}

/*
 * 사이즈를 받아서 커널 확장이 필요한지 확인
 */

u8_t heap_is_extend_needed(u32_t size) {
	return heap_cursor + size > heap_end;
}

u8_t heap_page_extend(void) {
	u32_t new_page = pmm_alloc_page();
	if (new_page == 0) {
		println("!!!NO PAGE ALLOCATED!!!");
	    return 0; // 또는 panic
	}
	paging_map_page(heap_end, new_page, PAGE_PRESENT | PAGE_RW);
	heap_end += PAGE_SIZE;
	return 1;
}

void* kmalloc(u32_t size) {
	void* addr = NULL;
	char buf[30];

	if (size == 0)
	{
		return NULL;
	}

	print("kmalloc - requested size: ");
	itoa(size, buf);
	println(buf);

	u32_t alloc_start = align_up(heap_cursor, 8);
	u32_t alloc_end = alloc_start + align_up(size, 8);

	while (heap_is_extend_needed(size)) {
		print("EXTEND_NEEDED - heap_cursor: ");
		itohex(heap_cursor, buf);
		print(buf);
		print(" heap_end: ");
		itohex(heap_end, buf);
		print(buf);
		print("\n");
    	if (!heap_page_extend()) {
    		return NULL;
    	}
	}

	addr = (void*)alloc_start;
	heap_cursor = alloc_end;

	print("heap cursor updated: ");
	itohex(heap_cursor, buf);
	println(buf);

	print("return address: ");
	itohex(addr, buf);
	println(buf);

	return addr;
}