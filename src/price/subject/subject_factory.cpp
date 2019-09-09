/**  Copyright 2019 BidFX

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

            http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
 */

#include <memory>
#include <sstream>
#include <iomanip>
#include "include/user_info.h"
#include "src/price/request_type.h"
#include "include/price/deal_type.h"
#include "include/price/asset_class.h"
#include "src/price/level.h"
#include "src/price/currencies.h"
#include "include/price/subject/subject_factory.h"
#include "subject_key.h"
#include "include/price/subject/invalid_subject_exception.h"

namespace bidfx_public_api::price::subject 
{

SubjectFactory::SubjectFactory(UserInfo* user_info)
{
    user_info_ = user_info;
}

SubjectFactory::FX SubjectFactory::Fx()
{
    return FX(user_info_);
}

SubjectFactory::FX::FX(UserInfo* user_info)
{
    user_info_ = user_info;
    subject_builder_.SetComponent(SubjectKey::LEVEL, Level::ONE);
    subject_builder_.SetComponent(SubjectKey::ASSET_CLASS, AssetClass::FX);
}

void SubjectFactory::FX::ApplyBasicOTCFields(const std::string& request_type)
{
    subject_builder_.SetComponent(SubjectKey::REQUEST_TYPE, request_type);
    subject_builder_.SetComponent(SubjectKey::USER, user_info_->GetUsername());
    subject_builder_.SetComponent(SubjectKey::BUY_SIDE_ACCOUNT, user_info_->GetDefaultAccount());
}

/*****************************
 *  Mandatory Fields Subject Creator
 */
Subject SubjectFactory::MandatoryFieldsSubjectCreator::CreateSubject()
{
    VerifyMandatoryKeys();
    VerifyCurrencyAndPair();

    return subject_builder_.CreateSubject();
}

void SubjectFactory::MandatoryFieldsSubjectCreator::VerifyCurrencyAndPair()
{
    std::string ccy_pair = subject_builder_.GetComponent(SubjectKey::CURRENCY_PAIR);

    if (!ccy_pair.empty())
    {
        std::string ccy = subject_builder_.GetComponent(SubjectKey::CURRENCY);

        if (!ccy.empty())
        {
            if (!Currencies::IsValidCurrencyAndPair(ccy, ccy_pair))
            {
                throw InvalidSubjectException("currency \"" + ccy + "\" is not part of currency pair \"" + ccy_pair + "\"");
            }
        }
    }
}

void SubjectFactory::MandatoryFieldsSubjectCreator::VerifyMandatoryKeys()
{
    std::stringstream ss;
    bool failed_validation = false;

    for (const std::string& key : mandatory_keys_)
    {
        if (!subject_builder_.ContainsKey(key))
        {
            if (failed_validation)
            {
                ss << ", ";
            }

            ss << key;
            failed_validation = true;
        }
    }

    if (failed_validation)
    {
        throw InvalidSubjectException("subject is incomplete, missing: " + ss.str());
    }
}

void SubjectFactory::MandatoryFieldsSubjectCreator::AddMandatoryKey(const std::string& key)
{
    mandatory_keys_.push_back(key);
}

/*****************************
 *  Quote / Stream Setup
 */
SubjectFactory::FX::FXDealable SubjectFactory::FX::Stream()
{
    ApplyBasicOTCFields(RequestType::STREAM);
    return FXDealable(*this);
}

SubjectFactory::FX::FXDealable SubjectFactory::FX::Quote()
{
    ApplyBasicOTCFields(RequestType::QUOTE);
    return FXDealable(*this);
}


/*****************************
 *  Deal Type Setup
 */
SubjectFactory::FX::FXDealable::FXDealable(SubjectFactory::FX& outer_class) : outer_class_(outer_class)
{
}

SubjectFactory::FX::Spot SubjectFactory::FX::FXDealable::Spot()
{
    return FX::Spot(outer_class_);
}

SubjectFactory::FX::Swap SubjectFactory::FX::FXDealable::Swap()
{
    return FX::Swap(outer_class_, true);
}

SubjectFactory::FX::Swap SubjectFactory::FX::FXDealable::Nds()
{
    return FX::Swap(outer_class_, false);
}

SubjectFactory::FX::Forward SubjectFactory::FX::FXDealable::Forward()
{
    return FX::Forward(outer_class_, true);
}

SubjectFactory::FX::Forward SubjectFactory::FX::FXDealable::Ndf()
{
    return FX::Forward(outer_class_, false);
}

/*****************************
 *  Book Setup
 */
SubjectFactory::FX::FXBook::FXBook(SubjectFactory::FX& outer_class) : outer_class_(outer_class)
{
}

SubjectFactory::FX::Spot SubjectFactory::FX::FXBook::Spot()
{
    return FX::Spot(outer_class_);
}

SubjectFactory::FX::Forward SubjectFactory::FX::FXBook::Forward()
{
    return FX::Forward(outer_class_, true);
}

SubjectFactory::FX::Forward SubjectFactory::FX::FXBook::Ndf()
{
    return FX::Forward(outer_class_, false);
}

/*****************************
 *  FX Base
 */
SubjectFactory::FX::FXBase::FXBase(SubjectFactory::FX& outer_class) : deliverable_(false), outer_class_(outer_class)
{
    SetupMandatoryFields();
}

SubjectFactory::FX::FXBase::FXBase(SubjectFactory::FX& outer_class, bool deliverable ) : deliverable_(deliverable), outer_class_(outer_class)
{
    SetupMandatoryFields();
}

void SubjectFactory::FX::FXBase::FXBase::SetupMandatoryFields()
{
    outer_class_.AddMandatoryKey(SubjectKey::BUY_SIDE_ACCOUNT);
    outer_class_.AddMandatoryKey(SubjectKey::CURRENCY);
    outer_class_.AddMandatoryKey(SubjectKey::CURRENCY_PAIR);
    outer_class_.AddMandatoryKey(SubjectKey::DEAL_TYPE);
    outer_class_.AddMandatoryKey(SubjectKey::QUANTITY);
    outer_class_.AddMandatoryKey(SubjectKey::LIQUIDITY_PROVIDER);
}

Subject SubjectFactory::FX::FXBase::CreateSubject()
{
    return outer_class_.CreateSubject();
}

void SubjectFactory::FX::FXBase::SetLiquidityProvider(const std::string& lp)
{
    outer_class_.subject_builder_.SetComponent(SubjectKey::LIQUIDITY_PROVIDER, lp);
}

void SubjectFactory::FX::FXBase::SetCurrency(const std::string& ccy)
{
    outer_class_.subject_builder_.SetComponent(SubjectKey::CURRENCY, ValidateCurrency(ccy));
}

void SubjectFactory::FX::FXBase::SetCurrencyPair(const std::string& ccy_pair)
{
    outer_class_.subject_builder_.SetComponent(SubjectKey::CURRENCY_PAIR, ValidateCurrencyPair(ccy_pair));
}

void SubjectFactory::FX::FXBase::SetQuantity(const std::string& key, double qty)
{
    std::stringstream ss;

    if (qty <= 0)
    {
        ss << std::fixed << std::setprecision(2) << "dealt quantity must be a positive value: " << qty;
        throw InvalidSubjectException(ss.str());
    }

    ss << std::fixed << std::setprecision(2) << qty;

    std::string qty_string = ss.str();

    outer_class_.subject_builder_.SetComponent(key, ss.str());
}

void SubjectFactory::FX::FXBase::SetBuySideAccount(const std::string& account)
{
    outer_class_.subject_builder_.SetComponent(SubjectKey::BUY_SIDE_ACCOUNT, account);
}

void SubjectFactory::FX::FXBase::SetOnBehalfOf(const std::string& on_behalf_of)
{
    outer_class_.subject_builder_.SetComponent(SubjectKey::ON_BEHALF_OF, on_behalf_of);
}

void SubjectFactory::FX::FXBase::SetTenor(const std::string& tenor_key, const std::string& settlement_key, const Tenor& tenor)
{
    if (tenor.ToString() == Tenor::BROKEN_DATE.ToString() && !outer_class_.subject_builder_.ContainsKey(settlement_key))
    {
        outer_class_.AddMandatoryKey(settlement_key);
    }

    if (tenor_key == SubjectKey::FAR_TENOR)
    {
        if (outer_class_.subject_builder_.ContainsKey(SubjectKey::TENOR))
        {
            std::string near_tenor = outer_class_.subject_builder_.GetComponent(SubjectKey::TENOR);

            if (!Tenor::IsValidNearFarTenor(Tenor::GetTenorFromString(near_tenor), tenor))
            {
                std::stringstream ss;
                ss << "Invalid near/far tenor combination: [TENOR] " <<  near_tenor << " [FAR] " << tenor.ToString();
                throw InvalidSubjectException(ss.str());
            }
        }
    }
    else if (tenor_key == SubjectKey::TENOR)
    {
        if (outer_class_.subject_builder_.ContainsKey(SubjectKey::FAR_TENOR))
        {
            std::string far_tenor = outer_class_.subject_builder_.GetComponent(SubjectKey::FAR_TENOR);

            if (!Tenor::IsValidNearFarTenor(tenor, Tenor::GetTenorFromString(far_tenor)))
            {
                std::stringstream ss;
                ss << "Invalid near/far tenor combination: [TENOR] " <<  tenor.ToString() << " [FAR] " << far_tenor;
                throw InvalidSubjectException(ss.str());
            }
        }
    }
    else
    {
        std::stringstream ss;
        ss << "Invalid tenor key: " <<  tenor_key;
        throw InvalidSubjectException(ss.str());
    }

    outer_class_.subject_builder_.SetComponent(tenor_key, tenor.ToString());
}

void SubjectFactory::FX::FXBase::SetSettlementDate(const std::string& tenor_key, const std::string& settlement_key, Date* date)
{
    if (!outer_class_.subject_builder_.ContainsKey(tenor_key))
    {
        outer_class_.subject_builder_.SetComponent(tenor_key, Tenor::BROKEN_DATE.ToString());
    }

    outer_class_.subject_builder_.SetComponent(settlement_key, date->ToString());
}

void SubjectFactory::FX::FXBase::SetFixingDate(const std::string& fixing_key, Date* date)
{
    outer_class_.subject_builder_.SetComponent(fixing_key, date->ToString());
}

/*****************************
 *  SPOT Deal Type Setup
 */
SubjectFactory::FX::Spot::Spot(SubjectFactory::FX& outer_class) : FXBase(outer_class)
{
    outer_class_.subject_builder_.SetComponent(SubjectKey::DEAL_TYPE, DealType::SPOT);
    outer_class_.subject_builder_.SetComponent(SubjectKey::TENOR, Tenor::SPOT.ToString());
}

SubjectFactory::FX::Spot& SubjectFactory::FX::Spot::LiquidityProvider(const std::string& lp)
{
    SetLiquidityProvider(lp);
    return *this;
}

SubjectFactory::FX::Spot& SubjectFactory::FX::Spot::CurrencyPair(const std::string& ccy)
{
    SetCurrencyPair(ccy);
    return *this;
}

SubjectFactory::FX::Spot& SubjectFactory::FX::Spot::Currency(const std::string& ccy)
{
    SetCurrency(ccy);
    return *this;
}

SubjectFactory::FX::Spot& SubjectFactory::FX::Spot::Quantity(double quantity)
{
    SetQuantity(SubjectKey::QUANTITY, quantity);
    return *this;
}

SubjectFactory::FX::Spot& SubjectFactory::FX::Spot::BuySideAccount(const std::string& account)
{
    SetBuySideAccount(account);
    return *this;
}

SubjectFactory::FX::Spot& SubjectFactory::FX::Spot::OnBehalfOf(const std::string& username)
{
    SetOnBehalfOf(username);
    return *this;
}

/*****************************
 *  Forward/NDF Deal Type Setup
 */

SubjectFactory::FX::Forward::Forward(SubjectFactory::FX& outer_class, bool deliverable) : FXBase(outer_class)
{
    deliverable_ = deliverable;

    if (deliverable)
    {
        outer_class_.subject_builder_.SetComponent(SubjectKey::DEAL_TYPE, DealType::FORWARD);
    }
    else
    {
        outer_class_.subject_builder_.SetComponent(SubjectKey::DEAL_TYPE, DealType::NDF);
    }
}

SubjectFactory::FX::Forward& SubjectFactory::FX::Forward::LiquidityProvider(const std::string& lp)
{
    SetLiquidityProvider(lp);
    return *this;
}

SubjectFactory::FX::Forward& SubjectFactory::FX::Forward::CurrencyPair(const std::string& ccy)
{
    SetCurrencyPair(ccy);
    return *this;
}

SubjectFactory::FX::Forward& SubjectFactory::FX::Forward::Currency(const std::string& ccy)
{
    SetCurrency(ccy);
    return *this;
}

SubjectFactory::FX::Forward& SubjectFactory::FX::Forward::Quantity(double quantity)
{
    SetQuantity(SubjectKey::QUANTITY, quantity);
    return *this;
}

SubjectFactory::FX::Forward& SubjectFactory::FX::Forward::BuySideAccount(const std::string& buy_side_account)
{
    SetBuySideAccount(buy_side_account);
    return *this;
}

SubjectFactory::FX::Forward& SubjectFactory::FX::Forward::OnBehalfOf(const std::string& username)
{
    SetOnBehalfOf(username);
    return *this;
}

SubjectFactory::FX::Forward& SubjectFactory::FX::Forward::Tenor(const bidfx_public_api::Tenor& tenor)
{
    SetTenor(SubjectKey::TENOR, SubjectKey::SETTLEMENT_DATE, tenor);
    return *this;
}

SubjectFactory::FX::Forward& SubjectFactory::FX::Forward::SettlementDate(Date* date)
{
    SetSettlementDate(SubjectKey::TENOR, SubjectKey::SETTLEMENT_DATE, date);
    return *this;
}

SubjectFactory::FX::Forward& SubjectFactory::FX::Forward::FixingDate(Date* date)
{
    if (!deliverable_)
    {
        SetFixingDate(SubjectKey::FIXING_DATE, date);
    }
    return *this;
}


/*****************************
 *  Swap/NDS Deal Type Setup
 */
SubjectFactory::FX::Swap::Swap(SubjectFactory::FX& outer_class, bool deliverable) : FXBase(outer_class)
{
    outer_class_.AddMandatoryKey(SubjectKey::FAR_QUANTITY);
    outer_class_.AddMandatoryKey(SubjectKey::FAR_TENOR);

    deliverable_ = deliverable;

    if (deliverable)
    {
        outer_class_.subject_builder_.SetComponent(SubjectKey::DEAL_TYPE, DealType::SWAP);
    }
    else
    {
        outer_class_.subject_builder_.SetComponent(SubjectKey::DEAL_TYPE, DealType::NDS);
    }
}

SubjectFactory::FX::Swap& SubjectFactory::FX::Swap::LiquidityProvider(const std::string& lp)
{
    SetLiquidityProvider(lp);
    return *this;
}

SubjectFactory::FX::Swap& SubjectFactory::FX::Swap::CurrencyPair(const std::string& ccy)
{
    SetCurrencyPair(ccy);
    return *this;
}

SubjectFactory::FX::Swap& SubjectFactory::FX::Swap::Currency(const std::string& ccy)
{
    outer_class_.subject_builder_.SetComponent(SubjectKey::FAR_CURRENCY, ccy);
    SetCurrency(ccy);
    return *this;
}

SubjectFactory::FX::Swap& SubjectFactory::FX::Swap::NearQuantity(double quantity)
{
    SetQuantity(SubjectKey::QUANTITY, quantity);
    return *this;
}

SubjectFactory::FX::Swap& SubjectFactory::FX::Swap::FarQuantity(double quantity)
{
    SetQuantity(SubjectKey::FAR_QUANTITY, quantity);
    return *this;
}

SubjectFactory::FX::Swap& SubjectFactory::FX::Swap::BuySideAccount(const std::string& account)
{
    SetBuySideAccount(account);
    return *this;
}

SubjectFactory::FX::Swap& SubjectFactory::FX::Swap::OnBehalfOf(const std::string& username)
{
    SetOnBehalfOf(username);
    return *this;
}

SubjectFactory::FX::Swap& SubjectFactory::FX::Swap::NearTenor(const Tenor& tenor)
{
    SetTenor(SubjectKey::TENOR, SubjectKey::SETTLEMENT_DATE, tenor);
    return *this;
}

SubjectFactory::FX::Swap& SubjectFactory::FX::Swap::FarTenor(const Tenor& tenor)
{
    SetTenor(SubjectKey::FAR_TENOR, SubjectKey::FAR_SETTLEMENT_DATE, tenor);
    return *this;
}

SubjectFactory::FX::Swap& SubjectFactory::FX::Swap::NearSettlementDate(Date* date)
{
    SetSettlementDate(SubjectKey::TENOR, SubjectKey::SETTLEMENT_DATE, date);
    return *this;
}
SubjectFactory::FX::Swap& SubjectFactory::FX::Swap::FarSettlementDate(Date* date)
{
    SetSettlementDate(SubjectKey::FAR_TENOR, SubjectKey::FAR_SETTLEMENT_DATE, date);
    return *this;
}

SubjectFactory::FX::Swap& SubjectFactory::FX::Swap::NearFixingDate(Date* date)
{
    if (!deliverable_)
    {
        SetFixingDate(SubjectKey::FIXING_DATE, date);
    }
    return *this;
}

SubjectFactory::FX::Swap& SubjectFactory::FX::Swap::FarFixingDate(Date* date)
{
    if (!deliverable_)
    {
        SetFixingDate(SubjectKey::FAR_FIXING_DATE, date);
    }
    return *this;
}

/*****************************
 *  Validation Utility
 */

const std::string& SubjectFactory::ValidateCurrencyPair(const std::string& ccy_pair) {
    if (Currencies::IsValidCurrencyPair(ccy_pair))
    {
        return ccy_pair;
    }

    throw InvalidSubjectException("symbol \"" + ccy_pair + "\" is not a valid 6 letter currency pair");
}

const std::string& SubjectFactory::ValidateCurrency(const std::string& ccy) {
    if (Currencies::IsValidCurrency(ccy))
    {
        return ccy;
    }

    throw InvalidSubjectException("symbol \"" + ccy + "\" is not a valid ISO 3 letter currency code");
}

}