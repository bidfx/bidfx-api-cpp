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

#include "src/price/provider/pixie/pixie_version.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::pixie
{

TEST(PixieVersionTest, TheCurrentServerWillNegotiateVersion_1_WithAClientOnVersion_1)
{
    ASSERT_EQ(1, PixieVersion::ServerNegotiateVersion(1));
}

TEST(PixieVersionTest, TheCurrentServerWillNegotiateVersion_2_WithAClientOnVersion_2)
{
    ASSERT_EQ(2, PixieVersion::ServerNegotiateVersion(2));
}

TEST(PixieVersionTest, TheCurrentServerWillNegotiateVersion_3_WithAClientOnVersion_3)
{
    ASSERT_EQ(3, PixieVersion::ServerNegotiateVersion(3));
}

TEST(PixieVersionTest, AFutureServerOnVersions_3_to_6_WillNegotiateVersion_4_WithAClientOnVersion_4)
{
    ASSERT_EQ(4, PixieVersion::ServerNegotiateVersion(4, 3, 6));
}

TEST(PixieVersionTest, AFutureServerOnVersions_3_to_6_WillNegotiateVersion_6_WithAClientOnVersion_2)
{
    ASSERT_EQ(6, PixieVersion::ServerNegotiateVersion(2, 3, 6));
}

TEST(PixieVersionTest, AFutureServerOnVersions_3_to_6_WillNegotiateVersion_6_WithAClientOnVersion_9)
{
    ASSERT_EQ(6, PixieVersion::ServerNegotiateVersion(9, 3, 6));
}

TEST(PixieVersionTest, TheCurrentClientWillAcceptAServerThatNegotiatesVersion_1)
{
    PixieVersion::ClientVerifyVersion(1);
}

TEST(PixieVersionTest, TheCurrentClientWillAcceptAServerThatNegotiatesVersion_2)
{
    PixieVersion::ClientVerifyVersion(2);
}

TEST(PixieVersionTest, TheCurrentClientWillAcceptAServerThatNegotiatesVersion_3)
{
    PixieVersion::ClientVerifyVersion(3);
}

TEST(PixieVersionTest, TheCurrentClientWillAcceptAServerThatNegotiatesVersion_4)
{
    PixieVersion::ClientVerifyVersion(4);
}

TEST(PixieVersionTest, TheCurrentClientWillRejectAServerThatNegotiatesVersion_5)
{
    try
    {
        PixieVersion::ClientVerifyVersion(5);
        FAIL();
    }
    catch (std::invalid_argument &e)
    {

    }
}

TEST(PixieVersionTest, FutureClientOnVersions_4_to_6_WillAcceptAServerThatNegotiatesVersion_4)
{
    PixieVersion::ClientVerifyVersion(4, 4, 6);
}

TEST(PixieVersionTest, FutureClientOnVersions_4_to_6_WillAcceptAServerThatNegotiatesVersion_5)
{
    PixieVersion::ClientVerifyVersion(5, 4, 6);
}

TEST(PixieVersionTest, FutureClientOnVersions_4_to_6_WillAcceptAServerThatNegotiatesVersion_6)
{
    PixieVersion::ClientVerifyVersion(6, 4, 6);
}

TEST(PixieVersionTest, FutureClientOnVersions_4_to_6_WillRejectAServerThatNegotiatesVersion_3)
{
    try
    {
        PixieVersion::ClientVerifyVersion(3, 4, 6);
        FAIL();
    }
    catch (std::invalid_argument &e)
    {

    }
}

TEST(PixieVersionTest, TestClientRejectMessagesForAClientThatSupportsJustOneVersion)
{
    try
    {
        PixieVersion::ClientVerifyVersion(3, 1, 1);
        FAIL();
    }
    catch (std::invalid_argument &e)
    {
        ASSERT_STREQ("a server negotiating version 3 is incompatible with client on pixie version 1\n", e.what());
    }
}

TEST(PixieVersionTest, TestClientRejectMessagesForAClientThatSupportsARangeOfVersions)
{
    try
    {
        PixieVersion::ClientVerifyVersion(8, 1, 4);
        FAIL();
    }
    catch (std::invalid_argument &e)
    {
        ASSERT_STREQ("a server negotiating version 8 is incompatible with client on pixie version 1 to 4\n", e.what());
    }
}


}