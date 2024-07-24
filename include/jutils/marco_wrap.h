// Copyright © 2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "macro_args_count.h"

#define JUTILS_WRAP_SUPPORTED JUTILS_ARGS_COUNT_SUPPORTED

#if JUTILS_WRAP_SUPPORTED
    #define  JUTILS_WRAP_HELPER0(...)
    #define  JUTILS_WRAP_HELPER1(macro, arg) macro(arg)
    #define  JUTILS_WRAP_HELPER2(macro, arg, ...) macro(arg)  JUTILS_WRAP_HELPER1(macro, __VA_ARGS__)
    #define  JUTILS_WRAP_HELPER3(macro, arg, ...) macro(arg)  JUTILS_WRAP_HELPER2(macro, __VA_ARGS__)
    #define  JUTILS_WRAP_HELPER4(macro, arg, ...) macro(arg)  JUTILS_WRAP_HELPER3(macro, __VA_ARGS__)
    #define  JUTILS_WRAP_HELPER5(macro, arg, ...) macro(arg)  JUTILS_WRAP_HELPER4(macro, __VA_ARGS__)
    #define  JUTILS_WRAP_HELPER6(macro, arg, ...) macro(arg)  JUTILS_WRAP_HELPER5(macro, __VA_ARGS__)
    #define  JUTILS_WRAP_HELPER7(macro, arg, ...) macro(arg)  JUTILS_WRAP_HELPER6(macro, __VA_ARGS__)
    #define  JUTILS_WRAP_HELPER8(macro, arg, ...) macro(arg)  JUTILS_WRAP_HELPER7(macro, __VA_ARGS__)
    #define  JUTILS_WRAP_HELPER9(macro, arg, ...) macro(arg)  JUTILS_WRAP_HELPER8(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER10(macro, arg, ...) macro(arg)  JUTILS_WRAP_HELPER9(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER11(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER10(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER12(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER11(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER13(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER12(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER14(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER13(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER15(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER14(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER16(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER15(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER17(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER16(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER18(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER17(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER19(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER18(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER20(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER19(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER21(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER20(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER22(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER21(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER23(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER22(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER24(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER23(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER25(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER24(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER26(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER25(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER27(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER26(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER28(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER27(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER29(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER28(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER30(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER29(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER31(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER30(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER32(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER31(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER33(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER32(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER34(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER33(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER35(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER34(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER36(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER35(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER37(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER36(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER38(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER37(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER39(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER38(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER40(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER39(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER41(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER40(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER42(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER41(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER43(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER42(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER44(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER43(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER45(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER44(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER46(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER45(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER47(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER46(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER48(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER47(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER49(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER48(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER50(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER49(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER51(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER50(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER52(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER51(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER53(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER52(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER54(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER53(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER55(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER54(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER56(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER55(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER57(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER56(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER58(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER57(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER59(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER58(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER60(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER59(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER61(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER60(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER62(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER61(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER63(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER62(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER64(macro, arg, ...) macro(arg) JUTILS_WRAP_HELPER63(macro, __VA_ARGS__)

    #define JUTILS_WRAP_HELPER_IMPL(bit, macro, ...) JUTILS_WRAP_HELPER ## bit(macro, __VA_ARGS__)
    #define JUTILS_WRAP_HELPER(bit, macro, ...) JUTILS_WRAP_HELPER_IMPL(bit, macro, __VA_ARGS__)
    #define JUTILS_WRAP(macro, ...) JUTILS_WRAP_HELPER(JUTILS_ARGS_COUNT(__VA_ARGS__), macro, __VA_ARGS__)
#endif
