#pragma once
#include "any.hpp"
template<typename _Tp>
struct any::_Manager
{
	static void
	_S_manage(_Op __which, const any* __any, _Arg* __arg)
    {
        // The contained object is *_M_storage._M_ptr
        auto __ptr = static_cast<const _Tp*>(__any->_M_storage._M_ptr);
        switch (__which)
        {
        case _Op::ACCESS :
            __arg->_M_obj = const_cast<_Tp*>(__ptr);
            break;
        case _Op::GET_TYPE_INFO:
            break;
        case _Op::CLONE:
            __arg->_M_any->_M_storage._M_ptr = new _Tp(*__ptr);
            __arg->_M_any->_M_manager = __any->_M_manager;
            break;
        case _Op::DESTROY:
            delete __ptr;
            break;
        case _Op::XFER:
            __arg->_M_any->_M_storage._M_ptr = __any->_M_storage._M_ptr;
            __arg->_M_any->_M_manager = __any->_M_manager;
            const_cast<any*>(__any)->_M_manager = nullptr;
            break;
        }
    }

	template<typename _Up>
    static void
    _S_create(_Storage& __storage, _Up&& __value)
    {
    __storage._M_ptr = new _Tp(std::forward<_Up>(__value));
    }
	template<typename... _Args>
    static void
    _S_create(_Storage& __storage, _Args&&... __args)
    {
    __storage._M_ptr = new _Tp(std::forward<_Args>(__args)...);
    }
	static _Tp*
	_S_access(const _Storage& __storage)
	{
	  // The contained object is in *__storage._M_ptr
	  return static_cast<_Tp*>(__storage._M_ptr);
	}
};