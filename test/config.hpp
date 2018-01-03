#pragma once

#ifdef SVBB_USE_STX_STRING_VIEW
#include "stx/string_view.hpp"
#include "stx/optional.hpp"
#define SVBB_STRING_VIEW_IMPL stx
#define SVBB_OPTIONAL_IMPL stx
#elif defined SVBB_USE_BOOST_STRING_VIEW
#include "boost/utility/string_view.hpp"
#include "boost/optional.hpp"
#include "boost/optional/optional_io.hpp"
#define SVBB_STRING_VIEW_IMPL boost
#define SVBB_OPTIONAL_IMPL boost
#endif
