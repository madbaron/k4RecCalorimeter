#include "RealisticCaloReco.h"

#include <edm4hep/SimCalorimeterHit.h>
#include <edm4hep/MutableCalorimeterHit.h>
#include <edm4hep/CaloHitSimCaloHitLinkCollection.h>

#include <iostream>
#include <string>
#include <algorithm>
#include <assert.h>
#include <cmath>

using namespace std;

RealisticCaloReco::RealisticCaloReco(const std::string& name, ISvcLocator* svcLoc) : MultiTransformer(name, svcLoc, 
  { KeyValues("inputLinkCollections", {"CaloHitLinks"}) },
  { KeyValues("outputHitCollections", {"CalorimeterHitsRec"}),
   KeyValues("outputRelationCollections", {"CaloHitLinksRec"}) }) {}

StatusCode RealisticCaloReco::initialize() {
  m_geoSvc = serviceLocator()->service("GeoSvc");
  if (!m_geoSvc) {
    error() << "Unable to retrieve the GeoSvc" << endmsg;
    return StatusCode::FAILURE;
  }

  assert ( m_calibrCoeff.size()>0 );
  assert ( m_calibrCoeff.size() == m_calLayers.size() );

  // the cellID encoding does not change during the job, so decode it once here
  m_bitFieldCoder =
      dd4hep::DDSegmentation::BitFieldCoder(m_geoSvc->constantAsString(m_encodingStringVariable.value()));

  return StatusCode::SUCCESS;
}

//-----------------------------------------------------------------------------------------------

std::tuple<edm4hep::CalorimeterHitCollection, 
           edm4hep::CaloHitSimCaloHitLinkCollection> RealisticCaloReco::operator()(
           const edm4hep::CaloHitSimCaloHitLinkCollection& inputLinks) const {
  // * Reading Collections of digitised calorimeter Hits *
  edm4hep::CalorimeterHitCollection newcol;
  edm4hep::CaloHitSimCaloHitLinkCollection relcol;
  debug()  << " number of elements = " << inputLinks.size() << endmsg;

  for (const auto& link : inputLinks) {
      const edm4hep::CalorimeterHit hit = link.getFrom();
      edm4hep::MutableCalorimeterHit calhit = newcol.create(); // make new hit

      const auto cellID = hit.getCellID();
      float energy = reconstructEnergy( hit, m_bitFieldCoder.get(cellID, "layer") ); // overloaded method, technology dependent

      calhit.setCellID(cellID);
      calhit.setEnergy(energy);
      calhit.setTime( hit.getTime() );
      calhit.setPosition( hit.getPosition() );
      calhit.setType( hit.getType() );

      edm4hep::MutableCaloHitSimCaloHitLink newLink = relcol.create();
      newLink.setFrom(calhit);
      newLink.setTo(link.getTo());
      newLink.setWeight(1.0);
  }

  return std::make_tuple(std::move(newcol), std::move(relcol));
}

float RealisticCaloReco::getLayerCalib( int ilayer ) const{
  float calib_coeff = 0;
  // retrieve calibration constants
  // Fixed the following logic (DJeans, June 2016)
  int min(0),max(0);
  for (unsigned int k(0); k < m_calLayers.size(); ++k) {
    if ( k > 0 ) min+=m_calLayers[k-1];
    max+=m_calLayers[k];
    if (ilayer >= min && ilayer < max) {
      calib_coeff = m_calibrCoeff[k];
      break;
    }
  }
  assert( calib_coeff>0 );
  return calib_coeff;
}

