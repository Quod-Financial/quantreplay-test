#ifndef SIMULATOR_GENERATOR_SRC_VALUES_QUANTITY_GENERATION_PARAMS_HPP_
#define SIMULATOR_GENERATOR_SRC_VALUES_QUANTITY_GENERATION_PARAMS_HPP_

namespace Simulator::Generator::Random {

class QuantityGenerationParams
{
    struct Params
    {
        double m_qtyMultiplier { 0 };
        double m_minQty { 0. };
        double m_maxQty { 0. };
    };

public:

    class Builder
    {
        friend class QuantityGenerationParams;

    public:

        Builder() = default;

        void setQuantityMultiplier(double _multiplier) noexcept;

        void setMinimalQuantity(double _minQuantity) noexcept;

        void setMaximalQuantity(double _maxQuantity) noexcept;

    private:

        Params m_specifiedParams;
    };


    QuantityGenerationParams() = delete;

    explicit QuantityGenerationParams(Builder _builder);


    [[nodiscard]]
    double getMultiplier() const noexcept;

    [[nodiscard]]
    double getMinimalQuantity() const noexcept;

    [[nodiscard]]
    double getMaximalQuantity() const noexcept;

private:

    static void validate(Params const & _qtyGenerationParams);


    Params m_params;
};

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_SRC_VALUES_QUANTITY_GENERATION_PARAMS_HPP_
