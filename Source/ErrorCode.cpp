// Copyright 2021 SMS
// License(Apache-2.0)

#include "ErrorCode.h"

std::error_code ErrorCode;

namespace std
{

std::error_code make_error_code(Errc code)
{
    return std::error_code();
}

}