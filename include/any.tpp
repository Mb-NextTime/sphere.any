#pragma once

constexpr any::any() noexcept
: _M_manager(nullptr)
{}

/// @brief Copy constructor, copies the state of @p __other
any::any(const any& __other)
{
    if (!__other.has_value()) 
        _M_manager = nullptr;
    else {
        _Arg __arg;
        __arg._M_any = this;
        __other._M_manager(_Op_clone, &__other, &__arg);
    }
}

/// @brief Move constructor, transfer the state from @p __other
any::any(any&& __other) noexcept
{
    if (!__other.has_value())
        _M_manager = nullptr;
    else {
        _Arg __arg;
        __arg._M_any = this;
        __other._M_manager(_Op_xfer, &__other, &__arg);
    }
}

any::~any()
{
    reset();
}

void any::reset() noexcept
{
    if (has_value()) {
        _M_manager(_Op_destroy, this, nullptr);
        _M_manager = nullptr;
    }
}

// assignments

/// Copy the state of another object.
any&
any::operator=(const any& __rhs)
{
    *this = any(__rhs);
    return *this;
}

template<typename _ValueType>
any&
any::operator=(_ValueType&& __rhs)
{
    *this = any(__rhs);
    return *this;
}

/**
 * @brief Move assignment operator
 *
 * @post @c !__rhs.has_value()
 */
any&
any::operator=(any&& __rhs) noexcept
{
    if (!__rhs.has_value())
        reset();
    else if (this != &__rhs) {
        _Arg __arg;
        __arg._M_any = this;
        __rhs._M_manager(_Op_xfer, &__rhs, &__arg);
    }
    return *this;
}

/// Reports whether there is a contained object or not.
bool any::has_value() const noexcept { return _M_manager != nullptr; }

template<typename _Tp>
constexpr bool any::__is_valid_cast()
{
    return std::__or_<std::is_reference<_Tp>, std::is_copy_constructible<_Tp>>::value;
}

/**
   * @brief Access the contained object.
   *
   * @tparam  _ValueType  A const-reference or CopyConstructible type.
   * @param   __any       The object to access.
   * @return  The contained object.
   * @throw   bad_any_cast If <code>
   *          __any.type() != typeid(remove_reference_t<_ValueType>)
   *          </code>
*/
template<typename _ValueType>
inline _ValueType any_cast(const any& __any)
{
    using _Up = std::__remove_cvref_t<_ValueType>;
    static_assert(any::__is_valid_cast<_ValueType>(),
    "Template argument must be a reference or CopyConstructible type");
    static_assert(std::is_constructible_v<_ValueType, const _Up&>,
    "Template argument must be constructible from a const value.");
    auto __p = any_cast<_Up>(&__any);
    if (__p)
        return static_cast<_ValueType>(*__p);
    __throw_bad_any_cast();
}


/**
   * @brief Access the contained object.
   *
   * @tparam  _ValueType  A reference or CopyConstructible type.
   * @param   __any       The object to access.
   * @return  The contained object.
   * @throw   bad_any_cast If <code>
   *          __any.type() != typeid(remove_reference_t<_ValueType>)
   *          </code>
*/
template<typename _ValueType>
inline _ValueType any_cast(any& __any)
{
    using _Up = std::__remove_cvref_t<_ValueType>;
    static_assert(any::__is_valid_cast<_ValueType>(),
    "Template argument must be a reference or CopyConstructible type");
    static_assert(std::is_constructible_v<_ValueType, _Up&>,
    "Template argument must be constructible from an lvalue.");
    auto __p = any_cast<_Up>(&__any);
    if (__p)
        return static_cast<_ValueType>(*__p);
    __throw_bad_any_cast();
}

/**
   * @brief Access the contained object.
   *
   * @tparam  _ValueType  A reference or CopyConstructible type.
   * @param   __any       The object to access.
   * @return  The contained object.
   * @throw   bad_any_cast If <code>
   *          __any.type() != typeid(remove_reference_t<_ValueType>)
   *          </code>
*/
template<typename _ValueType>
inline _ValueType any_cast(any&& __any)
{
    using _Up = std::__remove_cvref_t<_ValueType>;
    static_assert(any::__is_valid_cast<_ValueType>(),
    "Template argument must be a reference or CopyConstructible type");
    static_assert(std::is_constructible_v<_ValueType, _Up>,
    "Template argument must be constructible from an rvalue.");
    auto __p = any_cast<_Up>(&__any);
    if (__p)
        return static_cast<_ValueType>(std::move(*__p));
    __throw_bad_any_cast();
}

template<typename _Tp>
void* __any_caster(const any* __any)
{
    // any_cast<T> returns non-null if __any->type() == typeid(T) and
    // typeid(T) ignores cv-qualifiers so remove them:
    using _Up = std::remove_cv_t<_Tp>;
    // The contained value has a decayed type, so if decay_t<U> is not U,
    // then it's not possible to have a contained value of type U:
    if constexpr (!std::is_same_v<std::decay_t<_Up>, _Up>)
        return nullptr;
    // Only copy constructible types can be used for contained values:
    if constexpr (!std::is_copy_constructible_v<_Up>)
        return nullptr;
    // Try comparing function addresses, which works without RTTI
    if (__any->_M_manager == &any::_Manager<_Up>::_S_manage) {
        return any::_Manager<_Up>::_S_access(__any->_M_storage);
    }
    return nullptr;
}

/**
   * @brief Access the contained object.
   *
   * @tparam  _ValueType  The type of the contained object.
   * @param   __any       A pointer to the object to access.
   * @return  The address of the contained object if <code>
   *          __any != nullptr && __any.type() == typeid(_ValueType)
   *          </code>, otherwise a null pointer.
*/
template<typename _ValueType>
inline const _ValueType* any_cast(const any* __any) noexcept
{
    if constexpr (std::is_object_v<_ValueType>)
        return static_cast<_ValueType*>(__any_caster<_ValueType>(__any));
    return nullptr;
}

template<typename _ValueType>
inline _ValueType* any_cast(any* __any) noexcept
{
    if constexpr (std::is_object_v<_ValueType>)
        return static_cast<_ValueType*>(__any_caster<_ValueType>(__any));
    return nullptr;
}