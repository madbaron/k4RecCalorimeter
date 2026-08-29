#ifndef BASECELLENERGYCALIBRATORSCINPPD_H
#define BASECELLENERGYCALIBRATORSCINPPD_H 1

#include "BaseCellEnergyCalibrator.h"

/** === BaseCellEnergyCalibratorScinPpd algorithm === <br>
    calibration of digitised scint+PPD calorimeter hits
    D.Jeans 02/2016.
*/

struct BaseCellEnergyCalibratorScinPpd final : BaseCellEnergyCalibrator {
public:
  BaseCellEnergyCalibratorScinPpd(const std::string& name, ISvcLocator* svcLoc);

protected:
  float reconstructEnergy(const edm4hep::CalorimeterHit& hit, int layer) const override;

  Gaudi::Property<float> m_PPD_pe_per_mip{this, "ppd_mipPe", 10.0f,
                                          "# Photo-electrons per MIP (scintillator): used to Poisson smear #PEs if >0"};
  Gaudi::Property<int> m_PPD_n_pixels{this, "ppd_npix", 10000,
                                      "Total number of MPPC/SiPM pixels for implementation of saturation effect"};
};

#endif
