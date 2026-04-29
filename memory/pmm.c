#include "memory/pmm.h"

#define BITMAP_INDEX(bit)   ((bit) / 32)
#define BITMAP_OFFSET(bit)  ((bit) % 32)

static u32_t* bitmap = 0;

static u32_t total_pages = 0;
static u32_t used_pages = 0;

static u32_t bitmap_start = 0;
static u32_t bitmap_size = 0;
static u32_t first_free_page = 0;

static u32_t align_up(u32_t value, u32_t align)
{
    return (value + align - 1) & ~(align - 1);
}

static void bitmap_set(u32_t bit)
{
    bitmap[BITMAP_INDEX(bit)] |= (1 << BITMAP_OFFSET(bit));
}

static void bitmap_clear(u32_t bit)
{
    bitmap[BITMAP_INDEX(bit)] &= ~(1 << BITMAP_OFFSET(bit));
}

static int bitmap_test(u32_t bit)
{
    return bitmap[BITMAP_INDEX(bit)] & (1 << BITMAP_OFFSET(bit));
}

void pmm_init(u32_t memory_size, u32_t kernel_end)
{
    total_pages = memory_size / PMM_PAGE_SIZE;
    used_pages = total_pages;

    bitmap_start = align_up(kernel_end, PMM_PAGE_SIZE);

    /*
     * page 1개당 bitmap 1bit 필요.
     * total_pages / 8 bytes.
     */
    bitmap_size = align_up(total_pages / 8, PMM_PAGE_SIZE);

    bitmap = (u32_t*)bitmap_start;

    /*
     * 처음에는 모든 페이지를 used로 처리.
     */
    for (u32_t i = 0; i < bitmap_size / 4; i++) {
        bitmap[i] = 0xFFFFFFFF;
    }

    /*
     * bitmap 뒤쪽부터 실제 할당 가능 영역으로 사용.
     */
    u32_t free_start_addr = align_up(bitmap_start + bitmap_size, PMM_PAGE_SIZE);
    first_free_page = free_start_addr / PMM_PAGE_SIZE;

    for (u32_t page = first_free_page; page < total_pages; page++) {
        bitmap_clear(page);
        used_pages--;
    }
}

u32_t pmm_alloc_page(void)
{
    for (u32_t page = first_free_page; page < total_pages; page++) {
        if (!bitmap_test(page)) {
            bitmap_set(page);
            used_pages++;
            return page * PMM_PAGE_SIZE;
        }
    }

    return 0;
}

void pmm_free_page(u32_t addr)
{
    if (addr == 0) {
        return;
    }

    if (addr % PMM_PAGE_SIZE != 0) {
        return;
    }

    u32_t page = addr / PMM_PAGE_SIZE;

    if (page < first_free_page || page >= total_pages) {
        return;
    }

    if (bitmap_test(page)) {
        bitmap_clear(page);
        used_pages--;
    }
}

u32_t pmm_get_total_pages(void)
{
    return total_pages;
}

u32_t pmm_get_used_pages(void)
{
    return used_pages;
}

u32_t pmm_get_free_pages(void)
{
    return total_pages - used_pages;
}