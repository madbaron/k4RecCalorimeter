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
  int getMyUnit() const { return MIP; }
  float convertEnergy(float energy, int inputUnit) const; // convert energy from input to output (MIP) scale
  float digitiseDetectorEnergy(float energy) const;       // apply silicon-specific digitisation

  Gaudi::Property<float> m_ehEnergy{this, "silicon_pairEnergy", 3.6f,
                                    "energy required to create e-h pair in silicon (in eV)"};
};

#endif
