//
//  ofxPubSubOscTypeUtils.h
//
//  Created by ISHII 2bit on 2016/05/29.
//
//

#pragma once

#ifndef ofxPubSubOscTypeUtils_h
#define ofxPubSubOscTypeUtils_h

#include <type_traits>
#include <tuple>

#include "ofxPubSubOscSettings.h"

namespace ofx {
    namespace PubSubOsc {
        template <typename T>
        using get_type = typename T::type;
        
        template <bool b, typename T = void>
        using enable_if_t = get_type<std::enable_if<b, T>>;
        
        template <bool b, typename T, typename U>
        using conditional_t = get_type<std::conditional<b, T, U>>;
        
        template <std::size_t index, typename ... arguments>
        using type_at = get_type<std::tuple_element<index, std::tuple<arguments ...>>>;
        
        namespace detail {
            template <typename T>
            struct remove_const_reference {
                using type = T;
            };
            template <typename T>
            struct remove_const_reference<T &> : remove_const_reference<T> {};
            template <typename T>
            struct remove_const_reference<T &&> : remove_const_reference<T> {};
            template <typename T>
            struct remove_const_reference<const T> : remove_const_reference<T> {};
        };
        
        template <typename T>
        using remove_const_reference = get_type<detail::remove_const_reference<T>>;
        
        template <typename T>
        using remove_ref = get_type<std::remove_reference<T>>;
        
        template <typename T>
        using add_reference_if_non_arithmetic = std::conditional<std::is_arithmetic<T>::value, T, T&>;
        
        template <typename T>
        using type_ref = typename add_reference_if_non_arithmetic<T>::type;
        
        template <typename T>
        struct is_integral_and_lt_64bit {
            static const bool value = std::is_integral<T>::value && (sizeof(T) < 8);
        };
        
        template <typename T>
        struct is_integral_and_geq_64bit {
            static const bool value = std::is_integral<T>::value && (8 <= sizeof(T));
        };

#pragma mark function
        
        template <typename T>
        struct is_callable {
            template <typename U, decltype(&U::operator()) = &U::operator()>
            struct checker {};
            template <typename U> static std::true_type  test(checker<U> *);
            template <typename>   static std::false_type test(...);
            static constexpr bool value = decltype(test<T>(nullptr))::value;
        };
        
        template <typename R, typename ... Arguments>
        struct is_callable<R(*)(Arguments ...)> {
            static constexpr bool value = true;
        };
        
        template <typename R, typename ... Arguments>
        struct is_callable<R(*&)(Arguments ...)> {
            static constexpr bool value = true;
        };
        
        template <typename R, typename ... Arguments>
        struct is_callable<R(&)(Arguments ...)> {
            static constexpr bool value = true;
        };
        
        template <typename R, typename ... Arguments>
        struct is_callable<R(Arguments ...)> {
            static constexpr bool value = true;
        };
        
        template <typename R, typename ... Arguments>
        struct is_callable<std::function<R(Arguments ...)>> {
            static constexpr bool value = true;
        };
        
        namespace detail {
            template <typename ret, typename ... arguments>
            struct function_traits {
                static constexpr std::size_t arity = sizeof...(arguments);
                using result_type = ret;
                using arguments_types_tuple = std::tuple<arguments ...>;
                template <std::size_t index>
                using argument_type = type_at<index, arguments ...>;
                using function_type = std::function<ret(arguments ...)>;
                template <typename function_t>
                static constexpr function_type cast(function_t f) {
                    return static_cast<function_type>(f);
                }
            };
        };
        
        template <typename T>
        struct function_traits : public function_traits<decltype(&T::operator())> {};
        
        template <typename class_type, typename ret, typename ... arguments>
        struct function_traits<ret(class_type::*)(arguments ...) const>
        : detail::function_traits<ret, arguments ...> {};
        
        template <typename class_type, typename ret, typename ... arguments>
        struct function_traits<ret(class_type::*)(arguments ...)>
        : detail::function_traits<ret, arguments ...> {};
        
        template <typename ret, typename ... arguments>
        struct function_traits<ret(*)(arguments ...)>
        : detail::function_traits<ret, arguments ...> {};
        
        template <typename ret, typename ... arguments>
        struct function_traits<ret(*&)(arguments ...)>
        : detail::function_traits<ret, arguments ...> {};
        
        template <typename ret, typename ... arguments>
        struct function_traits<ret(arguments ...)>
        : detail::function_traits<ret, arguments ...> {};
        
        template <typename ret, typename ... arguments>
        struct function_traits<std::function<ret(arguments ...)>>
        : detail::function_traits<ret, arguments ...> {};
        
        template<typename T>
        using result_type = typename function_traits<T>::result_type;
        
        template<typename T>
        using arguments_types_tuple = typename function_traits<T>::arguments_types_tuple;
        
        template<typename T, std::size_t index>
        using argument_type = typename function_traits<T>::template argument_type<index>;
        
        template<typename T>
        struct arity {
            static constexpr std::size_t value = function_traits<T>::arity;
        };
        
        template <typename function_t>
        constexpr auto cast_lambda(function_t f)
        -> typename function_traits<function_t>::function_type {
            return static_cast<typename function_traits<function_t>::function_type>(f);
        }
        
#pragma mark method
        
        template <typename obj, typename meth>
        struct is_bindable
        : public std::false_type {};

        template <typename obj, typename res, typename ... args>
        struct is_bindable<obj *, res(obj::*)(args ...)>
        : public std::true_type {};
        
        template <typename obj, typename res, typename ... args>
        struct is_bindable<obj *, res(obj::*)(args ...) const>
        : public std::true_type {};
        
        template <typename obj, typename res, typename ... args>
        struct is_bindable<const obj *, res(obj::*)(args ...) const>
        : public std::true_type {};

        template <typename obj, typename res, typename ... args>
        struct is_bindable<obj, res(obj::*)(args ...)>
        : public std::true_type {};

        template <typename obj, typename res, typename ... args>
        struct is_bindable<obj, res(obj::*)(args ...) const>
        : public std::true_type {};

        template <typename obj, typename res, typename ... args>
        struct is_bindable<const obj, res(obj::*)(args ...) const>
        : public std::true_type {};
        
        template <typename meth>
        struct get_arg_num;
        
        template <typename obj, typename res, typename ... args>
        struct get_arg_num<res(obj::*)(args ...)> {
            static constexpr std::size_t value = sizeof...(args);
        };

        template <typename obj, typename res, typename ... args>
        struct get_arg_num<res(obj::*)(args ...) const> {
            static constexpr std::size_t value = sizeof...(args);
        };

        template <typename meth>
        constexpr std::size_t get_arg_num_v() {
            return get_arg_num<meth>::value;
        }

#define P(n) std::placeholders:: _##n
        
        template <typename obj, typename meth>
        auto bind(obj o, meth m)
        -> enable_if_t<get_arg_num_v<meth>() == 0, typename function_traits<meth>::function_type> {
            return std::bind(m, o);
        }

        template <typename obj, typename meth>
        auto bind(obj o, meth m)
        -> enable_if_t<get_arg_num_v<meth>() == 1, typename function_traits<meth>::function_type> {
            return std::bind(m, o, P(1));
        }
        
        template <typename obj, typename meth>
        auto bind(obj o, meth m)
        -> enable_if_t<get_arg_num_v<meth>() == 2, typename function_traits<meth>::function_type> {
            return std::bind(m, o, P(1), P(2));
        }
        
        template <typename obj, typename meth>
        auto bind(obj o, meth m)
        -> enable_if_t<get_arg_num_v<meth>() == 3, typename function_traits<meth>::function_type> {
            return std::bind(m, o, P(1), P(2), P(3));
        }
        
        template <typename obj, typename meth>
        auto bind(obj o, meth m)
        -> enable_if_t<get_arg_num_v<meth>() == 4, typename function_traits<meth>::function_type> {
            return std::bind(m, o, P(1), P(2), P(3), P(4));
        }
        
        template <typename obj, typename meth>
        auto bind(obj o, meth m)
        -> enable_if_t<get_arg_num_v<meth>() == 5, typename function_traits<meth>::function_type> {
            return std::bind(m, o, P(1), P(2), P(3), P(4), P(5));
        }
        
        template <typename obj, typename meth>
        auto bind(obj o, meth m)
        -> enable_if_t<get_arg_num_v<meth>() == 6, typename function_traits<meth>::function_type> {
            return std::bind(m, o, P(1), P(2), P(3), P(4), P(5), P(6));
        }
        
        template <typename obj, typename meth>
        auto bind(obj o, meth m)
        -> enable_if_t<get_arg_num_v<meth>() == 7, typename function_traits<meth>::function_type> {
            return std::bind(m, o, P(1), P(2), P(3), P(4), P(5), P(6), P(7));
        }
        
        template <typename obj, typename meth>
        auto bind(obj o, meth m)
        -> enable_if_t<get_arg_num_v<meth>() == 8, typename function_traits<meth>::function_type> {
            return std::bind(m, o, P(1), P(2), P(3), P(4), P(5), P(6), P(7), P(8));
        }
        
        template <typename obj, typename meth>
        auto bind(obj o, meth m)
        -> enable_if_t<get_arg_num_v<meth>() == 9, typename function_traits<meth>::function_type> {
            return std::bind(m, o, P(1), P(2), P(3), P(4), P(5), P(6), P(7), P(8), P(9));
        }
        
        template <typename obj, typename meth>
        auto bind(obj o, meth m)
        -> enable_if_t<get_arg_num_v<meth>() == 10, typename function_traits<meth>::function_type> {
            return std::bind(m, o, P(1), P(2), P(3), P(4), P(5), P(6), P(7), P(8), P(9), P(10));
        }
        
#undef P

#pragma mark sequences
        namespace sequences {
            template <typename type, type ... ns>
            struct integer_sequence {
                using value_type = type;
                static constexpr std::size_t size() noexcept { return sizeof...(ns); }
            };
            
            namespace detail {
                template <typename integer_type, integer_type n, integer_type ... ns>
                struct make_integer_sequence {
                    struct sequence_wrapper { using type = integer_sequence<integer_type, ns ...>; };
                    using type = get_type<conditional_t<
                        0 < n,
                        make_integer_sequence<integer_type, n - 1, n - 1, ns ...>,
                        sequence_wrapper
                    >>;
                };
            };
            
            template <typename type, type n>
            using make_integer_sequence = get_type<detail::make_integer_sequence<type, n>>;
            
            template <std::size_t ... ns>
            using index_sequence = integer_sequence<std::size_t, ns ...>;
            
            template <std::size_t n>
            using make_index_sequence = make_integer_sequence<std::size_t, n>;
            
            template <typename... types>
            using index_sequence_for = make_index_sequence<sizeof...(types)>;
        };
        using namespace sequences;
        
        namespace applying {
            template <typename res, typename ... arguments, std::size_t ... indices>
            res apply(std::function<res(arguments ...)> f,
                       std::tuple<remove_const_reference<arguments> ...> &args,
                       index_sequence<indices ...> &&)
            {
                return f(std::forward<arguments>(std::get<indices>(args)) ...);
            }
            
            template <typename res, typename ... arguments>
            res apply(std::function<res(arguments ...)> f, std::tuple<remove_const_reference<arguments> ...> &args) {
                return apply(f, args, index_sequence_for<arguments ...>());
            }
        };
        using namespace applying;
    };
};


bool operator==(const ofMatrix3x3 &x, const ofMatrix3x3 &y) {
    return (x.a == y.a) && (x.b == y.b) && (x.c == y.c)
    && (x.d == y.d) && (x.e == y.e) && (x.f == y.f)
    && (x.g == y.g) && (x.h == y.h) && (x.i == y.i);
}

bool operator!=(const ofMatrix3x3 &x, const ofMatrix3x3 &y) {
    return !operator==(x, y);
}

bool operator==(const ofMatrix4x4 &x, const ofMatrix4x4 &y) {
    return (x._mat[0][0] == y._mat[0][0]) && (x._mat[0][1] == y._mat[0][1]) && (x._mat[0][2] == y._mat[0][2]) && (x._mat[0][3] == y._mat[0][3])
    && (x._mat[1][0] == y._mat[1][0]) && (x._mat[1][1] == y._mat[1][1]) && (x._mat[1][2] == y._mat[1][2]) && (x._mat[1][3] == y._mat[1][3])
    && (x._mat[2][0] == y._mat[2][0]) && (x._mat[2][1] == y._mat[2][1]) && (x._mat[2][2] == y._mat[2][2]) && (x._mat[2][3] == y._mat[2][3])
    && (x._mat[3][0] == y._mat[3][0]) && (x._mat[3][1] == y._mat[3][1]) && (x._mat[3][2] == y._mat[3][2]) && (x._mat[3][3] == y._mat[3][3]);
}

bool operator!=(const ofMatrix4x4 &x, const ofMatrix4x4 &y) {
    return !operator==(x, y);
}

bool operator==(const ofBuffer &x, const ofBuffer &y) {
    return (x.size() == y.size()) && (memcmp(x.getData(), y.getData(), x.size()) == 0);
}

bool operator!=(const ofBuffer &x, const ofBuffer &y) {
    return !operator==(x, y);
}

#ifndef bbb_explicit_bool
#define bbb_explicit_bool

namespace bbb {
    struct explicit_bool {
        explicit_bool()
        : b(false) {}
        explicit_bool(bool b)
        : b(b) {}
        template <typename _>
        explicit_bool(_) = delete;
        bool &operator=(bool b) { this->b = b; }
        
        operator bool&() { return b; };
        operator bool() const { return b; };
        bool get() const { return b; };
        bool b;
    };
};

#endif

#endif /* ofxPubSubOscTypeUtils_h */
