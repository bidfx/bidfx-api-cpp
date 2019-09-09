/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#ifndef PUBLIC_API_CPP_PRICE_PIXIE_MESSAGES_PRICE_EVENT_H_
#define PUBLIC_API_CPP_PRICE_PIXIE_MESSAGES_PRICE_EVENT_H_

namespace bidfx_public_api::price::pixie
{

class PriceEvent
{
private:

public:
    virtual std::string ToString() = 0;
    virtual uint32_t GetSid() = 0;
    virtual std::map<std::string, PriceField> GetPrice() = 0;

    virtual std::string GetType() = 0;
};

}
#endif //PUBLIC_API_CPP_PRICE_PIXIE_MESSAGES_PRICE_EVENT_H_
