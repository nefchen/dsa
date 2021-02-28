/*
 * Created on 28.02.2021 by nefchen.
 */

#ifndef VIEWS_LOADER_HPP
#define VIEWS_LOADER_HPP

#include <memory>
#include <functional>


namespace view
{
    struct View;
    template<typename T> struct from_type
    {};

    struct Loader
    {
        template<typename T, typename... Args>
        Loader(from_type<T> vt, Args... args)
        {
            m_delegate_load_func = [args...]() {
                return std::make_unique<T>(args...);
            };
        };

        std::unique_ptr<View> operator()()
        {
            return m_delegate_load_func();
        };

        private:
            std::function<std::unique_ptr<View>()> m_delegate_load_func;
    };
}

#endif  // VIEWS_LOADER_HPP

