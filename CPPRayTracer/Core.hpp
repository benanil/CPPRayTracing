#pragma once

#ifndef FINLINE 
#	ifndef _MSC_VER 
#		define FINLINE  inline
#	else
#		define FINLINE __forceinline
#	endif
#endif

#ifndef VECTORCALL
#	ifndef _MSC_VER
#		define VECTORCALL 
#	else
#	   define VECTORCALL __vectorcall
#	endif
#endif

// coppied from here: winnt.h line 2481  DEFINE_ENUM_FLAG_OPERATORS we are not using this because we don't want to include winnt.h
// Define operator overloads to enable bit operations on enum values that are
// used to define flags. Use HS_CREATE_ENUM_OPERATORS(YOUR_TYPE) to enable these
// Templates are defined here in order to avoid a dependency on C++ <type_traits> header file,
template <size_t S> struct _ENUM_TO_INT;
template <> struct _ENUM_TO_INT<1> { typedef char  type; };
template <> struct _ENUM_TO_INT<2> { typedef short type; };
template <> struct _ENUM_TO_INT<4> { typedef int  type; };
template <> struct _ENUM_TO_INT<8> { typedef long type; };
// used as an approximation of std::underlying_type<T>
template <class T> struct UnderlyingType {
	typedef typename _ENUM_TO_INT<sizeof(T)>::type type;
};

#define A_CREATE_ENUM_OPERATORS(ENUMTYPE) \
inline constexpr ENUMTYPE operator | (ENUMTYPE a, ENUMTYPE b) noexcept { return ENUMTYPE(((UnderlyingType<ENUMTYPE>::type)a) | ((UnderlyingType<ENUMTYPE>::type)b)); } \
inline ENUMTYPE &operator |= (ENUMTYPE &a, ENUMTYPE b) noexcept { return (ENUMTYPE&)(((UnderlyingType<ENUMTYPE>::type&)a) |= ((UnderlyingType<ENUMTYPE>::type)b)); } \
inline constexpr ENUMTYPE operator & (ENUMTYPE a, ENUMTYPE b) noexcept { return ENUMTYPE(((UnderlyingType<ENUMTYPE>::type)a) & ((UnderlyingType<ENUMTYPE>::type)b)); } \
inline ENUMTYPE &operator &= (ENUMTYPE &a, ENUMTYPE b) noexcept { return (ENUMTYPE&)(((UnderlyingType<ENUMTYPE>::type&)a) &= ((UnderlyingType<ENUMTYPE>::type)b)); } \
inline constexpr ENUMTYPE operator ~ (ENUMTYPE a) noexcept { return ENUMTYPE(~((UnderlyingType<ENUMTYPE>::type)a)); } \
inline constexpr ENUMTYPE operator ^ (ENUMTYPE a, ENUMTYPE b) noexcept { return ENUMTYPE(((UnderlyingType<ENUMTYPE>::type)a) ^ ((UnderlyingType<ENUMTYPE>::type)b)); } \
inline ENUMTYPE &operator ^= (ENUMTYPE &a, ENUMTYPE b) noexcept { return (ENUMTYPE&)(((UnderlyingType<ENUMTYPE>::type&)a) ^= ((UnderlyingType<ENUMTYPE>::type)b)); } 


using byte		= unsigned char;
using uint		= unsigned int;
using ushort	= unsigned short;
using ulong		= unsigned long;

namespace Helper
{
    // http://www.cse.yorku.ca/~oz/hash.html
    constexpr uint StringToHash(const char* str)
    {
        uint hash = 5381;
        int c = 0;
        while (c = *str++) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        return hash;
    }
}

