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

#ifndef PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_FACTORY_H_
#define PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_FACTORY_H_

#include <include/user_info.h>
#include <include/tenor.h>
#include <include/tools/date.h>
#include "subject_builder.h"

namespace bidfx_public_api::price::subject
{

using bidfx_public_api::tools::Date;

/**
 * This class provides factory built correctly formatted subjects for commonly subscribed price streams. Price subjects for OTC
 * asset classes, such as FX, can be long and complicated. The challenge for new API users is providing the right subject
 * components, correctly formatted, for each asset class and deal type. The API rightly gives complete freedom (via {@link
 * SubjectBuilder}) to build any subject that the user needs. Using this factory class however helps the new user to get the
 * subjects right for most common use cases.
 * @author Frazer Bennett Wilford
 */
class SubjectFactory
{
private:
    UserInfo* user_info_;
    static const std::string BID_FX_TOP_OF_BOOK_SERVICE;

    static const std::string& ValidateCurrencyPair(const std::string& ccy_pair);
    static const std::string& ValidateCurrency(const std::string& ccy);

public:
    ////////////////////////////////////
    class SubjectCreator
    {
    public:
        virtual Subject CreateSubject() = 0;
    };

    ////////////////////////////////////
    class MandatoryFieldsSubjectCreator : public SubjectCreator
    {
    private:
        std::vector<std::string> mandatory_keys_;

        void VerifyCurrencyAndPair();
        void VerifyMandatoryKeys();

    protected:
        SubjectBuilder subject_builder_ = SubjectBuilder();

        void AddMandatoryKey(const std::string& key);

    public:
        Subject CreateSubject() override;

    };

    ////////////////////////////////////
    class FX : MandatoryFieldsSubjectCreator
    {
    private:
        UserInfo* user_info_;

        void ApplyBasicOTCFields(const std::string& request_type);

    public:
        ////////////////////////////////////
        class FXBase : public SubjectCreator
        {
        private:
            const bool deliverable_;

        protected:
            SubjectFactory::FX& outer_class_;

            void SetLiquidityProvider(const std::string& lp);
            void SetCurrency(const std::string& ccy);
            void SetCurrencyPair(const std::string& ccy_pair);
            void SetQuantity(const std::string& key, double qty);
            void SetBuySideAccount(const std::string& buy_side);
            void SetOnBehalfOf(const std::string& behalf_of);
            void SetTenor(const std::string& tenor_key, const std::string& settlement_key, const Tenor& tenor);
            void SetSettlementDate(const std::string& tenor_key, const std::string& settlement_key, Date* date);
            void SetFixingDate(const std::string& fixing_key, Date* date);

            void SetupMandatoryFields();

        public:
            explicit FXBase(SubjectFactory::FX& outer_class);
            FXBase(SubjectFactory::FX& outer_class, bool deliverable);

            /**
             * Creates a new price subject.
             */
            Subject CreateSubject() override;
        };

        ////////////////////////////////////
        class Spot : public FXBase
        {
        public:
            explicit Spot(SubjectFactory::FX& outer_class);

            /**
             * Sets the liquidity provider to to request dealable pricing from.
             * @param lp the liquidity provider code.
             * @return the ongoing method chaining.
             */
            FX::Spot& LiquidityProvider(const std::string& lp);

            /**
             * Sets the currency pair.
             * @param ccyPair the currency pair as a 6 letter ISO code.
             * @return the ongoing method chaining.
             */
            FX::Spot& CurrencyPair(const std::string& ccy_pair);

            /**
             * Sets the dealt currency code.
             * @param ccy the currency as a 3 letter ISO code.
             * @return the ongoing method chaining.
             */
            FX::Spot& Currency(const std::string& ccy);

            /**
             * Sets the dealt quantity.
             * @param quantity the quantity of dealt currency.
             * @return the ongoing method chaining.
             */
            FX::Spot& Quantity(double quantity);

            /**
             * Sets the buy side account code used for trading FX.
             * @param account the account code.
             * @return the ongoing method chaining.
             */
            FX::Spot& BuySideAccount(const std::string& buy_side_account);

            /**
             * Sets the username for whom trading is being done on behalf of (if permitted).
             * @param username a valid username.
             * @return the ongoing method chaining.
             */
            FX::Spot& OnBehalfOf(const std::string& username);
        };

        ////////////////////////////////////
        class Forward : public FXBase
        {
        private:
            bool deliverable_;
        public:
            explicit Forward(SubjectFactory::FX& outer_class, bool deliverable);

            /**
             * Sets the liquidity provider to to request dealable pricing from.
             * @param lp the liquidity provider code.
             * @return the ongoing method chaining.
             */
            FX::Forward& LiquidityProvider(const std::string& lp);

            /**
             * Sets the currency pair.
             * @param ccyPair the currency pair as a 6 letter ISO code.
             * @return the ongoing method chaining.
             */
            FX::Forward& CurrencyPair(const std::string& ccy_pair);

            /**
             * Sets the dealt currency code.
             * @param ccy the currency as a 3 letter ISO code.
             * @return the ongoing method chaining.
             */
            FX::Forward& Currency(const std::string& ccy);

            /**
             * Sets the dealt quantity.
             * @param quantity the quantity of dealt currency.
             * @return the ongoing method chaining.
             */
            FX::Forward& Quantity(double quantity);

            /**
             * Sets the buy side account code used for trading FX.
             * @param account the account code.
             * @return the ongoing method chaining.
             */
            FX::Forward& BuySideAccount(const std::string& buy_side_account);

            /**
             * Sets the username for whom trading is being done on behalf of (if permitted).
             * @param username a valid username.
             * @return the ongoing method chaining.
             */
            FX::Forward& OnBehalfOf(const std::string& username);

            /**
             * Sets the forward tenor.
             * @param tenor the tenor.
             * @return the ongoing method chaining.
             */
            FX::Forward& Tenor(const bidfx_public_api::Tenor& tenor);

            /**
             * Sets the forward settlement date.
             * @param date a valid future business day.
             * @return the ongoing method chaining.
             */
            FX::Forward& SettlementDate(Date* date);

            /**
             * Sets the forward fixing date for non-deliverable forwards.
             * @param date a valid future business day.
             * @return the ongoing method chaining.
             */
            FX::Forward& FixingDate(Date* date);

        };

        ////////////////////////////////////
        class Swap : public FXBase
        {
        private:
            bool deliverable_;
        public:
            explicit Swap(SubjectFactory::FX& outer_class, bool deliverable);

            /**
             * Sets the liquidity provider to to request dealable pricing from.
             * @param lp the liquidity provider code.
             * @return the ongoing method chaining.
             */
            FX::Swap& LiquidityProvider(const std::string& lp);

            /**
             * Sets the currency pair.
             * @param ccyPair the currency pair as a 6 letter ISO code.
             * @return the ongoing method chaining.
             */
            FX::Swap& CurrencyPair(const std::string& ccy_pair);

            /**
             * Sets the dealt currency code.
             * @param ccy the currency as a 3 letter ISO code.
             * @return the ongoing method chaining.
             */
            FX::Swap& Currency(const std::string& ccy);

            /**
             * Sets the dealt quantity of the near leg.
             * @param quantity the quantity of dealt currency.
             * @return the ongoing method chaining.
             */
            FX::Swap& NearQuantity(double quantity);

            /**
             * Sets the dealt quantity of the far leg.
             * @param quantity the quantity of dealt currency.
             * @return the ongoing method chaining.
             */
            FX::Swap& FarQuantity(double quantity);

            /**
             * Sets the buy side account code used for trading FX.
             * @param account the account code.
             * @return the ongoing method chaining.
             */
            FX::Swap& BuySideAccount(const std::string& buy_side_account);

            /**
             * Sets the username for whom trading is being done on behalf of (if permitted).
             * @param username a valid username.
             * @return the ongoing method chaining.
             */
            FX::Swap& OnBehalfOf(const std::string& username);

            /**
             * Sets the forward tenor of the near leg.
             * @param tenor the tenor.
             * @return the ongoing method chaining.
             */
            FX::Swap& NearTenor(const Tenor& tenor);

            /**
             * Sets the forward tenor of the far leg.
             * @param tenor the tenor.
             * @return the ongoing method chaining.
             */
            FX::Swap& FarTenor(const Tenor& tenor);

            /**
             * Sets the forward settlement date of the near leg.
             * @param date a valid future business day.
             * @return the ongoing method chaining.
             */
            FX::Swap& NearSettlementDate(Date* date);

            /**
             * Sets the forward fixing date for non-deliverable forwards of the near leg.
             * @param date a valid future business day.
             * @return the ongoing method chaining.
             */
            FX::Swap& NearFixingDate(Date* date);

            /**
             * Sets the forward settlement date of the far leg.
             * @param date a valid future business day.
             * @return the ongoing method chaining.
             */
            FX::Swap& FarSettlementDate(Date* date);

            /**
             * Sets the forward fixing date for non-deliverable forwards of the far leg.
             * @param date a valid future business day.
             * @return the ongoing method chaining.
             */
            FX::Swap& FarFixingDate(Date* date);
        };

        ////////////////////////////////////
        class FXDealable
        {
        private:
            SubjectFactory::FX& outer_class_;
        public:
            explicit FXDealable(SubjectFactory::FX& outer_class);

            /**
             * Gets a helper for creating a subject for dealable FX spot prices.
             */
            FX::Spot Spot();

            /**
             * Gets a helper for creating a subject for dealable FX outright forward prices.
             */
            FX::Forward Forward();

            /**
             * Gets a helper for creating a subject for dealable FX non-deliverable forward prices.
             */
            FX::Forward Ndf();

            /**
             * Gets a helper for creating a subject for dealable FX swap prices.
             */
            FX::Swap Swap();

            /**
             * Gets a helper for creating a subject for dealable FX non-deliverable swap prices.
             */
            FX::Swap Nds();
        };

        ////////////////////////////////////
        class FXBook
        {
        private:
            SubjectFactory::FX& outer_class_;
        public:
            explicit FXBook(SubjectFactory::FX& outer_class);
            FX::Spot Spot();
            FX::Forward Forward();
            FX::Forward Ndf();
        };

        // FX Public Variables/Functions
        explicit FX(UserInfo* user_info);

        /**
         * Gets a helper to create a request for stream (RFS) subject.
         */
        FXDealable Stream();

        /**
         * Gets a helper to create a request for quote (RFQ) subject.
         */
        FXDealable Quote();

        /**
         * Gets a helper to create a FX top-of-book subject.
         */
        FXBook Book();

        /**
         * Gets a helper to create a FX top-of-book subject.
         * @param maxRows the maximum number of rows to subscribe to from the top of the book down.
         */
        FXBook Book(int max_rows);
    };

    // Subjects Public Variables/Functions
    explicit SubjectFactory(UserInfo* user_info);

    /**
     * Gets a helper to create an OTC FX subject.
     */
    SubjectFactory::FX Fx();
};

}

#endif //PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_FACTORY_H_
