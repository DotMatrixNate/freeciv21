/* tolua: event functions
** Support code for Lua bindings.
** Written by Waldemar Celes
** TeCGraf/PUC-Rio
** Apr 2003
** $Id: tolua_event.c,v 1.7 2011/01/13 13:43:46 fabraham Exp $
*/

/* This code is free software; you can redistribute it and/or modify it. 
** The software provided hereunder is on an "as is" basis, and 
** the author has no obligation to provide maintenance, support, updates,
** enhancements, or modifications. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tolua.h"

#include "tolua_event.h"

/* Store at peer
	* It stores, creating the corresponding table if needed,
	* the pair key/value in the corresponding peer table
*/
static void storeatpeer (lua_State* L, int index)
{
	 /* stack: key value (to be stored) */
		lua_pushstring(L,"tolua_peer");
		lua_rawget(L,LUA_REGISTRYINDEX);        /* stack: k v peer */
    lua_pushvalue(L, index);
		lua_rawget(L,-2);                       /* stack: k v peer peer[u] */
		if (!lua_istable(L,-1))
		{
			lua_pop(L,1);                          /* stack: k v peer */
			lua_newtable(L);                       /* stack: k v peer table */
			lua_pushvalue(L,index);
			lua_pushvalue(L,-2);                   /* stack: k v peer table u table */
			lua_settable(L,-4);                    /* stack: k v peer peer[u]=table */
		}
		lua_insert(L,-4);                       /* put table before k */
		lua_pop(L,1);                           /* pop peer */
		lua_rawset(L,-3);                       /* store at table */
		lua_pop(L,1);                           /* pop peer[u] */
}

/* Module index function
*/
static int module_index_event (lua_State* L)
{
	lua_pushstring(L,".get");
	lua_rawget(L,-3);
	if (lua_istable(L,-1))
	{
		lua_pushvalue(L,2);  /* key */
		lua_rawget(L,-2);
		if (lua_iscfunction(L,-1))
		{
			lua_call(L,0,1);
			return 1;
		}
		else if (lua_istable(L,-1))
			return 1;
	}
	/* call old index meta event */
	if (lua_getmetatable(L,1))
	{
		lua_pushstring(L,"__index");
		lua_rawget(L,-2);
		lua_pushvalue(L,1);
		lua_pushvalue(L,2);
		if (lua_isfunction(L,-1))
		{
			lua_call(L,2,1);
			return 1;
		}
		else if (lua_istable(L,-1))
		{
			lua_gettable(L,-3);
			return 1;
		}
	}
	lua_pushnil(L);
	return 1;
}

/* Module newindex function
*/
static int module_newindex_event (lua_State* L)
{
	lua_pushstring(L,".set");
	lua_rawget(L,-4);
	if (lua_istable(L,-1))
	{
		lua_pushvalue(L,2);  /* key */
		lua_rawget(L,-2);
		if (lua_iscfunction(L,-1))
		{
			lua_pushvalue(L,1); /* only to be compatible with non-static vars */
			lua_pushvalue(L,3); /* value */
			lua_call(L,2,0);
			return 0;
		}
	}
	/* call old newindex meta event */
	if (lua_getmetatable(L,1) && lua_getmetatable(L,-1))
	{
		lua_pushstring(L,"__newindex");
		lua_rawget(L,-2);
		if (lua_isfunction(L,-1))
		{
		 lua_pushvalue(L,1);
		 lua_pushvalue(L,2);
		 lua_pushvalue(L,3);
			lua_call(L,3,0);
		}
	}
	lua_settop(L,3);
	lua_rawset(L,-3);
	return 0;
}

/* Class index function
	* If the object is a userdata (ie, an object), it searches the field in 
	* the alternative table stored in the corresponding "peer" table.
*/
static int class_index_event (lua_State* L)
{
  int t = lua_type(L,1);
	if (t == LUA_TUSERDATA)
	{
		/* Access alternative table */
		lua_pushstring(L,"tolua_peer");
		lua_rawget(L,LUA_REGISTRYINDEX);        /* stack: obj key peer */
		lua_pushvalue(L,1);
		lua_rawget(L,-2);                       /* stack: obj key peer peer[u] */
		if (lua_istable(L,-1))
		{
			lua_pushvalue(L,2);  /* key */
			lua_gettable(L,-2);             /* stack: obj key peer peer[u] value */
			if (!lua_isnil(L,-1))
				return 1;
		}
		lua_settop(L,2);                        /* stack: obj key */
		/* Try metatables */
		lua_pushvalue(L,1);                     /* stack: obj key obj */
		while (lua_getmetatable(L,-1))
		{                                       /* stack: obj key obj mt */
			lua_remove(L,-2);                      /* stack: obj key mt */
			if (lua_isnumber(L,2))                 /* check if key is a numeric value */
			{
				/* try operator[] */
				lua_pushstring(L,".geti");    
				lua_rawget(L,-2);                      /* stack: obj key mt func */
				if (lua_isfunction(L,-1))
				{
					lua_pushvalue(L,1);
					lua_pushvalue(L,2);
					lua_call(L,2,1);
					return 1;
				}
      }
			else
			{
			 lua_pushvalue(L,2);                    /* stack: obj key mt key */
				lua_rawget(L,-2);                      /* stack: obj key mt value */
				if (!lua_isnil(L,-1))
					return 1;
				else
					lua_pop(L,1);
				/* try C/C++ variable */
				lua_pushstring(L,".get");    
				lua_rawget(L,-2);                      /* stack: obj key mt tget */
				if (lua_istable(L,-1))
				{
					lua_pushvalue(L,2);
					lua_rawget(L,-2);                      /* stack: obj key mt value */
					if (lua_iscfunction(L,-1))
					{
						lua_pushvalue(L,1);
						lua_pushvalue(L,2); 
						lua_call(L,2,1);
						return 1;
					}
					else if (lua_istable(L,-1))
					{
						/* deal with array: create table to be returned and cache it in peer */
						void* u = *((void**)lua_touserdata(L,1));
						lua_newtable(L);                /* stack: obj key mt value table */
						lua_pushstring(L,".self");
						lua_pushlightuserdata(L,u);
						lua_rawset(L,-3);               /* store usertype in ".self" */
						lua_insert(L,-2);               /* stack: obj key mt table value */
						lua_setmetatable(L,-2);         /* set stored value as metatable */
						lua_pushvalue(L,-1);            /* stack: obj key met table table */
						lua_pushvalue(L,2);             /* stack: obj key mt table table key */
						lua_insert(L,-2);               /*  stack: obj key mt table key table */
						storeatpeer(L,1);               /* stack: obj key mt table */
						return 1;
					}
				}
			}
			lua_settop(L,3);
		}
		lua_pushnil(L);
		return 1;
	}
	else if (t== LUA_TTABLE)
	{
		module_index_event(L);
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

/* Newindex function
	* It first searches for a C/C++ varaible to be set.
	* Then, it either stores it in the alternative peer table (in the case it is
	* an object) or in the own table (that represents the class or module).
*/
static int class_newindex_event (lua_State* L)
{
 int t = lua_type(L,1);
	if (t == LUA_TUSERDATA)
	{
    if (lua_isnumber(L,2))                 /* check if key is a numeric value */
    {
      /* try operator[] */
      lua_pushvalue(L,1);                    /* stack: obj key v obj */
      while (lua_getmetatable(L,-1)) {       /* stack: obj key v obj mt */
        lua_remove(L,-2);                    /* stack: obj key v mt */
        lua_pushstring(L,".seti");    
        lua_rawget(L,-2);                    /* stack: obj key v mt func */
        if (lua_isfunction(L,-1))
        {
          lua_pushvalue(L,1);
          lua_pushvalue(L,2);
          lua_pushvalue(L,3);
          lua_call(L,3,0);
          return 0;
        }
			  lua_settop(L,4);                     /* stack: obj key v mt */
      }
      tolua_error(L,"Attempt to set indexed value on an invalid operand",NULL);
    }
    else {
	    /* Try accessing a C/C++ variable to be set */
      lua_pushvalue(L,1);                    /* stack: obj key v obj */
      while (lua_getmetatable(L,-1)) {       /* stack: obj key v obj mt */
        lua_remove(L,-2);                    /* stack: obj key v mt */
        lua_pushstring(L,".set");
        lua_rawget(L,-2);                    /* stack: t k v mt tset */
        if (lua_istable(L,-1))
        {
          lua_pushvalue(L,2);
          lua_rawget(L,-2);                  /* stack: t k v mt tset func */
          if (lua_iscfunction(L,-1))
          {
            lua_pushvalue(L,1);
            lua_pushvalue(L,3); 
            lua_call(L,2,0);
            return 0;
          }
        }
	      lua_settop(L,4);                          /* stack: t k v mt */
      }
		}
		/* then, store as a new field */
	  lua_settop(L,3);                              /* stack: t k v */
		storeatpeer(L,1);
	}
	else if (t== LUA_TTABLE)
	{
		module_newindex_event(L);
	}
	return 0;
}

static int do_operator (lua_State* L, const char* op)
{
	if (lua_isuserdata(L,1))
	{
		/* Try metatables */
		lua_pushvalue(L,1);                     /* stack: op1 op2 */
		while (lua_getmetatable(L,-1))
		{                                       /* stack: op1 op2 op1 mt */
			lua_remove(L,-2);                      /* stack: op1 op2 mt */
			lua_pushstring(L,op);                  /* stack: op1 op2 mt key */
			lua_rawget(L,-2);                      /* stack: obj key mt func */
			if (lua_isfunction(L,-1))
			{
				lua_pushvalue(L,1);
				lua_pushvalue(L,2); 
				lua_call(L,2,1);
				return 1;
			}
			lua_settop(L,3);
		}
	}
 if (strcmp(op,".eq")==0)
 {
  lua_pushboolean(L,lua_rawequal(L,1,2));
  return 1;
 }
 else
 {
	 tolua_error(L,"Attempt to perform operation on an invalid operand",NULL);
	 return 0;
 }
}

static int class_add_event (lua_State* L)
{
	return do_operator(L,".add");
}

static int class_sub_event (lua_State* L)
{
	return do_operator(L,".sub");
}

static int class_mul_event (lua_State* L)
{
	return do_operator(L,".mul");
}

static int class_div_event (lua_State* L)
{
	return do_operator(L,".div");
}

static int class_lt_event (lua_State* L)
{
	return do_operator(L,".lt");
}

static int class_le_event (lua_State* L)
{
	return do_operator(L,".le");
}

static int class_eq_event (lua_State* L)
{
	return do_operator(L,".eq");
}

static int class_len_event (lua_State* L)
{
	if (lua_isuserdata(L,1))
	{
		/* Try metatables */
		lua_pushvalue(L,1);                     /* stack: op1 op2 */
		while (lua_getmetatable(L,-1))
		{                                       /* stack: op1 op2 op1 mt */
			lua_remove(L,-2);                     /* stack: op1 op2 mt */
			lua_pushstring(L,".len");             /* stack: op1 op2 mt key */
			lua_rawget(L,-2);                     /* stack: obj key mt func */
			if (lua_isfunction(L,-1))
			{
				lua_pushvalue(L,1);
				lua_call(L,1,1);
				return 1;
			}
			lua_settop(L,3);
		}
	}
	tolua_error(L,"Attempt to perform operation on an invalid operand",NULL);
	return 0;
}

static int class_gc_event (lua_State* L)
{
  if (lua_type(L,1) == LUA_TUSERDATA)
  {
    int top = lua_gettop(L);
    void* u = *((void**)lua_touserdata(L,1));
    lua_pushstring(L,"tolua_gc");
    lua_rawget(L,LUA_REGISTRYINDEX);   /* gc */
    lua_pushlightuserdata(L,u);        /* gc u */
    lua_rawget(L,-2);                  /* gc func */
    if (!lua_isnil(L, -1))
    {
      /* remove entry from table */
      lua_pushlightuserdata(L,u);
      lua_pushnil(L);
      lua_rawset(L,-4);
      if (lua_isfunction(L,-1)) {
        /* call collect function */
        lua_pushvalue(L,1);            /* tolua_gc tolua_gc.u(func) u */
        lua_call(L,1,0);               /* tolua_gc */
      }
      else if (lua_isuserdata(L,-1) && *((void**)lua_touserdata(L,-1))==NULL) {
        /* free object */
        free(u);
        tolua_release(L,u);                /* unmap from tolua tables */
      }
    }
    lua_settop(L,top);
  }
	return 0;
}




/* Register module events
	* It expects the metatable on the top of the stack
*/
TOLUA_API void tolua_moduleevents (lua_State* L)
{
	lua_pushstring(L,"__index");
	lua_pushcfunction(L,module_index_event);
	lua_rawset(L,-3);
	lua_pushstring(L,"__newindex");
	lua_pushcfunction(L,module_newindex_event);
	lua_rawset(L,-3);
}

/* Check if the object on the top has a module metatable
*/
TOLUA_API int tolua_ismodulemetatable (lua_State* L)
{
	int r = 0;
	if (lua_getmetatable(L,-1))
	{
		lua_pushstring(L,"__index");
		lua_rawget(L,-2);
		r = (lua_tocfunction(L,-1) == module_index_event);
		lua_pop(L,2);
	}
	return r;
}

/* Register class events
	* It expects the metatable on the top of the stack
*/
TOLUA_API void tolua_classevents (lua_State* L)
{
	lua_pushstring(L,"__index");
	lua_pushcfunction(L,class_index_event);
	lua_rawset(L,-3);
	lua_pushstring(L,"__newindex");
	lua_pushcfunction(L,class_newindex_event);
	lua_rawset(L,-3);
 
	lua_pushstring(L,"__add");
	lua_pushcfunction(L,class_add_event);
	lua_rawset(L,-3);
	lua_pushstring(L,"__sub");
	lua_pushcfunction(L,class_sub_event);
	lua_rawset(L,-3);
	lua_pushstring(L,"__mul");
	lua_pushcfunction(L,class_mul_event);
	lua_rawset(L,-3);
	lua_pushstring(L,"__div");
	lua_pushcfunction(L,class_div_event);
	lua_rawset(L,-3);

	lua_pushstring(L,"__lt");
	lua_pushcfunction(L,class_lt_event);
	lua_rawset(L,-3);
	lua_pushstring(L,"__le");
	lua_pushcfunction(L,class_le_event);
	lua_rawset(L,-3);
	lua_pushstring(L,"__eq");
	lua_pushcfunction(L,class_eq_event);
	lua_rawset(L,-3);

	lua_pushstring(L,"__len");
	lua_pushcfunction(L,class_len_event);
	lua_rawset(L,-3);

	lua_pushstring(L,"__gc");
	lua_pushcfunction(L,class_gc_event);
	lua_rawset(L,-3);
}

