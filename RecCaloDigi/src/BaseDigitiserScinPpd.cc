// Calorimeter digitiser for the IDC ECAL and HCAL
// For other detectors/models SimpleCaloDigi should be used

#include "BaseDigitiserScinPpd.h"

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <string>

#include "CLHEP/Random/RandBinomial.h"
#include "CLHEP/Random/RandGauss.h"

using namespace std;

DECLARE_COMPONENT(BaseDigitiserScinPpd)

BaseDigitiserScinPpd::BaseDigitiserScinPpd(const std::string& name, ISvcLocator* svcLoc)
    : BaseDigitiser(name, svcLoc) {}

// convert energy from input to output scale (NPE)
float BaseDigitiserScinPpd::convertEnergy(float energy, EnergyScale inUnit) const {
  if (inUnit == EnergyScale::NPE)
    return energy;
  else if (inUnit == EnergyScale::MIP)
    return m_PPD_pe_per_mip * energy;
  else if (inUnit == EnergyScale::GEVDEP)
    return m_PPD_pe_per_mip * energy / m_calib_mip;

  throw std::runtime_error("BaseDigitiserScinPpd::convertEnergy - unknown unit " +
                           std::to_string(static_cast<int>(inUnit)));
}

float BaseDigitiserScinPpd::digitiseDetectorEnergy(float energy) const {
  // input energy in deposited GeV
  // output in npe
  float npe = energy * m_PPD_pe_per_mip / m_calib_mip; // convert to pe scale

  if (m_PPD_n_pixels > 0) {
    // apply average sipm saturation behaviour
    npe = m_PPD_n_pixels * (1.0 - exp(-npe / m_PPD_n_pixels));
    // apply binomial smearing
    float p = npe / m_PPD_n_pixels;             // fraction of hit pixels on SiPM
    npe = m_engine.Binomial(m_PPD_n_pixels, p); // npe now quantised to integer pixels

    if (m_pixSpread > 0) {
      // variations in pixel capacitance
      npe *= m_engine.Gaus(1, m_pixSpread / sqrt(npe));
    }
  }

  return npe;
}
