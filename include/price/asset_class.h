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

#ifndef BIDFX_PUBLIC_API_ASSET_CLASS_H_
#define BIDFX_PUBLIC_API_ASSET_CLASS_H_

#include <string>

namespace bidfx_public_api::price
{

/**
 * This class provides a set of standard constants for the different classes of instrument that are available for subscription via
 * PublicAPI. These constants are provided for use in client applications and servers so as to standardise asset class name usage;
 * they do not represent a complete list of what is available and a price session need not implement them all.
 * @author Frazer Bennett Wilford
 */
class AssetClass
{
public:
    static const std::string CONVERTIBLE;
    static const std::string EQUITY;
    static const std::string FUTURE;
    static const std::string FIXED_INCOME;
    static const std::string FX;
    static const std::string INDEX;
    static const std::string OPTION;
    static const std::string STRATEGY;
    static const std::string WARRANT;
};

}


#endif //BIDFX_PUBLIC_API_ASSET_CLASS_H_
