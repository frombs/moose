//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "DisplayVolumeFraction.h"

registerMooseObject("PhaseFieldApp", DisplayVolumeFraction);

template <>
InputParameters
validParams<DisplayVolumeFraction>()
{
  InputParameters params = validParams<ElementAverageValue>();
  params.addClassDescription(
      "Postprocessor to calculate the volume fraction of a variable or feature "
      "that is mapped or plotted for visualization purposes. Uses comparison "
      "operators to isolate the feature from the dataset.");
  MooseEnum comparison_operator(
      "equals not_equals greater_than_equals less_than_equals greater_than less_than");
  params.addRequiredParam<MooseEnum>("comparison_operator",
                                     comparison_operator,
                                     "The type of comparison operator to use. Options are: " +
                                         comparison_operator.getRawNames());
  params.addParam<Real>("threshold", 0.0, "The cutoff value to be used for thresholding.");
  return params;
}

DisplayVolumeFraction::DisplayVolumeFraction(const InputParameters & parameters)
  : ElementAverageValue(parameters),

    _comparison_operator(getParam<MooseEnum>("comparison_operator").getEnum<ComparisonOperator>()),
    _threshold(getParam<Real>("threshold"))
{
}

Real
DisplayVolumeFraction::computeQpIntegral()

{
  switch (_comparison_operator)
  {
    case ComparisonOperator::EQUALS:
      if (_u[_qp] == _threshold)
        return 1.0;
      return 0.0;
      break;
    case ComparisonOperator::NOT_EQUALS:
      if (_u[_qp] != _threshold)
        return 1.0;
      return 0.0;
      break;
    case ComparisonOperator::GREATER_THAN_EQUALS:
      if (_u[_qp] >= _threshold)
        return 1.0;
      return 0.0;
      break;
    case ComparisonOperator::LESS_THAN_EQUALS:
      if (_u[_qp] <= _threshold)
        return 1.0;
      return 0.0;
      break;
    case ComparisonOperator::GREATER_THAN:
      if (_u[_qp] > _threshold)
        return 1.0;
      return 0.0;
      break;
    case ComparisonOperator::LESS_THAN:
      if (_u[_qp] < _threshold)
        return 1.0;
      return 0.0;
      break;
    default:
      mooseError("Invalid comparison operator.");
  }
}
