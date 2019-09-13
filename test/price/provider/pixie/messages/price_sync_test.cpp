/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#include "src/price/provider/pixie/messages/price_sync.h"
#include "src/tools/zlib_buffer_inflator.h"
#include "src/tools/byte_buffer.h"
#include "src/tools/buffer_util.h"
#include "test/tools/varint_test.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::ByteBuffer;
using bidfx_public_api::tools::BufferUtil;
using bidfx_public_api::tools::BufferInflator;
using bidfx_public_api::tools::ZLIBBufferInflator;
using bidfx_public_api::price::pixie::PriceSync;

class PriceSyncTest : public ::testing::Test
{
public:
    ByteBuffer createBuffer()
    {
        return BufferUtil::HexBuffer("010090ae9fb8a029000032c4974d68144914c7bb7b325359bff7c3f5fb7bd5ddd54eeaa3abba5b0838c1382110a23121b87b3289b5abeec97858961c220a464450bc887888b087bd28398a28f87111150f228bec290711c9c1ab594fbeaa99cecc74d77432612487f7e8aeaa79bffa5755bf57232d644d5e7ce43acae5c62efe95797fe3896b2b97853754c8f71fe83a74d4ede8c78c0ace887be4504f9f4bb8b77f701ff108658cfb614802066d3c10fe88a9359036b2ae5f058c72b989c79b33d72f0146b92cbca1ce1a18168afa384eba9c5a9c8011338719399ecc2c480f23243472b8510f0f64d3c2380cfb660e377284cc226beaef677947b9dcc3f36d99a95bcff3b672597843bfb69f186e239e60047346438c3185301883201af2908481db8e47f323a7e61894c7a314eb464aa1d58349cb1cb2ee4e8244e5f4968d3f78ecdacae92d73a844c8ba72a1db01d34b706fbcdb06d30bb04f4d6c58e9094704e79e3748f4342a9b8e91d1c3bd6d155cc1300d6433b25edee70e988efadf036e83e9a85572b9882b1194c6e59a0725e41226bf2a6395d808aba47e49ec92c551bb1459ef66061c30adf6c5a7011b2ca956f064442f8e350f4a6243b9ac023b8dcbd869fc45b1cb91f57e028eb2727a99efde86a3ac5c72a17d320fb47950f22ba272c5e2a15756e65c859e3c07394a398d3e68ce513ca8ca503e163a17f955196ab655c855c87a7a1628cae9b334760d28cad553a8a280223462b0fc3a5dcc7c0bd59c9c6fd2e5ccb750cd728ccb2684fc76417a12852a8ac88d7a0496df2d8c132f54b31cdfc48182b81a59ff5c8642a55cb1c03f8542a55cf11874f4f71ad72d7e0c7cd8330f7b64c4d44ae5f79518550fc71e0146395d0f6b60a888ab290664cc84619e5c93aea6d38c61b8861c6a94039fe8da7439b5388c9af510dfc421815c87ac371f6698a39c3e0513afff67b672c554d07fc4740a421ea3842e211e0e6267bad42ae4fa744ac14c61f15440b00ee8578b895a03b9a132a5aacd29a7d444fd6a704add080aa15abf2955ebb750addf9aaa7590b865f954c4b1e641a66abda98c557a236c426d83b19b1747ed16648d3f84fd55ce50321d22b7ce39621bb26ec0d5144cafd8f4856e1b2cbab86e4feddd81ac3bd07ba774adfd177ac1a2c83fa4f6ee4ceddd95dabbbbfa3a3d05bd53a55eac973b1826434361789c0ff1410a4b4668718593edf2c7722ca5308aa514d61deba706ceebe706ce6b4f03e7b5b7d91afbd8ea80e5262ee732af665a6db0a60f1f5bb3f08e5a44af20bfe11e1c04bc0bd35ff23d7f749efcfdd49fb44ff40e951eda8f971eb074cbd194caca684a699dd15a9a674fabd27913fe668135455a5b7aba3883680347fb7001d34247f769efcc09fddbd31da587e142e901cbd68646c3cd555f526534adb4be68a491d13e030000ffff000000ffff");
    }

    std::shared_ptr<BufferInflator> buffer_inflator = std::make_shared<ZLIBBufferInflator>();
    ByteBuffer buffer_ = createBuffer();
    PriceSync price_sync = PriceSync(buffer_, *buffer_inflator);
};

TEST_F(PriceSyncTest, TestEqualsSelf)
{
    EXPECT_EQ(price_sync, price_sync);
}

TEST_F(PriceSyncTest, TestEqualsSimilar)
{
    ByteBuffer buffer1 = createBuffer();
    EXPECT_EQ(price_sync, PriceSync(buffer1, *buffer_inflator));
}

TEST_F(PriceSyncTest, TestNotEqualsDifferent)
{
    ByteBuffer buffer = createBuffer();
    buffer.SetByte(1, 33);
    EXPECT_NE(price_sync, PriceSync(buffer, *buffer_inflator));
}

TEST_F(PriceSyncTest, TestToString)
{
    EXPECT_EQ("PriceSync(revision=0, revisionTime=1417457162000, conflationLatency=0, edition=0, size=50, compressed=true)",
            price_sync.ToString());
}

TEST_F(PriceSyncTest, TestStreamOverload)
{
    std::stringstream ss;
    ss << price_sync;
    EXPECT_EQ("PriceSync(revision=0, revisionTime=1417457162000, conflationLatency=0, edition=0, size=50, compressed=true)",
              ss.str());
}
} // namespace bidfx_public_api::price::pixie
