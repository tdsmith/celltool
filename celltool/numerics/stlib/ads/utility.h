// -*- C++ -*-

/*! 
  \file utility.h
  \brief Includes the utility files.
*/

#if !defined(__ads_utility_h__)
#define __ads_utility_h__

#include "utility/ObjectAndBlankSpace.h"
#include "utility/ParseOptionsArguments.h"
#include "utility/string.h"

BEGIN_NAMESPACE_ADS

/*!
  \page ads_utility Utility Package

  The utility sub-package has the following features.
  - The ParseOptionsArguments class parses command line options
  and arguments.
  - \ref ads_utility_string
  - ObjectAndBlankSpace pads an object with black space.  This is useful in
  avoiding false sharing.
*/

END_NAMESPACE_ADS

#endif
