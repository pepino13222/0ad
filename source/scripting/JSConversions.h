// A general system of converting between native objects and their JavaScript representations
// Mark Thompson (mark@wildfiregames.com / mot20@cam.ac.uk)

#ifndef JSCONVERSIONS_INCLUDED
#define JSCONVERSIONS_INCLUDED

#include "scripting/ScriptingHost.h"

class CEntity;
class HEntity;
class CBaseEntity;
class CStrW;
class CScriptObject;
class CObjectEntry;
class CVector3D;

// -----
//
// Defaults
//
// -----

template<typename T> T* ToNative( JSContext* cx, JSObject* obj )
{
	return( (T*)JS_GetInstancePrivate( cx, obj, &T::JSI_class, NULL ) );
}

template<typename T> JSObject* ToScript( T* Native )
{
	if( !Native )
		return( JSVAL_NULL );
	return( Native->GetScript() );
}

template<typename T> T* ToNative( jsval v )
{
	if( !JSVAL_IS_OBJECT( v ) ) return( NULL );
	return( ToNative<T>( g_ScriptingHost.GetContext(), JSVAL_TO_OBJECT( v ) ) );
}

template<typename T> bool ToPrimitive( JSContext* cx, jsval v, T& Storage )
{
	T* Native = ToNative<T>( v );
	if( !Native ) return( false );
	Storage = *Native;
	return( true );
}

template<typename T> inline T ToPrimitive( JSContext* cx, jsval v ) { T Temp; ToPrimitive( cx, v, Temp ); return( Temp ); }
template<typename T> inline T ToPrimitive( jsval v ) { return( ToPrimitive( g_ScriptingHost.GetContext(), v ) ); }

template<typename T> jsval ToJSVal( T& Native )
{
	return( OBJECT_TO_JSVAL( ToScript<T>( &Native ) ) );
}

template<typename T> jsval ToJSVal( const T& Native );

// -----
//
// Overrides
//
// -----

// CVector3D
template<> CVector3D* ToNative<CVector3D>( JSContext* cx, JSObject* obj );
template<> JSObject* ToScript<CVector3D>( CVector3D* Native );

// CBaseEntity
template<> bool ToPrimitive<CBaseEntity*>( JSContext* cx, jsval v, CBaseEntity*& Storage );
template<> JSObject* ToScript<CBaseEntity*>( CBaseEntity** Native );

// CObjectEntry
template<> bool ToPrimitive<CObjectEntry*>( JSContext* cx, jsval v, CObjectEntry*& Storage );
template<> jsval ToJSVal<CObjectEntry*>( CObjectEntry*& Native );

// HEntity
template<> HEntity* ToNative<HEntity>( JSContext* cx, JSObject* obj );
template<> JSObject* ToScript<HEntity>( HEntity* Native );

// CScriptObject
template<> bool ToPrimitive<CScriptObject>( JSContext* cx, jsval v, CScriptObject& Storage );
template<> jsval ToJSVal<CScriptObject>( CScriptObject& Native );

// i32
template<> bool ToPrimitive<int>( JSContext* cx, jsval v, i32& Storage );
template<> jsval ToJSVal<int>( const i32& Native );
template<> jsval ToJSVal<int>( i32& Native );

// double
template<> bool ToPrimitive<double>( JSContext* cx, jsval v, double& Storage );
template<> jsval ToJSVal<double>( const double& Native );
template<> jsval ToJSVal<double>( double& Native );

// float
template<> bool ToPrimitive<float>( JSContext* cx, jsval v, float& Storage );
template<> jsval ToJSVal<float>( const float& Native );
template<> jsval ToJSVal<float>( float& Native );

// bool
template<> bool ToPrimitive<bool>( JSContext* cx, jsval v, bool& Storage );
template<> jsval ToJSVal<bool>( const bool& Native );
template<> jsval ToJSVal<bool>( bool& Native );

// CStrW
template<> bool ToPrimitive<CStrW>( JSContext* cx, jsval v, CStrW& Storage );
template<> jsval ToJSVal<CStrW>( const CStrW& Native );
template<> jsval ToJSVal<CStrW>( CStrW& Native );

// jsval

template<> jsval ToJSVal<jsval>( const jsval& Native );





#endif
