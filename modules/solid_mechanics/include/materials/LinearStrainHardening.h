/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef LINEARSTRAINHARDENING_H
#define LINEARSTRAINHARDENING_H

#include "SolidModel.h"

class LinearStrainHardening;

template <>
InputParameters validParams<LinearStrainHardening>();

class LinearStrainHardening : public SolidModel
{
public:
  LinearStrainHardening(const InputParameters & parameters);
  virtual ~LinearStrainHardening() {}
};

#endif // LINEARSTRAINHARDENING_H
