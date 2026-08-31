#ifndef BASEDIGITISERSILICON_H
#define BASEDIGITISERSILICON_H 1

#include "BaseDigitiser.h"

/** === BaseDigitiserSilicon algorithm === <br>
    digitisation of silicon calorimeter hits
    D.Jeans 02/2016.
*/

struct BaseDigitiserSilicon : public BaseDigitiser {

public:
  BaseDigitiserSilicon(const std::string& name, ISvcLocator* svcLoc);

protected:
  // convert energy from the input scale to the output (MIP) scale
  float convertEnergy(float energy, EnergyScale inputUnit) const override;
  // apply silicon-specific digitisation
  float digitiseDetectorEnergy(float energy) const override;

  Gaudi::Property<float> m_ehEnergy{this, "silicon_pairEnergy", 3.6f,
                                    "energy required to create e-h pair in silicon (in eV)"};
};

#endif
