# 类型萃取与模板特化详解

```cpp
using para_type = dim2_parameterized_dim1::para_type;
template <class T>
        struct param_type_number : std::integral_constant<para_type, error> {
            using _use_type = T;
        };
```
#### `std::integral_constant<para_type, X>`

- **用途**：使得 `dim2_parameterized_dim1::para_type::X` 可以作为一个类。

#### `param_type_number<X>`

- **用途**：可以在其他模板结构体或函数中引用 `param_type_number<X>::_use_type` 来获取关联的类型 `X`。  
- **注意**：如果 `<X>` 中的 `X` 是没有显式特化的版本，则 `X` 为 `error`。

```cpp
        template <class... ts>
        struct type_judge : param_type_number<void> {};
        template <class T>
        struct type_judge<T> : param_type_number<T> {};
```
#### `type_judge<T>`

- **用途**：只有在模板参数只有一个的时候会继承 `T`，否则继承 `void`。

## 类型萃取 `type_extract<>`

```cpp

        template <class except_return_type, class func, class judge1, bool>
        struct type_extract : type_judge<void> {};

        template <class T, class Ret, class... Args, bool _b>
        struct type_extract<T, Ret(Args...), std::enable_if_t<std::is_convertible_v<Ret, T>, is_a_funcp>, _b>
            : type_judge<std::remove_cvref_t<Args>...> {};

    #define TYPE_OBJ_EXTRACT(X)                                                                                                 \
        template <class T, class callable_obj, bool _b>                                                                         \
        struct type_extract <                                                                                                   \
            T,                                                                                                                  \
            callable_obj,                                                                                                       \
            std::enable_if_t <  std::is_invocable_v<callable_obj, const X &> &&                                                 \
                                    std::is_convertible_v<std::invoke_result_t<callable_obj, const X &>, T> &&                  \
                                    (   !                                                                                       \
                                        (                                                                                       \
                                            ( requires /* obj->const function->!const */                                        \
                                              (std::invoke_result_t<callable_obj, const X &> (callable_obj::*p) (const X &))    \
                                              { p = &callable_obj::operator(); }                                                \
                                            ) && _b                                                                             \
                                        )                                                                                       \
                                    ),                                                                                          \
                                is_a_object>,                                                                                   \
            _b>                                                                                                                 \
            : type_judge<X> {}
```

### 参数说明

1. **`except_return_type`**  
   - **含义**：预期的返回类型（Expected Return Type）。  
   - **作用**：用于指定从函数或可调用对象中期望返回的类型，以便进行类型匹配和转换检查。  

2. **`func`**  
   - **含义**：函数类型（Function Type）。  
   - **作用**：表示被检查的函数或可调用对象的类型，包括其参数和返回类型。  

3. **`judge1`**  
   - **含义**：判断类型（Judge Type）。  
   - **作用**：用于标识类型提取的判别条件，例如 `is_a_funcp` 或 `is_a_object`。  

4. **`bool`**  
   - **含义**：布尔标志（Boolean Flag）。  
   - **作用**：控制类型提取过程中的特定逻辑分支或条件判断。  

### 特化条件

- **条件 1**：
  - **函数参数**必须是函数类型，即 `Ret(Args...)` 形式。
  - **返回类型** `Ret` 必须可转换为 `except_return_type`（即 `std::is_convertible_v<Ret, T>` 为 `true`）。
  - **`judge1` 参数**必须是 `is_a_funcp`。
  - **`bool` 参数**的值（`_b`）可以是任意值，但在特化中通常会影响行为。

- **条件 2**：
  - **可调用对象**必须能够以 `const X&` 作为参数被调用（即 `std::is_invocable_v<callable_obj, const X&>` 返回 `true`）。
  - **返回类型**可转换性：
    - **可调用对象**调用后的返回类型必须能够转换为 `T`（即 `std::is_convertible_v<std::invoke_result_t<callable_obj, const X&>, T>` 返回 `true`）。
  - **特定条件的否定**：
    - 不满足以下两个条件的组合：
      - 可调用对象拥有一个接受 `const X&` 参数的 `operator()` 成员函数。
      - 布尔标志 `_b` 为 `true`（即 `!(has_const_operator && _b)` 必须为 `true`）。
  - **`judge1` 必须为 `is_a_object`**。

## 模板特化 `ptr0_obj1`

### 特化版本

| 特化版本 | `Function` 类型 | `T` 类型 | 第三个模板参数条件 | 继承类型          | `_i_func` 返回类型  | 使用场景                                          |
|----------|----------------|---------|--------------------|-------------------|---------------------|---------------------------------------------------|
| 主模板   | 任意（默认）     | 任意     | `void`             | `std::false_type` | `std::optional<T>`  | 处理未特化的情况，默认不满足条件                 |
| 第二个特化 | 任意          | `void`  | 任意 `F`           | `std::false_type` | `std::optional<bool>` | 处理返回类型为 `void` 的情况，不满足特定条件        |
| 第三个特化 | 类类型          | 非 `void` | `std::is_class_v<std::remove_cvref_t<Function>>` | `std::true_type`  | `std::optional<T>`  | 处理 `Function` 为类类型且返回类型非 `void`       |
| 第四个特化 | 类类型          | `void`   | `std::is_class_v<std::remove_cvref_t<Function>>` | `std::true_type`  | `std::optional<bool>` | 处理 `Function` 为类类型且返回类型为 `void`       |

### 各特化版本解释

#### 1. 主模板

```cpp
        template <class Function, class T, class = void>
        struct ptr0_obj1 : std::false_type {
            template <class Func>
            static std::optional<T> _i_func(const mc_particle::svg::dim2_parameterized_dim1 &param, Func &&func) {
                using _type = type_extract<T, std::remove_cvref_t<Func>, is_a_funcp, std::is_const_v<Func>>;
                if (_type::value == param.type()) {
                    const auto &ref = dynamic_cast<const typename _type::_use_type &>(param);
                    return static_cast<T>(func(ref));
                }
                else {
                    return std::nullopt;
                }
            }
        };
```

- **功能**：提供默认行为，当没有其他特化匹配时使用。
- **继承**：`std::false_type` 表示当前特化不满足条件。
- **返回类型**：`std::optional<T>`，表示可能返回 `T` 类型的值，也可能不返回（`std::nullopt`）。

#### 2. 第二个特化

```cpp
        template <class Function, class F>
        struct ptr0_obj1<Function, void, F> : std::false_type {
            template <class Func>
            static std::optional<bool> _i_func(const mc_particle::svg::dim2_parameterized_dim1 &param, Func &&func) {
                using _type = type_extract<void, std::remove_cvref_t<Func>, is_a_funcp, std::is_const_v<Func>>;
                if (_type::value == param.type()) {
                    const auto &ref = dynamic_cast<const typename _type::_use_type &>(param);
                    func(ref);
                    return true;
                }
                else {
                    return std::nullopt;
                }
            }
        };
```

- **功能**：处理返回类型为 `void` 的情况。
- **继承**：`std::false_type`，表示不满足特定条件。
- **返回类型**：`std::optional<bool>`，表示函数是否成功执行。

#### 3. 第三个特化

```cpp
        template <class Function, class T>
        struct ptr0_obj1<Function, T, std::enable_if_t<std::is_class_v<std::remove_cvref_t<Function>>, void>> : std::true_type {
            template <class Func>
            static std::optional<T> _i_func(const mc_particle::svg::dim2_parameterized_dim1 &param, Func &&func) {
                using _type = type_extract<T, std::remove_cvref_t<Func>, is_a_object, std::is_const_v<Func>>;
                if (_type::value == param.type()) {
                    const auto &ref = dynamic_cast<const typename _type::_use_type &>(param);
                    return static_cast<T>(func(ref));
                }
                else {
                    return std::nullopt;
                }
            }
        };
```

- **功能**：处理 `Function` 为类类型且返回类型非 `void` 的情况。
- **继承**：`std::true_type`，表示满足条件。
- **返回类型**：`std::optional<T>`，表示可能返回 `T` 类型的值。

#### 4. 第四个特化

```cpp
        template <class Function>
        struct ptr0_obj1<Function, void, std::enable_if_t<std::is_class_v<std::remove_cvref_t<Function>>, void>> : std::true_type {
            template <class Func>
            static std::optional<bool> _i_func(const mc_particle::svg::dim2_parameterized_dim1 &param, Func &&func) {
                using _type = type_extract<void, std::remove_cvref_t<Func>, is_a_object, std::is_const_v<Func>>;
                if (_type::value == param.type()) {
                    const auto &ref = dynamic_cast<const typename _type::_use_type &>(param);
                    func(ref);
                    return true;
                }
                else {
                    return std::nullopt;
                }
            }
        };
```

- **功能**：处理 `Function` 为类类型且返回类型为 `void` 的情况。
- **继承**：`std::true_type`，表示满足条件。
- **返回类型**：`std::optional<bool>`，表示函数是否成功执行。

## `checkout_valid_function` 函数模板总结

```cpp
template <class T, class Func, class... Funcs>
T mc_particle::svg::dim2_parameterized_dim1::checkout_valid_function(const dim2_parameterized_dim1 &param, Func &&func, Funcs &&...funcs) {
    auto &&ret = _details::ptr0_obj1<std::remove_cvref_t<Func>, T>::_i_func(param, std::forward<Func &&>(func));
    if (ret.has_value()) {
        if constexpr (std::is_void_v<T>) {
            return;
        }
        else {
            return *ret;
        }
    }
    else {
        checkout_valid_function<T>(param, std::forward<Funcs &&>(funcs)...);
    }
}

template <class T, class Func>
T mc_particle::svg::dim2_parameterized_dim1::checkout_valid_function(const dim2_parameterized_dim1 &param, Func &&func) {
    auto &&ret = _details::ptr0_obj1<std::remove_cvref_t<Func>, T>::_i_func(param, std::forward<Func &&>(func));
    if (ret.has_value()) {
        if constexpr (std::is_void_v<T>) {
            return;
        }
        else {
            return *ret;
        }
    }
    else {
        throw std::runtime_error("Can't Invoke Any Function!");
    }
}
```

1. **带有递归参数的版本**  
   - **逻辑流程**：若 `_i_func` 处理成功则返回，否则递归调用下一个函数。

#### 特点与工作原理

- **模板参数**
  - `T`：期望的返回类型。
  - `Func`：当前尝试调用的可调用对象类型。
  - `Funcs...`：余下的可调用对象类型，用于递归调用。

- **参数**
  - `const dim2_parameterized_dim1 &param`：需要处理的参数对象。
  - `Func &&func`：当前尝试调用的可调用对象。
  - `Funcs &&...funcs`：余下的可调用对象，用于递归调用。

2. **基础（非递归）版本**  
   - **逻辑流程**：若 `_i_func` 处理成功则返回，否则抛出异常。

- **模板参数**
  - `T`：期望的返回类型。
  - `Func`：当前尝试调用的可调用对象类型。

- **参数**
  - `const dim2_parameterized_dim1 &param`：需要处理的参数对象。
  - `Func &&func`：当前尝试调用的可调用对象。

---
