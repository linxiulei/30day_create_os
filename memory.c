#include "bootpack.h"

unsigned int memtest(unsigned int start, unsigned int end)
{
	char flg486 = 0;
	unsigned int eflg, cr0, i;

	eflg = io_load_eflags();
	eflg |= EFLAGS_AC_BIT; /* AC-bit = 1 */
	io_store_eflags(eflg);
	eflg = io_load_eflags();
	if ((eflg & EFLAGS_AC_BIT) != 0) { 
		flg486 = 1;
	}
	eflg &= ~EFLAGS_AC_BIT; /* AC-bit = 0 */
	io_store_eflags(eflg);

	if (flg486 != 0) {
		cr0 = load_cr0();
		cr0 |= CR0_CACHE_DISABLE; 
		store_cr0(cr0);
	}

	i = memtest_sub(start, end);

	if (flg486 != 0) {
		cr0 = load_cr0();
		cr0 &= ~CR0_CACHE_DISABLE; 
		store_cr0(cr0);
	}

	return i;
}

void memman_init(struct MEMMAN *memman)
{
    memman->frees = 0;
    memman->maxfrees = 0;
    memman->lostsize = 0;
    memman->losts = 0;
    return;
}

unsigned int memman_total(struct MEMMAN *memman)
{
    int total, i;
    total = 0;
    for (i = 0; i < memman->frees; i++)
    {
        total += memman->free[i].size;
    }
    return total;
}

unsigned int memman_alloc(struct MEMMAN *memman, unsigned int size)
{
    int addr, i;
    for (i = 0; i < memman->frees; i++)
    {
        if (memman->free[i].size >= size)
        {
            addr = memman->free[i].addr;
            memman->free[i].addr += size;
            memman->free[i].size -= size;
            if (memman->free[i].size == 0)
            {
                memman->frees--;
                for (;i < memman->frees; i++)
                {
                    memman->free[i] = memman->free[i+1];
                }
            }
            return addr;
        }
    }
    return 0;
}

unsigned int memman_alloc_4k(struct MEMMAN *memman, unsigned int size)
{
    int a;
    size = (size + 0xfff) & 0xfffff000;
    a = memman_alloc(memman, size);
    return a;
}

int memman_free_4k(struct MEMMAN *memman, unsigned int addr, unsigned int size)
{
    int i;
    size = (size + 0xfff) && 0xfffff000;
    i = memman_free(memman, addr, size);
    return i;
}

int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size)
{
	int i, j;
	for (i = 0; i < man->frees; i++) {
		if (man->free[i].addr > addr) {
			break;
		}
	}
	/* free[i - 1].addr < addr < free[i].addr */
	if (i > 0) {
		if (man->free[i - 1].addr + man->free[i - 1].size == addr) {
			man->free[i - 1].size += size;
			if (i < man->frees) {
				if (addr + size == man->free[i].addr) {
					man->free[i - 1].size += man->free[i].size;
					man->frees--;
					for (; i < man->frees; i++) {
						man->free[i] = man->free[i + 1]; 
					}
				}
			}
			return 0;
		}
	}
	if (i < man->frees) {
		if (addr + size == man->free[i].addr) {
			man->free[i].addr = addr;
			man->free[i].size += size;
			return 0; 
		}
	}
	if (man->frees < MEMMAN_FREES) {
		for (j = man->frees; j > i; j--) {
			man->free[j] = man->free[j - 1];
		}
		man->frees++;
		if (man->maxfrees < man->frees) {
			man->maxfrees = man->frees; 
		}
		man->free[i].addr = addr;
		man->free[i].size = size;
		return 0; 
	}
	man->losts++;
	man->lostsize += size;
	return -1; 
}


// vim:ts=4 shiftwidth=4
