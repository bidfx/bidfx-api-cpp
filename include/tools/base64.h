//
// Created by fbennett on 03/10/2018.
//

#ifndef PUBLIC_API_CPP_TOOLS__BASE64_H_
#define PUBLIC_API_CPP_TOOLS_BASE64_H_

#include <string>

namespace bidfx_public_api::tools
{

std::string Base64Encode(char const*, unsigned long len);
std::string Base64Decode(std::string const& s);

}

#endif //PUBLIC_API_CPP_TOOLS_BASE64_H_
