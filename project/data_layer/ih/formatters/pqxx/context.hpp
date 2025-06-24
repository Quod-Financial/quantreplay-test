#ifndef SIMULATOR_DATA_LAYER_IH_FORMATTERS_PQXX_CONTEXT_HPP_
#define SIMULATOR_DATA_LAYER_IH_FORMATTERS_PQXX_CONTEXT_HPP_

#include <fmt/format.h>

template<>
class fmt::formatter<Simulator::DataLayer::Pqxx::Context> {
    using Type = Simulator::DataLayer::Pqxx::Context;
    enum class Format : char { Connection = 'c', EntireContext = 'a' };

  public:
    constexpr auto parse(fmt::format_parse_context& _ctx)
        -> decltype(_ctx.begin())
    {
        auto it = _ctx.begin();
        auto const end = _ctx.end();

        if (it != end && *it == static_cast<char>(Format::Connection)) {
            mFormat = Format::Connection;
            it++;
        }
        if (it != end && *it == static_cast<char>(Format::EntireContext)) {
            mFormat = Format::EntireContext;
            it++;
        }
        return it;
    }

    template<typename Context>
    auto format(Type const& _pqxxContext, Context& _fmtContext) const
        -> decltype(_fmtContext.out())
    {
        if (mFormat == Format::Connection) {
            formatConnection(_pqxxContext, _fmtContext);
        } else {
            formatContext(_pqxxContext, _fmtContext);
        }
        return _fmtContext.out();
    }

  private:
    template<typename Context>
    void formatConnection(Type const& _pqxxContext, Context& _fmtContext) const
    {
        fmt::format_to(
            _fmtContext.out(),
            "{}",
            _pqxxContext.getConnectionString()
        );
    }

    template<typename Context>
    void formatContext(Type const& _pqxxContext, Context& _fmtContext) const
    {
        fmt::format_to(_fmtContext.out(), "Pqxx::Context={{ Connection=\"");
        formatConnection(_pqxxContext, _fmtContext);
        fmt::format_to(_fmtContext.out(), "\" }}");
    }

    Format mFormat{Format::EntireContext};
};

#endif // SIMULATOR_DATA_LAYER_IH_FORMATTERS_PQXX_CONTEXT_HPP_
