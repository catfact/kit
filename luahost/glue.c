
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <string.h>
#include "glue.h"

#define RETURN_BUF_SIZE (1024 * 128)
char ret_buf[RETURN_BUF_SIZE];

#define CODE_BUF_SIZE (1024 * 128)
char code_buf[CODE_BUF_SIZE];

// the interpreter state
lua_State* lua;

void l_init(void) {
  lua = luaL_newstate();
  luaL_openlibs(lua);
  lua_pcall(lua, 0, 0, 0);
}

/* int l_run_code(const char* code) { */
/*   int top = lua_gettop(lua); */
/*   int err = luaL_loadstring(lua, code); // compile code, push to stack */
/*   err |= lua_pcall(lua, 0, LUA_MULTRET, 0); // pop stack and evaluate */
/*   z = lua_gettop(lua) - top; */
/*   return err; */
/* } */

/// arg..
//#define LUA_CODE_BUF_SIZE 125
//char code_buf[LUA_CODE_BUF_SIZE];

void l_run_code(const char* code) {
  //  int top = lua_gettop(lua);
  //  sprintf(code_buf, "return ");
  //  strncat(code_buf, code, LUA_CODE_BUF_SIZE - strlen("return "));

  /*
  luaL_loadstring(lua, code); // compile code, push to stack
  lua_pcall(lua, 0, LUA_MULTRET, 0); // pop stack and evaluate
  */

  // 
  snprintf(code_buf, CODE_BUF_SIZE, "function exec_tmp() return %s end", code);
  printf("%s\r\n",code_buf);
  luaL_loadstring(lua, code);
  lua_setglobal(lua, "exec_tmp");
  lua_pcall(lua, 0, 0, 0);
  lua_getglobal(lua, "exec_tmp");
  lua_pcall(lua, 0, LUA_MULTRET, 0);
  
  //// woff??
  char* p = ret_buf;
  int l = RETURN_BUF_SIZE - 1;
  int n = 0;

  int z = lua_gettop(lua);
  
  printf("lua call finished with %d items on stack \r\n", z);
	for(int i=0; i<z; i++) {
	  n = snprintf(p, l, "%s", lua_tostring(lua, i));
	l -= n;
	p += n;
  }
  *p='\0';
  
}


/* const char* l_print_stack(void) { */
/*   char* p = ret_buf; */
/*   int l = RETURN_BUF_SIZE - 1; */
/*   int n = 0; */
/*   //  int d = lua_gettop(lua); */
/*   printf("lua call finished with %d items on stack \r\n", z); */
/*   //	n = snprintf(p, l, "%s", lua_tostring(lua, i-z)); */
/*   //  for(int i=0; i<d; i++) { */
/* 	for(int i=0; i<z; i++) { */
/* 	  n = snprintf(p, l, "%s", lua_tostring(lua, i-z)); */
/* 	l -= n; */
/* 	p += n; */
/*   } */
/*   *p='\0'; */
/*   return (const char*)ret_buf; */
/* } */


const char* l_get_stack_buf(void) {
  return (const char*)ret_buf;
}
