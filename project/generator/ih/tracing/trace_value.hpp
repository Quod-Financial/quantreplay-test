#ifndef SIMULATOR_GENERATOR_IH_TRACING_TRACE_VALUE_HPP_
#define SIMULATOR_GENERATOR_IH_TRACING_TRACE_VALUE_HPP_

#include <functional>
#include <string>
#include <string_view>
#include <type_traits>

namespace Simulator::Generator::Trace {

template<typename T>
class TraceValue
{
    using StoredType = std::decay_t<T>;

    static constexpr bool HoldsArithmetic = std::is_arithmetic_v<StoredType>;
    static constexpr bool HoldsString = std::is_same_v<StoredType, std::string>;
    static constexpr bool HoldsStringView = std::is_same_v<StoredType, std::string_view>;

    static_assert(
        HoldsArithmetic || HoldsString || HoldsStringView,
        "Type T is not supported by tracing component. "
        "Only std::string, std::string_view and arithmetic could be traced"
    );

public:

    static constexpr bool holdsString() { return HoldsString; }

    static constexpr bool holdsStringView() { return HoldsStringView; }

    static constexpr bool holdsArithmeticType() { return HoldsArithmetic; }

    TraceValue(
            std::string_view _key
        ,   T const & _value
    ) noexcept;


    [[nodiscard]] std::string_view getKey() const noexcept;

    [[nodiscard]] StoredType const & getValue() const noexcept;


    [[nodiscard]] bool holdsComment() const noexcept;

    [[nodiscard]] std::string_view getComment() const noexcept;

    void setComment(std::string_view _comment) noexcept;

private:

    std::string_view m_key;
    std::string_view m_comment;

    std::reference_wrapper<StoredType const> m_valueReference;
};


template<typename T>
TraceValue<T>::TraceValue(
        std::string_view _key
    ,   T const & _value
) noexcept
    :   m_key { _key }
    ,   m_valueReference { _value }
{}


template<typename T>
std::string_view TraceValue<T>::getKey() const noexcept
{
    return m_key;
}


template<typename T>
typename TraceValue<T>::StoredType const &
    TraceValue<T>::getValue() const noexcept
{
    return m_valueReference.get();
}


template<typename T>
bool TraceValue<T>::holdsComment() const noexcept
{
    return !m_comment.empty();
}


template<typename T>
std::string_view TraceValue<T>::getComment() const noexcept
{
    return m_comment;
}


template<typename T>
void TraceValue<T>::setComment(std::string_view _comment) noexcept
{
    m_comment = _comment;
}

} // namespace Simulator::GeneratorImpl::Trace

#endif // SIMULATOR_GENERATOR_IH_TRACING_TRACE_VALUE_HPP_
