#pragma once
#include <lua.h>
extern lua_State* l_state(void);

extern void l_init(void);
extern void l_run_code(const char* code);
extern void l_print_stack_dump(void);

extern void l_run_table_test(void);

