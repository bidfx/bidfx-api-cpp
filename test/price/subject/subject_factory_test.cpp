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

#include "include/price/subject/subject_factory.h"
#include "src/basic_user_info.h"
#include "math.h"
#include "include/price/subject/invalid_subject_exception.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::subject
{

UserInfo* USER_INFO = new BasicUserInfo("ln-tunnel.uatprod.tradingscreen.com", "fbennett", "FX_ACCT", "api_test", "1", "XYZCODE123");

/*****************
 * Spot Tests - Valid Creation
 *****************/

TEST(SubjectFactoryTest, test_setting_fx_spot_stream)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=EUR,DealType=Spot,Level=1,LiquidityProvider=CSFX,Quantity=1000000.00,RequestFor=Stream,Symbol=EURGBP,Tenor=Spot,User=fbennett",
            subject_factory.Fx().Stream().Spot()
                    .CurrencyPair("EURGBP")
                    .Currency("EUR")
                    .Quantity(1000000)
                    .LiquidityProvider("CSFX")
                    .CreateSubject()
                    .ToString().c_str());

    ASSERT_STREQ("AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=GBP,DealType=Spot,Level=1,LiquidityProvider=BARCFX,Quantity=543219.07,RequestFor=Stream,Symbol=GBPUSD,Tenor=Spot,User=fbennett",
                 subject_factory.Fx().Stream().Spot()
                         .Currency("GBP")
                         .Quantity(543219.07)
                         .CurrencyPair("GBPUSD")
                         .LiquidityProvider("BARCFX").CreateSubject().ToString().c_str());
}

TEST(SubjectFactoryTest, test_setting_fx_spot_stream_with_non_default_account)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=METALS_ACC1,Currency=XAU,DealType=Spot,Level=1,LiquidityProvider=RBSFX,Quantity=23000.00,RequestFor=Stream,Symbol=XAUUSD,Tenor=Spot,User=fbennett",
            subject_factory.Fx().Stream().Spot()
                    .BuySideAccount("METALS_ACC1")
                    .CurrencyPair("XAUUSD")
                    .Currency("XAU")
                    .Quantity(23000)
                    .LiquidityProvider("RBSFX")
                    .CreateSubject()
                    .ToString().c_str());
}

TEST(SubjectFactoryTest, test_setting_fx_spot_quote)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=EUR,DealType=Spot,Level=1,LiquidityProvider=CSFX,Quantity=1000000.00,RequestFor=Quote,Symbol=EURGBP,Tenor=Spot,User=fbennett",
            subject_factory.Fx().Quote().Spot()
                    .CurrencyPair("EURGBP")
                    .Currency("EUR")
                    .Quantity(1000000)
                    .LiquidityProvider("CSFX")
                    .CreateSubject()
                    .ToString().c_str());
}

/*****************
 * Spot Tests - Error Handling
 *****************/

TEST(SubjectFactoryTest, test_fx_spot_stream_missing_Quantity)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    try
    {
        subject_factory.Fx().Quote().Spot()
                .CurrencyPair("EURGBP")
                .Currency("EUR")
                .LiquidityProvider("CSFX")
                .CreateSubject()
                .ToString().c_str();
    }
    catch (InvalidSubjectException& e)
    {
        ASSERT_STREQ(e.what(), "subject is incomplete, missing: Quantity");
        return;
    }

    FAIL();
}

TEST(SubjectFactoryTest, test_fx_spot_stream_missing_Quantity_and_currency)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    try
    {
        subject_factory.Fx().Quote().Spot()
                .CurrencyPair("EURGBP")
                .LiquidityProvider("CSFX")
                .CreateSubject()
                .ToString().c_str();
    }
    catch (InvalidSubjectException& e)
    {
        ASSERT_STREQ(e.what(), "subject is incomplete, missing: Currency, Quantity");
        return;
    }

    FAIL();
}

TEST(SubjectFactoryTest, test_fx_spot_stream_currency_must_be_part_of_CurrencyPair)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    try
    {
        subject_factory.Fx().Quote().Spot()
                .CurrencyPair("EURGBP")
                .Currency("MXN")
                .Quantity(1000000)
                .LiquidityProvider("CSFX")
                .CreateSubject()
                .ToString().c_str();
    }
    catch (InvalidSubjectException& e)
    {
        ASSERT_STREQ(e.what(), "currency \"MXN\" is not part of currency pair \"EURGBP\"");
        return;
    }

    FAIL();
}

TEST(SubjectFactoryTest, test_fx_spot_stream_Quantity_must_not_be_negative)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    try
    {
        subject_factory.Fx().Quote().Spot()
                .CurrencyPair("EURGBP")
                .Currency("EUR")
                .Quantity(-1)
                .LiquidityProvider("CSFX")
                .CreateSubject()
                .ToString().c_str();
    }
    catch (InvalidSubjectException& e)
    {
        ASSERT_STREQ(e.what(), "dealt quantity must be a positive value: -1.00");
        return;
    }

    FAIL();
}

TEST(SubjectFactoryTest, test_fx_spot_stream_Quantity_must_not_be_zero)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    try
    {
        subject_factory.Fx().Quote().Spot()
                .CurrencyPair("EURGBP")
                .Currency("EUR")
                .Quantity(0)
                .LiquidityProvider("CSFX")
                .CreateSubject()
                .ToString().c_str();
    }
    catch (InvalidSubjectException& e)
    {
        ASSERT_STREQ(e.what(), "dealt quantity must be a positive value: 0.00");
        return;
    }

    FAIL();
}


/*****************
 * Forward Tests - Valid Creation
 *****************/

TEST(SubjectFactoryTest, test_setting_fx_fwd_stream_with_month_Tenor)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=EUR,DealType=Outright,Level=1,LiquidityProvider=CSFX,Quantity=1000000.00,RequestFor=Stream,Symbol=EURGBP,Tenor=1M,User=fbennett",
            subject_factory.Fx().Stream().Forward()
                    .CurrencyPair("EURGBP")
                    .Currency("EUR")
                    .Quantity(1000000)
                    .LiquidityProvider("CSFX")
                    .Tenor(Tenor::IN_1_MONTH)
                    .CreateSubject()
                    .ToString().c_str());
}

TEST(SubjectFactoryTest, test_setting_fx_fwd_stream_with_broken_date_Tenor)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=EUR,DealType=Outright,Level=1,LiquidityProvider=CSFX,Quantity=3500000.00,RequestFor=Stream,SettlementDate=20180918,Symbol=EURGBP,Tenor=BD,User=fbennett",
            subject_factory.Fx().Stream().Forward()
                    .CurrencyPair("EURGBP")
                    .Currency("EUR")
                    .Quantity(3500000)
                    .LiquidityProvider("CSFX")
                    .SettlementDate(new Date(2018, 9, 18))
                    .Tenor(Tenor::BROKEN_DATE)
                    .CreateSubject()
                    .ToString().c_str());
}

TEST(SubjectFactoryTest, test_setting_fx_fwd_stream_with_Tenor_and_settlement)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=EUR,DealType=Outright,Level=1,LiquidityProvider=CSFX,Quantity=3500000.00,RequestFor=Stream,SettlementDate=20190918,Symbol=EURGBP,Tenor=1Y,User=fbennett",
            subject_factory.Fx().Stream().Forward()
                    .CurrencyPair("EURGBP")
                    .Currency("EUR")
                    .Quantity(3500000)
                    .LiquidityProvider("CSFX")
                    .SettlementDate(new Date(2019, 9, 18))
                    .Tenor(Tenor::IN_1_YEAR)
                    .CreateSubject()
                    .ToString().c_str());
}

/*****************
 * Forward Tests - Error Handling
 *****************/

TEST(SubjectFactoryTest, test_setting_fx_fwd_stream_missing_LP)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    try
    {
        subject_factory.Fx().Stream().Forward()
                .CurrencyPair("EURGBP")
                .Currency("EUR")
                .Quantity(3500000)
                .SettlementDate(new Date(2019, 9, 18))
                .Tenor(Tenor::IN_1_YEAR)
                .CreateSubject()
                .ToString().c_str();
    }
    catch (InvalidSubjectException& e)
    {
        ASSERT_STREQ(e.what(), "subject is incomplete, missing: LiquidityProvider");
        return;
    }

    FAIL();
}

TEST(SubjectFactoryTest, test_setting_fx_fwd_stream_missing_CurrencyPair)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    try
    {
        subject_factory.Fx().Stream().Forward()
                .Currency("EUR")
                .Quantity(3500000)
                .SettlementDate(new Date(2019, 9, 18))
                .LiquidityProvider("CSFX")
                .Tenor(Tenor::IN_1_YEAR)
                .CreateSubject()
                .ToString().c_str();
    }
    catch (InvalidSubjectException& e)
    {
        ASSERT_STREQ(e.what(), "subject is incomplete, missing: Symbol");
        return;
    }

    FAIL();
}

TEST(SubjectFactoryTest, test_setting_fx_fwd_stream_missing_Quantity)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    try
    {
        subject_factory.Fx().Stream().Forward()
                .CurrencyPair("EURGBP")
                .Currency("EUR")
                .SettlementDate(new Date(2019, 9, 18))
                .LiquidityProvider("CSFX")
                .Tenor(Tenor::IN_1_YEAR)
                .CreateSubject()
                .ToString().c_str();
    }
    catch (InvalidSubjectException& e)
    {
        ASSERT_STREQ(e.what(), "subject is incomplete, missing: Quantity");
        return;
    }

    FAIL();
}

TEST(SubjectFactoryTest, test_setting_fx_fwd_stream_missing_broken_SettlementDate)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    try
    {
        subject_factory.Fx().Stream().Forward()
                .CurrencyPair("EURGBP")
                .Currency("EUR")
                .Quantity(3500000)
                .LiquidityProvider("CSFX")
                .Tenor(Tenor::BROKEN_DATE)
                .CreateSubject()
                .ToString().c_str();
    }
    catch (InvalidSubjectException& e)
    {
        ASSERT_STREQ(e.what(), "subject is incomplete, missing: SettlementDate");
        return;
    }

    FAIL();
}

/*****************
 * Swap Tests - Valid Creation
 *****************/

TEST(SubjectFactoryTest, test_setting_fx_swap_stream_with_month_Tenor)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=EUR,DealType=Swap,FarCurrency=EUR,FarQuantity=1000000.00,FarTenor=3M,Level=1,LiquidityProvider=CSFX,Quantity=1000000.00,RequestFor=Stream,Symbol=EURGBP,Tenor=1M,User=fbennett",
            subject_factory.Fx().Stream().Swap()
                    .LiquidityProvider("CSFX")
                    .CurrencyPair("EURGBP")
                    .Currency("EUR")
                    .NearQuantity(1000000)
                    .NearTenor(Tenor::IN_1_MONTH)
                    .FarQuantity(1000000)
                    .FarTenor(Tenor::IN_3_MONTHS)
                    .CreateSubject()
                    .ToString().c_str());
}

TEST(SubjectFactoryTest, test_setting_fx_swap_stream_with_broken_date_Tenor)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=EUR,DealType=Swap,FarCurrency=EUR,FarQuantity=3500000.00,FarSettlementDate=20181218,FarTenor=BD,Level=1,LiquidityProvider=CSFX,Quantity=3500000.00,RequestFor=Stream,SettlementDate=20180918,Symbol=EURGBP,Tenor=BD,User=fbennett",
            subject_factory.Fx().Stream().Swap()
                    .LiquidityProvider("CSFX")
                    .CurrencyPair("EURGBP")
                    .Currency("EUR")
                    .NearQuantity(3500000)
                    .NearSettlementDate(new Date(2018, 9, 18))
                    .NearTenor(Tenor::BROKEN_DATE)
                    .FarQuantity(3500000)
                    .FarSettlementDate(new Date(2018, 12, 18))
                    .FarTenor(Tenor::BROKEN_DATE)
                    .CreateSubject()
                    .ToString().c_str());
}

TEST(SubjectFactoryTest, test_setting_fx_swap_stream_with_broken_date_as_default)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=EUR,DealType=Swap,FarCurrency=EUR,FarQuantity=3500000.00,FarSettlementDate=20181018,FarTenor=BD,Level=1,LiquidityProvider=CSFX,Quantity=3500000.00,RequestFor=Stream,SettlementDate=20180918,Symbol=EURGBP,Tenor=BD,User=fbennett",
            subject_factory.Fx().Stream().Swap()
                    .LiquidityProvider("CSFX")
                    .CurrencyPair("EURGBP")
                    .Currency("EUR")
                    .NearQuantity(3500000)
                    .NearSettlementDate(new Date(2018, 9, 18))
                    .FarQuantity(3500000)
                    .FarSettlementDate(new Date(2018, 10, 18))
                    .CreateSubject()
                    .ToString().c_str());
}

TEST(SubjectFactoryTest, test_setting_fx_swap_stream_with_Tenor_and_settlement)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=EUR,DealType=Swap,FarCurrency=EUR,FarQuantity=3500000.00,FarSettlementDate=20181223,FarTenor=3M,Level=1,LiquidityProvider=CSFX,Quantity=3500000.00,RequestFor=Stream,SettlementDate=20180915,Symbol=EURGBP,Tenor=3W,User=fbennett",
            subject_factory.Fx().Stream().Swap()
                    .LiquidityProvider("CSFX")
                    .CurrencyPair("EURGBP")
                    .Currency("EUR")
                    .NearQuantity(3500000)
                    .NearSettlementDate(new Date(2018, 9, 15))
                    .NearTenor(Tenor::IN_3_WEEKS)
                    .FarQuantity(3500000)
                    .FarSettlementDate(new Date(2018, 12, 23))
                    .FarTenor(Tenor::IN_3_MONTHS)
                    .CreateSubject()
                    .ToString().c_str());
}

/*****************
 * Swap Tests - Error Handling
 *****************/

TEST(SubjectFactoryTest, test_setting_fx_swap_stream_with_missing_LP)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    try
    {
        subject_factory.Fx().Stream().Swap().
                CurrencyPair("EURGBP")
                .Currency("EUR")
                .NearQuantity(3500000)
                .NearSettlementDate(new Date(2018, 9, 15))
                .NearTenor(Tenor::IN_3_WEEKS)
                .FarQuantity(3500000)
                .FarSettlementDate(new Date(2018, 12, 23))
                .FarTenor(Tenor::IN_3_MONTHS)
                .CreateSubject()
                .ToString().c_str();
    }
    catch (InvalidSubjectException& e)
    {
        ASSERT_STREQ(e.what(), "subject is incomplete, missing: LiquidityProvider");
        return;
    }

    FAIL();
}

TEST(SubjectFactoryTest, test_setting_fx_swap_stream_with_missing_CurrencyPair)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    try
    {
        subject_factory.Fx().Stream().Swap()
                .LiquidityProvider("CSFX")
                .Currency("EUR")
                .NearQuantity(3500000)
                .NearSettlementDate(new Date(2018, 9, 15))
                .NearTenor(Tenor::IN_3_WEEKS)
                .FarQuantity(3500000)
                .FarSettlementDate(new Date(2018, 12, 23))
                .FarTenor(Tenor::IN_3_MONTHS)
                .CreateSubject()
                .ToString().c_str();
    }
    catch (InvalidSubjectException& e)
    {
        ASSERT_STREQ(e.what(), "subject is incomplete, missing: Symbol");
        return;
    }

    FAIL();
}


TEST(SubjectFactoryTest, test_setting_fx_swap_stream_with_missing_FarQuantity_and_Tenor)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    try
    {
        subject_factory.Fx().Stream().Swap()
                .LiquidityProvider("CSFX")
                .CurrencyPair("EURGBP")
                .Currency("EUR")
                .NearQuantity(3500000)
                .NearSettlementDate(new Date(2018, 9, 15))
                .NearTenor(Tenor::IN_3_WEEKS)
                .CreateSubject()
                .ToString().c_str();
    }
    catch (InvalidSubjectException& e)
    {
        ASSERT_STREQ(e.what(), "subject is incomplete, missing: FarQuantity, FarTenor");
        return;
    }

    FAIL();
}


TEST(SubjectFactoryTest, test_setting_fx_swap_stream_with_missing_far_leg)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    try
    {
        subject_factory.Fx().Stream().Swap()
                .LiquidityProvider("CSFX")
                .CurrencyPair("EURGBP")
                .Currency("EUR")
                .NearQuantity(3500000)
                .NearSettlementDate(new Date(2018, 9, 15))
                .NearTenor(Tenor::IN_3_WEEKS)
                .CreateSubject()
                .ToString().c_str();
    }
    catch (InvalidSubjectException& e)
    {
        ASSERT_STREQ(e.what(), "subject is incomplete, missing: FarQuantity, FarTenor");
        return;
    }

    FAIL();
}

/*****************
 * On Behalf - Valid Creation
 *****************/

TEST(SubjectFactoryTest, test_setting_on_behalf_fx_spot_stream)
{
    UserInfo* strategy_user = new BasicUserInfo("ln-tunnel.uatprod.tradingscreen.com", "strategy", "FX_ACCT", "api_test", "1", "XYZCODE123");
    SubjectFactory subject_factory = SubjectFactory(strategy_user);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=EUR,DealType=Spot,Level=1,LiquidityProvider=CSFX,OnBehalfOf=fbennett,Quantity=1000000.00,RequestFor=Stream,Symbol=EURGBP,Tenor=Spot,User=strategy",
            subject_factory.Fx().Stream().Spot()
                    .LiquidityProvider("CSFX")
                    .CurrencyPair("EURGBP")
                    .Currency("EUR")
                    .Quantity(1000000)
                    .OnBehalfOf("fbennett")
                    .CreateSubject()
                    .ToString().c_str());

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=EUR,DealType=Spot,Level=1,LiquidityProvider=CSFX,OnBehalfOf=psweeny,Quantity=1000000.00,RequestFor=Stream,Symbol=EURGBP,Tenor=Spot,User=strategy",
            subject_factory.Fx().Stream().Spot()
                    .LiquidityProvider("CSFX")
                    .CurrencyPair("EURGBP")
                    .Currency("EUR")
                    .Quantity(1000000)
                    .OnBehalfOf("psweeny")
                    .CreateSubject()
                    .ToString().c_str());
}

/*****************
 * NDS Tests - Valid Creation
 *****************/

TEST(SubjectFactoryTest, test_setting_nds_with_Tenors)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=USD,DealType=NDS,FarCurrency=USD,FarQuantity=850000.00,FarTenor=3M,Level=1,LiquidityProvider=CSFX,Quantity=1000000.00,RequestFor=Stream,Symbol=USDCNY,Tenor=1M,User=fbennett",
            subject_factory.Fx().Stream().Nds()
                    .LiquidityProvider("CSFX")
                    .CurrencyPair("USDCNY")
                    .Currency("USD")
                    .NearQuantity(1000000)
                    .NearTenor(Tenor::IN_1_MONTH)
                    .FarQuantity(850000)
                    .FarTenor(Tenor::IN_3_MONTHS)
                    .CreateSubject()
                    .ToString().c_str());
}

TEST(SubjectFactoryTest, test_setting_nds_with_explicit_dates)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=USD,DealType=NDS,FarCurrency=USD,FarFixingDate=20190308,FarQuantity=850000.00,FarSettlementDate=20190310,FarTenor=BD,FixingDate=20180915,Level=1,LiquidityProvider=CSFX,Quantity=1000000.00,RequestFor=Stream,SettlementDate=20180918,Symbol=USDCNY,Tenor=BD,User=fbennett",
            subject_factory.Fx().Stream().Nds()
                    .LiquidityProvider("CSFX")
                    .CurrencyPair("USDCNY")
                    .Currency("USD")
                    .NearQuantity(1000000)
                    .NearTenor(Tenor::BROKEN_DATE)
                    .NearFixingDate(new Date(2018, 9, 15))
                    .NearSettlementDate(new Date(2018, 9, 18))
                    .FarQuantity(850000)
                    .FarTenor(Tenor::BROKEN_DATE)
                    .FarFixingDate(new Date(2019, 3, 8))
                    .FarSettlementDate(new Date(2019, 3, 10))
                    .CreateSubject()
                    .ToString().c_str());
}

/*****************
 * NDF Tests - Valid Creation
 *****************/

TEST(SubjectFactoryTest, test_setting_ndf_with_Tenor)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=EUR,DealType=NDF,Level=1,LiquidityProvider=CSFX,Quantity=1000000.00,RequestFor=Stream,Symbol=EURGBP,Tenor=1M,User=fbennett",
            subject_factory.Fx().Stream().Ndf()
                    .LiquidityProvider("CSFX")
                    .CurrencyPair("EURGBP")
                    .Currency("EUR")
                    .Quantity(1000000)
                    .Tenor(Tenor::IN_1_MONTH)
                    .CreateSubject()
                    .ToString().c_str());
}

TEST(SubjectFactoryTest, test_setting_ndf_with_Tenor_and_settlement)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=EUR,DealType=NDF,Level=1,LiquidityProvider=CSFX,Quantity=1000000.00,RequestFor=Stream,SettlementDate=20180923,Symbol=EURGBP,Tenor=1M,User=fbennett",
            subject_factory.Fx().Stream().Ndf()
                    .LiquidityProvider("CSFX")
                    .CurrencyPair("EURGBP")
                    .Currency("EUR")
                    .Quantity(1000000)
                    .Tenor(Tenor::IN_1_MONTH)
                    .SettlementDate(new Date(2018, 9, 23))
                    .CreateSubject()
                    .ToString().c_str());
}

TEST(SubjectFactoryTest, test_setting_ndf_with_Tenor_and_settlement_and_fixing)
{
    SubjectFactory subject_factory = SubjectFactory(USER_INFO);

    ASSERT_STREQ(
            "AssetClass=Fx,BuySideAccount=FX_ACCT,Currency=EUR,DealType=NDF,FixingDate=20190921,Level=1,LiquidityProvider=CSFX,Quantity=1000000.00,RequestFor=Stream,SettlementDate=20190923,Symbol=EURGBP,Tenor=1Y,User=fbennett",
            subject_factory.Fx().Stream().Ndf()
                    .LiquidityProvider("CSFX")
                    .CurrencyPair("EURGBP")
                    .Currency("EUR")
                    .Quantity(1000000)
                    .Tenor(Tenor::IN_1_YEAR)
                    .SettlementDate(new Date(2019, 9, 23))
                    .FixingDate(new Date(2019, 9, 21))
                    .CreateSubject()
                    .ToString().c_str());
}


}