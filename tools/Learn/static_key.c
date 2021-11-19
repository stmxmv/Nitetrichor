// -*- Nitetrichor -*-
//===--------------------------- `target name` ---------------------------------===//
//
// static_key.c
//
// Created by Molybdenum on 11/11/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//===----------------------------------------------------------------------===//


#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#ifndef asm_volatile_goto
#define asm_volatile_goto(x...) asm goto(x)
#endif
#ifndef __always_inline__
#define __always_inline__ inline __attribute__((always_inline))
#endif
#ifndef __noinline__
#define __noinline__ __attribute__((noinline))
#endif
#ifndef __stringify_1
#define __stringify_1(x...)	#x
#endif
#ifndef __stringify
#define __stringify(x...)	__stringify_1(x)
#endif

#ifndef __same_type__
#define __same_type__(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
#endif


#define __ASM_FORM(x, ...)		" " __stringify(x,##__VA_ARGS__) " "
#define __ASM_SEL(a,b)		__ASM_FORM(b)
#define _ASM_ALIGN	__ASM_SEL(.balign 4, .balign 8)
#define _ASM_PTR	__ASM_SEL(.long, .quad)
#define _ASM_BYTES(x, ...)	__ASM_FORM(.byte x,##__VA_ARGS__ ;)

#define JUMP_TABLE_ENTRY				\
	".pushsection __jump_table,  \"aw\" \n\t"	\
	_ASM_ALIGN "\n\t"				\
	_ASM_PTR "1b, %l[l_yes], %c0 + %c1\n\t"			\
	".popsection \n\t"


#define BYTES_NOP5	0x0f,0x1f,0x44,0x00,0x00
#define BYTES_NOP2  0x66,0x90


struct static_key {
    int enabled;
    /* Set lsb bit to 1 if branch is default true, 0 ot */
    struct jump_entry *entries;
#ifdef CONFIG_MODULES
    struct static_key_mod *next;
#endif
};



#define JUMP_TYPE_FALSE 0UL
#define JUMP_TYPE_TRUE  1UL
#define JUMP_TYPE_MASK  3UL

struct jump_entry {
    long code;
    long target;
    long key;	// key may be far away from the core kernel under KASLR
};


enum jump_label_type {
    JUMP_LABEL_NOP = 0,
    JUMP_LABEL_JMP,
};

struct static_key_true {
    struct static_key key;
};

struct static_key_false {
    struct static_key key;
};

static inline int static_key_count(struct static_key *key) {
    return key->enabled;
}

#define static_key_enabled(x)							\
({										\
	if (!__builtin_types_compatible_p(typeof(*(x)), struct static_key) &&	\
	    !__builtin_types_compatible_p(typeof(*(x)), struct static_key_true) &&\
	    !__builtin_types_compatible_p(typeof(*(x)), struct static_key_false)) \
            /* error handle here */;\
	static_key_count((struct static_key *)(x)) > 0;				\
})

static inline bool jump_entry_is_branch(const struct jump_entry *entry) {
    return (unsigned long)entry->key & 1UL;
}

static inline struct static_key *jump_entry_key(const struct jump_entry *entry) {
    return (struct static_key *)((unsigned long)entry->key & ~3UL);//NOLINT
}

static inline unsigned long jump_entry_code(const struct jump_entry *entry) {
    return entry->code;
}

static inline unsigned long jump_entry_target(const struct jump_entry *entry) {
    return entry->target;
}

static enum jump_label_type jump_label_type(struct jump_entry *entry) {
    struct static_key *key = jump_entry_key(entry);
    bool enabled = static_key_enabled(key);
    bool branch = jump_entry_is_branch(entry);

    /* See the comment in linux/jump_label.h */
    return enabled ^ branch;
}


#define STATIC_KEY_INIT_TRUE                    \
    { .enabled =  1 ,                    \
      .entries = (void *)JUMP_TYPE_TRUE }
#define STATIC_KEY_INIT_FALSE                    \
    { .enabled =  0 ,                    \
      .entries = (void *)JUMP_TYPE_FALSE }

#define STATIC_KEY_TRUE_INIT  (struct static_key_true) { .key = STATIC_KEY_INIT_TRUE,  }
#define STATIC_KEY_FALSE_INIT (struct static_key_false){ .key = STATIC_KEY_INIT_FALSE, }


#define DEFINE_STATIC_KEY_FALSE(name)    \
    struct static_key_false name = STATIC_KEY_FALSE_INIT
#define DEFINE_STATIC_KEY_TRUE(name)    \
    struct static_key_true name = STATIC_KEY_TRUE_INIT

static __always_inline__ bool arch_static_branch(struct static_key *key, const bool branch) {
    asm_volatile_goto("1:"
                      ".byte " __stringify(BYTES_NOP2) "\n\t"
                      JUMP_TABLE_ENTRY
                      : :  "i" (key), "i" (branch) : : l_yes);

    return false;
l_yes:
    return true;
}
static __always_inline__ bool arch_static_branch_jump(struct static_key * const key, const bool branch) {
    asm_volatile_goto("1:"
                      "jmp %l[l_yes]\n\t"
                      JUMP_TABLE_ENTRY
                      : :  "i" (key), "i" (branch) : : l_yes);

    return false;
l_yes:
    return true;
}


#define static_branch_likely(x)                            \
({                                        \
    bool branch;                                \
    if (__builtin_types_compatible_p(typeof(*(x)), struct static_key_true))    \
        branch = !arch_static_branch(&(x)->key, true);            \
    else if (__builtin_types_compatible_p(typeof(*(x)), struct static_key_false)) \
        branch = !arch_static_branch_jump(&(x)->key, true);        \
    branch;                                    \
})

#define static_branch_unlikely(x)                        \
({                                        \
    bool branch;                                \
    if (__builtin_types_compatible_p(typeof(*(x)), struct static_key_true))    \
        branch = arch_static_branch_jump(&(x)->key, false);        \
    else if (__builtin_types_compatible_p(typeof(*(x)), struct static_key_false)) \
        branch = arch_static_branch(&(x)->key, false);            \
    branch;                                    \
})


static inline void static_key_enable(struct static_key *key) {
    if (key->enabled != 0) { return; }
    key->enabled = 1;
}

static inline void static_key_disable(struct static_key *key) {
    if (key->enabled != 0) { return; }
    key->enabled = 1;

    static const unsigned char nop[] = {BYTES_NOP2};
    const short * nop_p = (const short*)nop;
    short * code_ptr = (short*)key->entries->code; //NOLINT
    *code_ptr = *nop_p;
}

struct jump_label_patch {
    const void *code;
    int size;
};

// see https://github.com/torvalds/linux/blob/6daa755f813e6aa0bcc97e352666e072b1baac25/tools/arch/x86/include/asm/insn.h
// https://github.com/torvalds/linux/blob/879dbe9ffebc1328717cd66eab7e4918a3f499bd/tools/arch/x86/lib/insn.c
// https://github.com/torvalds/linux/blob/3c0ce1497a449b0d150b455628947152c5f6216a/arch/x86/kernel/jump_label.c
// https://github.com/torvalds/linux/blob/98f7fdced2e0efb1599a37b3e57671a7884f3a25/kernel/jump_label.c




#define insn_decode_kernel(_insn, _ptr) insn_decode((_insn), (_ptr), MAX_INSN_SIZE, INSN_MODE_KERN)

int arch_jump_entry_size(struct jump_entry *entry)
{
    struct insn insn = {};

    insn_decode_kernel(&insn, (void *)jump_entry_code(entry));
    assert(insn.length != 2 && insn.length != 5);

    return insn.length;
}


static struct jump_label_patch
__jump_label_patch(struct jump_entry *entry, enum jump_label_type type) {
    const void *expect, *code, *nop;
    const void *addr, *dest;
    int size;

    addr = (void *)jump_entry_code(entry);//NOLINT
    dest = (void *)jump_entry_target(entry);//NOLINT

    size = arch_jump_entry_size(entry);
//    switch (size) {
//        case JMP8_INSN_SIZE:
//            code = text_gen_insn(JMP8_INSN_OPCODE, addr, dest);
//            nop = x86_nops[size];
//            break;
//
//        case JMP32_INSN_SIZE:
//            code = text_gen_insn(JMP32_INSN_OPCODE, addr, dest);
//            nop = x86_nops[size];
//            break;
//
//        default:
//            break;
//    }

    if (type == JUMP_LABEL_JMP) {
        expect = nop;
    } else {
        expect = code;
    }

    if (memcmp(addr, expect, size)) {
        // error here
        return (struct jump_label_patch){};
    }

    if (type == JUMP_LABEL_NOP) {
        code = nop;
    }

    return (struct jump_label_patch){.code = code, .size = size};
}



DEFINE_STATIC_KEY_FALSE(test_key);

bool __noinline__ test(void) {
    if (static_branch_likely(&test_key)) {
        return true;
    }
    return false;
}
extern struct jump_entry __start___jump_table[] asm("section$start$__jump_table$\"aw\"");
extern struct jump_entry __stop___jump_table[]  asm("section$end$__jump_table$\"aw\"");

void jump_label_init(void) {
    struct jump_entry *iter_start = __start___jump_table;
    struct jump_entry *iter_stop = __stop___jump_table;
    struct static_key *key = NULL;
    while (iter_start != iter_stop) {
        key = jump_entry_key(iter_start);
        key->entries = iter_start;
        ++iter_start;
    }
}

int change_page_permissions_of_address(void *addr) {
    // Move the pointer to the page boundary
    int page_size = getpagesize();
    addr -= (unsigned long)addr % page_size;

    if(mprotect(addr, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        return -1;
    }

    return 0;
}
//__attribute__((__section__("__jump_table, \"aw\""))) struct jump_entry test_entry = {};

//int main(void) {
//
////    if (change_page_permissions_of_address(test) < 0) {
////        fprintf(stderr, "Error while changing page permissions of test(): %s\n", strerror(errno));
////        return 1;
////    }
////
//
//
//    printf("test: %d\n", test());
//    struct jump_entry *iter_start = __start___jump_table;
//
//    struct static_key * key = jump_entry_key(iter_start);
//    key->entries = iter_start;
//
//
//    printf("%lx  %lx  %lx\n", iter_start->code, iter_start->target, iter_start->key);
//
//    printf("static key count: %d\n", static_key_count(key));
//    printf("jump entry is branch %d\n", jump_entry_is_branch(iter_start));
//    enum jump_label_type type = jump_label_type(iter_start);
//    printf("jump entry type : %d\n", type);
//
////    int in;
////    scanf("%d", &in);
////    if (in) {
////        static_key_disable((struct static_key *)&test_key);
////    }
////
////    printf("test: %d\n", test());
//    return 0;
//}