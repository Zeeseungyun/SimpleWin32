#pragma once
#define ZEE_DEFINE_DEFAULT_COPY_FUNCTION(type)	\
	private:									\
	type& operator=(const type&) = default;		\
	type(const type&)= default;					\
	public: 

#define ZEE_DEFINE_DEFAULT_MOVE_FUNCTION(type)	\
	private:									\
	type& operator=(type&&) = default;			\
	type(type&&)= default;						\
	public: 

#define ZEE_DEFINE_DEFAULT_MOVE_AND_COPY_FUNCTION(type) \
	ZEE_DEFINE_DEFAULT_MOVE_FUNCTION(type)				\
	ZEE_DEFINE_DEFAULT_COPY_FUNCTION(type)				

#define ZEE_DEFINE_REMOVE_COPY_FUNCTION(type)	\
	private:									\
	type& operator=(const type&) = delete;		\
	type(const type&)= delete;					\
	public: 

#define ZEE_DEFINE_REMOVE_MOVE_FUNCTION(type)	\
	private:									\
	type& operator=(type&&) = delete;			\
	type(type&&)= delete;						\
	public: 

#define ZEE_DEFINE_REMOVE_MOVE_AND_COPY_FUNCTION(type)	\
	ZEE_DEFINE_REMOVE_MOVE_FUNCTION(type)				\
	ZEE_DEFINE_REMOVE_COPY_FUNCTION(type)				
