/*
    This file is part of Element
    Copyright (C) 2019  Kushview, LLC.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "sol/sol.hpp"
#include "scripting/ScriptingEngine.h"
#include "scripting/ScriptManager.h"
#include "scripting/LuaBindings.h"
#include "Globals.h"

#ifndef EL_LUA_SPATH
 #define EL_LUA_SPATH ""
#endif

namespace Element {

namespace LuaHelpers {

static int exceptionHandler (lua_State* L, sol::optional<const std::exception &> e, sol::string_view description) {
    return sol::stack::push (L, description);
    
}

}

//=============================================================================

//=============================================================================
class ScriptingEngine::Impl
{
public:
    Impl (ScriptingEngine& e)
        : owner (e)
    {
       
    }
    
    ~Impl() {}

    void scanDefaultLoctaion()
    {
        manager.scanDefaultLocation();
       
    }

    ScriptManager& getManager() { return manager; }

private:
    friend class ScriptingEngine;
    ScriptingEngine& owner;
    ScriptManager manager;
};

//=============================================================================
ScriptingEngine::ScriptingEngine()
{
    impl.reset (new Impl (*this));
    Lua::initializeState (lua);
}

ScriptingEngine::~ScriptingEngine()
{
    Lua::clearGlobals (lua);
    lua.collect_garbage();
}

void ScriptingEngine::initialize (Globals& g)
{
    world = &g;
    Lua::setGlobals (lua, g);
    lua.set_exception_handler (LuaHelpers::exceptionHandler);
}

ScriptManager& ScriptingEngine::getScriptManager()
{
    return impl->manager;
}

}
