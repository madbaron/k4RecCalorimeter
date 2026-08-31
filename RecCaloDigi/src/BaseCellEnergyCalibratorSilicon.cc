#include "BaseCellEnergyCalibratorSilicon.h"
#include <algorithm>
#include <iostream>

DECLARE_COMPONENT(BaseCellEnergyCalibratorSilicon)

BaseCellEnergyCalibratorSilicon::BaseCellEnergyCalibratorSilicon(const std::string& name, ISvcLocator* svcLoc)
    : BaseCellEnergyCalibrator(name, svcLoc) {}

float BaseCellEnergyCalibratorSilicon::reconstructEnergy(const edm4hep::CalorimeterHit& hit, int layer) const {
  // here the input energy should be in MIPs
  float energy = hit.getEnergy();
  // now correct for sampling fraction
  energy *= getLayerCalib(layer);
  return energy;
}
