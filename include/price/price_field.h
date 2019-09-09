/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#ifndef PUBLIC_API_CPP_PRICE_PRICE_FIELD_H_
#define PUBLIC_API_CPP_PRICE_PRICE_FIELD_H_

#include <string>
#include "spdlog/fmt/ostr.h"

namespace bidfx_public_api::price
{
class PriceField
{
public:
    enum Type
    {
        INTEGER,
        LONG,
        DOUBLE,
        STRING,
        NOTHING,
    };
private:
    Type type_;

    union
    {
        int32_t int_value_;
        int64_t long_value_;
        double double_value_;
        std::string* string_value_;
    };

    friend std::ostream& operator<<(std::ostream& os, const PriceField& pf);
public:
    static PriceField null;
    PriceField();
    PriceField(const PriceField& to_copy);
    PriceField& operator=(const PriceField& to_copy);
    PriceField(PriceField&& to_move);
    PriceField& operator=(PriceField&& to_move);
    ~PriceField();
    explicit PriceField(int32_t value);
    explicit PriceField(int64_t value);
    explicit PriceField(double value);
    explicit PriceField(std::string value);
    Type GetType() const;
    int32_t GetInt() const;
    int64_t GetLong() const;
    double GetDouble() const;
    std::string GetString() const;
    const bool operator==(const PriceField& pf) const;
};
}

#endif //PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_PRICE_FIELD_H_
