#pragma once

extern void l_init(void);

extern void l_run_code(const char* code);
//extern int l_run_code(const char* code);
extern const char* l_print_stack(void);

extern const char* l_get_stack_buf(void);
