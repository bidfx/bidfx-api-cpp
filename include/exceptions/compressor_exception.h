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

#ifndef BIDFX_PUBLIC_API_COMPRESSOR_EXCEPTION_H_
#define BIDFX_PUBLIC_API_COMPRESSOR_EXCEPTION_H_

#include <string>
#include "include/exceptions/bid_fx_exception.h"

namespace bidfx_public_api::exceptions
{

/**
 * This exception is thrown when there is a compressor issue
 * @author Frazer Bennett Wilford
 */
class CompressorException : public BidFXException
{

public:
    CompressorException(std::string&& reason);
    CompressorException(const std::string& reason);
    CompressorException(const CompressorException& to_copy);
    CompressorException(CompressorException&& to_move);

    CompressorException& operator=(const CompressorException& to_copy);
    CompressorException& operator=(CompressorException&& to_move);
};

}

#endif //BIDFX_PUBLIC_API_COMPRESSOR_EXCEPTION_H_
