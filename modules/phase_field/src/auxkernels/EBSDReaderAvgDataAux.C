//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "EBSDReaderAvgDataAux.h"
#include "EBSDReader.h"
#include "GrainTrackerInterface.h"
#include "MathUtils.h"

registerMooseObject("PhaseFieldApp", EBSDReaderAvgDataAux);

template <>
InputParameters
validParams<EBSDReaderAvgDataAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<unsigned int>("phase", "The phase to use for all queries.");
  params.addCoupledVar("integrated_index",
                       "The coupled aux variable representing the integrated feature index");
  params.addRequiredParam<UserObjectName>("ebsd_reader", "The EBSDReader GeneralUserObject");
  params.addRequiredParam<UserObjectName>("grain_tracker", "The GrainTracker UserObject");
  MooseEnum field_types = EBSDAccessFunctors::getAvgDataFieldType();
  params.addRequiredParam<MooseEnum>(
      "data_name",
      field_types,
      "The averaged data to be extracted from the EBSD data by this AuxKernel");
  params.addParam<Real>("invalid", -1.0, "Value to return for points without active grains.");
  return params;
}

EBSDReaderAvgDataAux::EBSDReaderAvgDataAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _phase(isParamValid("phase") ? getParam<unsigned int>("phase") : libMesh::invalid_uint),
    _integrated_index(isCoupled("integrated_index") ? coupledValue("integrated_index") : _zero),
    _ebsd_reader(getUserObject<EBSDReader>("ebsd_reader")),
    _grain_tracker(getUserObject<GrainTrackerInterface>("grain_tracker")),
    _data_name(getParam<MooseEnum>("data_name")),
    _val(_ebsd_reader.getAvgDataAccessFunctor(_data_name)),
    _invalid(getParam<Real>("invalid"))
{
}

void
EBSDReaderAvgDataAux::precalculateValue()
{
  /* Get the dominant grain for the current elem/node from the GrainTracker interface
    Note: if integrated_index is used, the returned index needs to be rounded to
    the nearest integer value to remove roundoff errors caused by aux variables
    being stored as doubles instead of integers. */

  const auto grain_id =
      isCoupled("integrated_index")
          ? MathUtils::round(_integrated_index[0])
          : _grain_tracker.getEntityValue(isNodal() ? _current_node->id() : _current_elem->id(),
                                          FeatureFloodCount::FieldType::UNIQUE_REGION,
                                          0);

  // no grain found
  if (grain_id < 0)
    _value = _invalid;

  // get the data for the grain
  else
    _value = _phase != libMesh::invalid_uint ? (*_val)(_ebsd_reader.getAvgData(_phase, grain_id))
                                             : (*_val)(_ebsd_reader.getAvgData(grain_id));
}

Real
EBSDReaderAvgDataAux::computeValue()
{
  return _value;
}
