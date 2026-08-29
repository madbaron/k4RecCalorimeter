#ifndef BASECELLENERGYCALIBRATORSILICON_H
#define BASECELLENERGYCALIBRATORSILICON_H 1

#include "BaseCellEnergyCalibrator.h"

/** === BaseCellEnergyCalibratorSilicon algorithm === <br>
    calibration of digitised silicon calorimeter hits
    D.Jeans 02/2016.

    24 March 2016: removed gap corrections - to be put into separate processor

*/

struct BaseCellEnergyCalibratorSilicon final : BaseCellEnergyCalibrator {

 public:
  BaseCellEnergyCalibratorSilicon(const std::string& name, ISvcLocator* svcLoc);

 protected:
  float reconstructEnergy(const edm4hep::CalorimeterHit& hit, int layer) const override;
} ;

#endif 
