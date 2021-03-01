// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/plugin.h>
/* *********************************************** */

//==========================================================
// Macro for export / import
//==========================================================

#if !defined(PHANTOM_STATIC_LINK_PHANTOM_CODE)
#    if defined(PHANTOM_LIB_PHANTOM_CODE)
#        define PHANTOM_EXPORT_PHANTOM_CODE PHANTOM_VISIBILITY_EXPORT
#    else
#        define PHANTOM_EXPORT_PHANTOM_CODE PHANTOM_VISIBILITY_IMPORT
#    endif

#else // defined(PHANTOM_STATIC_LINK_PHANTOM_CODE)
#    define PHANTOM_EXPORT_PHANTOM_CODE

#endif // !defined(PHANTOM_STATIC_LINK_PHANTOM_CODE)

//==========================================================
// Deactivation of warning 4251
// "T1 needs a DLL interface to be used by T2 class clients"
//==========================================================

#ifdef _MSC_VER
#    pragma warning(disable : 4251)
#endif

#if !defined(PHANTOM_CODE_CUSTOM_USE_FLEX_BISON_GRAMMAR)
#    define PHANTOM_CODE_CUSTOM_USE_FLEX_BISON_GRAMMAR 1
#endif
