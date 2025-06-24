#ifndef SIMULATOR_GENERATOR_SRC_VALUES_PRICE_GENERATION_PARAMS_HPP_
#define SIMULATOR_GENERATOR_SRC_VALUES_PRICE_GENERATION_PARAMS_HPP_

#include <cstdint>

namespace Simulator::Generator::Random {

class PriceGenerationParams
{
    struct Params
    {
        double m_priceTickSize { 0. };
        double m_priceSpread { 0. };
        std::uint32_t m_priceTickRange { 0 };
    };

public:

    class Builder
    {
        friend class PriceGenerationParams;

    public:

        void setPriceTickRange(std::uint32_t _pxTickRange) noexcept;

        void setPriceTickSize(double _pxTickSize) noexcept;

        void setPriceSpread(double _pxSpread) noexcept;

    private:

        Params m_specifiedParams;
    };


    PriceGenerationParams() = delete;

    explicit PriceGenerationParams(Builder _builder) noexcept;


    [[nodiscard]]
    std::uint32_t getPriceTickRange() const noexcept;

    [[nodiscard]]
    double getPriceTickSize() const noexcept;

    [[nodiscard]]
    double getPriceSpread() const noexcept;

private:

    Params m_params;
};

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_SRC_VALUES_PRICE_GENERATION_PARAMS_HPP_
