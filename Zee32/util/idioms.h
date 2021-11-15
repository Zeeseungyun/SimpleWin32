#pragma once
#define REMOVE_COPY_FUNCTION_DEFINE(type_name) type_name(const type_name&) = delete; type_name& operator=(const type_name&) = delete; 
#define REMOVE_MOVE_FUNCTION_DEFINE(type_name)  type_name(type_name&&) = delete;	type_name& operator=(type_name&&) = delete;
#define REMOVE_MOVE_AND_COPY_FUNCTION_DEFINE(type_name) REMOVE_COPY_FUNCTION_DEFINE(type_name); REMOVE_MOVE_FUNCTION_DEFINE(type_name);

#define DEFAULT_COPY_FUNCTION_DEFINE(type_name) type_name(const type_name&) = default; type_name& operator=(const type_name&) = default; 
#define DEFAULT_MOVE_FUNCTION_DEFINE(type_name) type_name(type_name&&) = default;	type_name& operator=(type_name&&) = default;
#define DEFAULT_MOVE_AND_COPY_FUNCTION_DEFINE(type_name) DEFAULT_COPY_FUNCTION_DEFINE(type_name); DEFAULT_MOVE_FUNCTION_DEFINE(type_name);
