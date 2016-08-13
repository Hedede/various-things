/*
   Copyright (C) 2014  absurdworlds

   License LGPLv3-only:
   GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
   This is free software: you are free to change and redistribute it.
   There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrengin_hdf_Type_
#define _hrengin_hdf_Type_

#include <type_traits>

namespace hrengin {
namespace hdf {

/*! Enumeration of possible HDF value types */
enum class Type {
	Integer,
	UnsignedInteger,
	Float,
	Boolean,
	String,
	Vector2d,
	Vector3d,
	Vector4d,
	Enum,
	Unknown
};

template<typename T>
inline bool checkType (hdf::Type type)
{
	return false;
}

template<typename T>
inline typename std::enable_if<
	std::is_integral<T>::value &&
	std::is_unsigned<T>::value &&
	!std::is_same<T, bool>::value,
	bool>::type
checkType (hdf::Type type)
{
	return Type::Integer == type;
}

template<typename T>
inline typename std::enable_if<
	std::is_integral<T>::value &&
	std::is_signed<T>::value &&
	!std::is_same<T, bool>::value,
	bool>::type
checkType (hdf::Type type)
{
	return Type::Integer == type;
}

template<typename T>
inline typename std::enable_if<
	std::is_floating_point<T>::value,
	bool>::type
checkType (hdf::Type type)
{
	return Type::Float == type;
}

template<>
inline bool checkType<bool> (hdf::Type type)
{
	return Type::Boolean == type;
}

template<>
inline bool checkType<std::string> (hdf::Type type)
{
	return Type::String == type;
}

template<>
inline bool checkType<Vector2d<f32>> (hdf::Type type)
{
	return Type::Vector2d == type;
}

template<>
inline bool checkType<Vector3d<f32>> (hdf::Type type)
{
	return Type::Vector3d == type;
}

template<typename T>
inline hdf::Type deduceType ()
{
	return Type::Unknown;
}

template<typename T>
inline typename std::enable_if<
	std::is_integral<T>::value &&
	std::is_unsigned<T>::value &&
	!std::is_same<T, bool>::value,
	hdf::Type>::type
deduceType ()
{
	return Type::Integer;
}

template<typename T>
inline typename std::enable_if<
	std::is_integral<T>::value &&
	std::is_signed<T>::value &&
	!std::is_same<T, bool>::value,
	hdf::Type>::type
deduceType ()
{
	return Type::Integer;
}


template<typename T>
inline typename std::enable_if<
	std::is_floating_point<T>::value,
	hdf::Type>::type
deduceType ()
{
	return Type::Float;
}

template<typename T>
inline typename std::enable_if<
	std::is_same<T, bool>::value,
	hdf::Type>::type
deduceType ()
{
	return Type::Boolean;
}

template<typename T>
inline typename std::enable_if<
	std::is_same<T, std::string>::value,
	hdf::Type>::type
deduceType ()
{
	return Type::String;
}

template<typename T>
inline typename std::enable_if<
	std::is_same<T, Vector2d<f32>>::value,
	hdf::Type>::type
deduceType ()
{
	return Type::Vector2d;
}

template<typename T>
inline typename std::enable_if<
	std::is_same<T, Vector3d<f32>>::value,
	hdf::Type>::type
deduceType ()
{
	return Type::Vector3d;
}

template<typename T>
inline typename std::enable_if<
	std::is_integral<T>::value &&
//	std::is_unsigned<T>::value &&
	!std::is_same<T, bool>::value,
	bool>::type
checkType (hdf::Type type, T)
{
	return Type::Integer == type;
}

template<typename T>
inline typename std::enable_if<
	std::is_floating_point<T>::value,
	bool>::type
checkType (hdf::Type type, T)
{
	return Type::Float == type;
}

template<typename T>
inline typename std::enable_if<
	std::is_same<T, bool>::value,
	bool>::type
checkType (hdf::Type type, T)
{
	return Type::Boolean == type;
}

template<typename T>
inline typename std::enable_if<
	std::is_same<T, std::string>::value,
	bool>::type
checkType (hdf::Type type, T)
{
	return Type::String == type;
}

template<typename T>
inline typename std::enable_if<
	std::is_same<T, Vector2d<f32>>::value,
	bool>::type
checkType (hdf::Type type, T)
{
	return Type::Vector2d == type;
}

template<typename T>
inline typename std::enable_if<
	std::is_same<T, Vector3d<f32>>::value,
	bool>::type
checkType (hdf::Type type, T)
{
	return Type::Vector3d == type;
}

template<typename T>
inline hdf::Type deduceType (T)
{
	return Type::Unknown;
}

template<typename T>
inline typename std::enable_if<
	std::is_integral<T>::value &&
//	std::is_unsigned<T>::value &&
	!std::is_same<T, bool>::value,
	hdf::Type>::type
deduceType (T)
{
	return Type::Integer;
}

template<typename T>
inline typename std::enable_if<
	std::is_floating_point<T>::value,
	hdf::Type>::type
deduceType (T)
{
	return Type::Float;
}


template<typename T>
inline typename std::enable_if<
	std::is_same<T, bool>::value,
	hdf::Type>::type
deduceType (T)
{
	return Type::Boolean;
}

template<typename T>
inline typename std::enable_if<
	std::is_same<T, std::string>::value,
	hdf::Type>::type
deduceType (T)
{
	return Type::String;
}

template<typename T>
inline typename std::enable_if<
	std::is_same<T, Vector2d<f32>>::value,
	hdf::Type>::type
deduceType (T)
{
	return Type::Vector2d;
}

template<typename T>
inline typename std::enable_if<
	std::is_same<T, Vector3d<f32>>::value,
	hdf::Type>::type
deduceType (T)
{
	return Type::Vector3d;
}

} // namespace hdf
} // namespace hrengin

#endif//_hrengin_hdf_Type_
