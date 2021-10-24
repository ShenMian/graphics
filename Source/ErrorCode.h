// Copyright 2021 SMS
// License(Apache-2.0)

#include <system_error>

extern std::error_code ErrorCode;

enum class Errc
{
	bed_file
};

namespace std
{

template <>
struct is_error_code_enum<Errc> : true_type {};

std::error_code make_error_code(Errc);

}
