# RecCaloDigi

Calorimeter digitisation and cell energy calibration for sampling calorimeters read out with
silicon or scintillator + PPD (SiPM/MPPC) sensors.

These algorithms are ported from the `CaloDigi/Realistic` family in
[MarlinReco](https://github.com/iLCSoft/MarlinReco/tree/master/CaloDigi/Realistic),
originally by D. Jeans (02/2016) with the charge-integration and timing rewrite by R. Ete
(11/2020).

## Algorithms

The package is organised as two technology-blind base classes with technology-specific
implementations inheriting from them.

| Algorithm | Purpose |
|---|---|
| `BaseDigitiser` | Base digitiser: timing cuts and charge integration, miscalibration, dead cells, electronics noise and dynamic range. `SimCalorimeterHit` → `CalorimeterHit`. |
| `BaseDigitiserSilicon` | Silicon-specific digitisation; output on the MIP scale. |
| `BaseDigitiserScinPpd` | Scintillator + PPD digitisation including SiPM saturation and pixel statistics; output in photo-electrons. |
| `BaseCellEnergyCalibrator` | Base calibration: applies layer-group calibration constants to go from the digitised scale to shower GeV. |
| `BaseCellEnergyCalibratorSilicon` | MIP → GeV calibration. |
| `BaseCellEnergyCalibratorScinPpd` | PPD de-saturation, NPE → MIP → GeV calibration. |

Both stages emit a `CaloHitSimCaloHitLink` collection relating their output hits back to the
originating simulated hits.

`CalorimeterHitType` (`CHT`) is the helper used to encode the calorimeter type, ID, layout and
layer number into the `CalorimeterHit` type field.

## Documentation

The upstream package carries more detailed documentation of the digitisation model at
[MarlinReco `CaloDigi/Realistic/doc`](https://github.com/iLCSoft/MarlinReco/tree/master/CaloDigi/Realistic/doc).
Porting that material is deliberately left to a follow-up pull request.
