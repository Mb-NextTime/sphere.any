#pragma once


const char* sphere::bad_any_cast::what() const noexcept { return "bad any_cast"; }

[[gnu::noreturn]] inline void __throw_bad_any_cast()
{
#if __cpp_exceptions
    throw bad_any_cast{};
#else
    __builtin_abort();
#endif
}