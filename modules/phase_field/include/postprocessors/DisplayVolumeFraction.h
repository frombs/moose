//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ElementAverageValue.h"

// Forward Declarations
class DisplayVolumeFraction;

template <>
InputParameters validParams<DisplayVolumeFraction>();

/**
 * Compute the volume fraction of a given variable/feature that will be mapped or plotted for
 * visualization purposes. Uses comparison operators to isolate the feature from the dataset.
 */

class DisplayVolumeFraction : public ElementAverageValue
{
public:
  DisplayVolumeFraction(const InputParameters & parameters);

  virtual Real computeQpIntegral();

protected:
  /// Comparison type
  enum class ComparisonOperator
  {
    EQUALS,
    NOT_EQUALS,
    GREATER_THAN_EQUALS,
    LESS_THAN_EQUALS,
    GREATER_THAN,
    LESS_THAN
  };

  /// Type of comparison to perform
  const ComparisonOperator _comparison_operator;

  // The threshold cuttoff value
  Real _threshold;
};
