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

#include <random>
#include <iostream>
#include "guid.h"
#include "hex_encoder.h"

namespace bidfx_public_api::tools
{

GUID::GUID()
{
    guid_ = GenerateRandomBytes(NUM_BYTES);
}

GUID::~GUID()
{
    delete guid_;
}

std::string GUID::ToString()
{
    return HexEncoder::EncodeAsHex(guid_, NUM_BYTES);
}

unsigned char* GUID::GenerateRandomBytes(size_t num_bytes)
{
    unsigned char* bytes = new unsigned char[num_bytes];
    std::random_device engine;
    for (int i = 0; i < num_bytes; )
    {
        unsigned bits = engine(); // sizeof(unsigned) * CHAR_BIT random bits
        for (int j = 0; j < sizeof(unsigned); j+= sizeof(unsigned char))
        {
            bytes[i] = ((unsigned char*) &bits)[j];
            i++;
        }
    }
    return bytes;
}

} // namespace bidfx_public_api::tools