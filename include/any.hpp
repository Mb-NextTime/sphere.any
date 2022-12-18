#pragma once

#include <exception>
#include <utility>
#include <functional>
#include <type_traits>

namespace sphere {


class bad_any_cast : public std::bad_cast {
public:
    virtual const char* what() const noexcept override;
};
#include "bad_cast.tpp"


class any
{
    template<typename _Tp, typename _VTp = std::decay_t<_Tp>>
    using _Decay_if_not_any = std::enable_if_t<!std::is_same_v<_VTp, any>, _VTp>;

    enum _Op {
	    _Op_access, _Op_get_type_info, _Op_clone, _Op_destroy, _Op_xfer
    };

    union _Arg;

    struct _Storage
    {
        constexpr _Storage() : _M_ptr{nullptr} {}

        // Prevent trivial copies of this type, buffer might hold a non-POD.
        _Storage(const _Storage&) = delete;
        _Storage& operator=(const _Storage&) = delete;

        void* _M_ptr;
    };

    template<typename _Tp>
    struct _Manager;

public:
    constexpr any() noexcept;

    any(const any& __other);
    any(any&& other) noexcept;

    template <typename _Tp, typename _VTp = _Decay_if_not_any<_Tp>,
    typename _Mgr = any::_Manager<_VTp>,
    std::enable_if_t<std::is_copy_constructible_v<_VTp>
    && !std::__is_in_place_type_v<_VTp>, bool> = true>
    any(_Tp&& __value)
    : _M_manager(&_Mgr::_S_manage)
    {
        _Mgr::_S_create(_M_storage, std::forward<_Tp>(__value));
    }

    template<class T, class... Args>
    explicit any(std::in_place_type_t<T>, Args&&...);

    ~any();

    any& operator=(const any& rhs);
    any& operator=(any&& rhs) noexcept;

    template<class T>
    any& operator=(T&& rhs);

    void reset() noexcept;

    bool has_value() const noexcept;

    
    template<class T, class... Args>
    friend any make_any(Args&&... args);

    template<typename _Tp>
    friend void* __any_caster(const any* __any);

    template<typename _Tp>
    static constexpr bool __is_valid_cast();

private:
    union _Arg
    {
        void* _M_obj;
        const std::type_info* _M_typeinfo;
        any* _M_any;
    };

    void (*_M_manager)(_Op, const any*, _Arg*);
    _Storage _M_storage;
    
};

// _Manager
#include "manager.tpp"

// class any
#include "any.tpp"

};
