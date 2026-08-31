// Calorimeter digitiser for the IDC ECAL and HCAL
// For other detectors/models SimpleCaloDigi should be used
#include "DigitiserSiliconMip.h"

#include <stdexcept>
#include <string>

using namespace std;

DECLARE_COMPONENT(DigitiserSiliconMip)

DigitiserSiliconMip::DigitiserSiliconMip(const std::string& name, ISvcLocator* svcLoc) : BaseDigitiser(name, svcLoc) {}

// convert energy from input to output scale (MIP)
float DigitiserSiliconMip::convertEnergy(float energy, EnergyScale inUnit) const {
  // converts input energy to MIP scale
  if (inUnit == EnergyScale::MIP)
    return energy;
  else if (inUnit == EnergyScale::GEVDEP)
    return energy / m_calib_mip;

  throw std::runtime_error("DigitiserSiliconMip::convertEnergy - unknown unit " +
                           std::to_string(static_cast<int>(inUnit)));
}

float DigitiserSiliconMip::digitiseDetectorEnergy(float energy) const {
  // applies extra digitisation to silicon hits
  //  input energy in deposited GeV
  //  output is MIP scale
  float smeared_energy(energy);
  if (m_ehEnergy > 0) {
    // calculate #e-h pairs
    float nehpairs = 1e9 * energy / m_ehEnergy; // check units of energy! _ehEnergy is in eV, energy in GeV
    // fluctuate it by Poisson (actually an overestimate: Fano factor actually makes it smaller, however even this
    // overstimated effect is tiny for our purposes)
    smeared_energy *= m_engine.Poisson(nehpairs) / nehpairs;
  }

  return smeared_energy / m_calib_mip; // convert to MIP units
}
