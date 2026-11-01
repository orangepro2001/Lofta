/* Publishes core_api.h to scripts as the global table `core`. */
#ifndef LOFTA_LUA_BIND_H
#define LOFTA_LUA_BIND_H

#include <lua.h>

void lua_bind_core(lua_State *L);

#endif
