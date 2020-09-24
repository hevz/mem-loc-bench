#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

typedef int (*Func) (void);

static Func
gen_func (void *addr)
{
    extern int func_templ (void);
    extern void icache_sync (void);
    const size_t page_size = 16384;
    const size_t page_mask = page_size - 1;
    void *base = (void *)((uintptr_t)addr & ~page_mask);

    base = mmap (base, 2 * page_size, PROT_READ | PROT_WRITE | PROT_EXEC,
                 MAP_FIXED | MAP_PRIVATE | MAP_ANON, -1, 0);
    if (base == MAP_FAILED)
        return NULL;

    memcpy (addr, func_templ, 0x1000);
    icache_sync ();

    return (Func)addr;
}

static size_t
rdtsc (void)
{
    size_t cycles;

    asm volatile("rdhwr %[cycles], $30 \n\t" : [cycles] "=r"(cycles));

    return cycles;
}

int
main (int argc, char *argv[])
{
    Func func;
    size_t c1, c2;
    void *addr;
    int i;

    if (argc < 2) {
        fprintf (stderr, "%s ADDR\n", argv[0]);
        return 0;
    }

    addr = (void *)strtoul (argv[1], NULL, 16);
    func = gen_func (addr);
    printf ("func: %p\n", func);

    c1 = rdtsc ();
    for (i = 0; i < 1000000000; i++)
        func ();
    c2 = rdtsc ();

    printf ("cycles: %zd\n", c2 - c1);

    return 0;
}
