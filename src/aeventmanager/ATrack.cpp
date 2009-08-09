#include "ATrack.h"
#include <QDebug>

#include <math.h>

const int ATrack::codelist[]=
  {
    0, 11, 12, 13, 14, 15, 16, 22, 23, 24, 111, 113, 130,
    211, 221, 310, 321, 411, 421, 431, 2112, 2212, 3112, 3122, 3212, 3222,
    3312, 3322, 3334, 4122, -11, -12, -13, -14, -15, -16, -24, -211, -321,
    -411, -421, -431, -2112, -2212, -3112, -3122, -3212, -3222, -3312,
    -3322, -3334, -4122
  };

const int ATrack::chargelist[]=
  {
    0, -1, 0, -1, 0, -1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1,
    0, 1, 0, 1, -1, 0, 0, 1, -1, 0, -1, 1, 1, 0, 1, 0, 1, 0, -1, -1, -1, -1, 0,
    -1, 0, -1, 1, 0, 0, -1, 1, 0, 1, -1
  };
  
const QString ATrack::namelist[]=
  { "0", "e-", "nu_e", "mu-", "nu_mu", "tau-", "nu_tau",
    "gamma", "Z0", "W+", "pi0", "rho0", "K_L0", "pi+", "eta", "K_S0", "K+",
    "D+", "D0", "D_s+", "n0", "p+", "Sigma-", "Lambda0", "Sigma0", "Sigma+",
    "Xi-", "Xi0", "Omega-", "Lambda_c+", "e+", "nu_ebar", "mu+", "nu_mubar",
    "tau+", "nu_taubar", "W-", "pi-", "K-", "D-", "Dbar0", "D_s-", "nbar0",
    "pbar-", "Sigmabar+", "Lambdabar0", "Sigmabar0", "Sigmabar-", "Xibar+",
    "Xibar0", "Omegabar+", "Lambda_cbarminus"
  };


ATrack::ATrack(QString name,QString type)
  : AEventObject(name,AEventObject::eTrack),_code(0),_known(false),_charge(0),_trackType(type)
{ }

ATrack::~ATrack()
{ }

bool ATrack::isElectron()
{
  return (_code == 11 || _code == -11);
}

bool ATrack::isMuon()
{
  return (_code == 13 || _code == -13);
}

bool ATrack::isPhoton()
{
  return (_code == 22);
}

bool ATrack::isNeutrino()
{
  return (_code == 12 || _code == -12 || 
	  _code == 14 || _code == -14 ||
	  _code == 16 || _code == -16);
}

bool ATrack::isNeutralHadron()
{
  return (_code == 23 || _code == 111 || _code == 113 || _code == 130 || _code == 221 || _code ==310 
	  || _code == 421 || _code == -421
	  || _code == 2212 || _code == -2212
	  || _code == 3122 || _code == -3122
	  || _code == 3212 || _code == -3212
	  || _code == 3322 || _code == -3322);
}

bool ATrack::isChargedHadron()
{
  return (_code == 15 || _code == -15 ||
	  _code == 24 || _code == -24 ||
	  _code == 211 || _code == -211 ||
	  _code == 321 || _code == -321 ||
	  _code == 411 || _code == -411 ||
	  _code == 431 || _code == -431 ||
	  _code == 2212 || _code == -2212 ||
	  _code == 3112 || _code == -3112 ||
	  _code == 3222 || _code == -3222 ||
	  _code == 3312 || _code == -3312 ||
	  _code == 3334 || _code == -3334 ||
	  _code == 4122 || _code == -4122);
}

bool ATrack::isHadron()
{
  return isNeutralHadron() || isChargedHadron();
}

void ATrack::setCode(int code)
{
  _code=code;

  /* Loop over the pdg -> info map and update.. */
  int idx=index(); //Step 1: Find the entry in the map, 0 is the first "invalid" entry..
  
  // Step2: Update
  setName(namelist[idx]);
  _charge=chargelist[idx];
  _known=true;
}

int ATrack::code()
{
  return _code;
}

int ATrack::index()
{
  for ( unsigned int j = 0; j < 52; j++ )
    if ( codelist[j] == _code )
      return j;
  
  return 0;
}

QString ATrack::trackType()
{
  return _trackType;
}

void ATrack::setEta(float eta)
{
  _eta=eta;
  _theta=2*atan( exp( -_eta ) );
}

void ATrack::setPhi(float phi)
{
  _phi=phi;
}

void ATrack::setCharge(int charge)
{
  _charge=charge;
}

int ATrack::charge()
{
  return _charge;
}

void ATrack::setPt(float pt)
{
  _pt=pt;
  _et=pt; // Massless particle..
}

bool ATrack::isKnown()
{
  return _known;
}

bool ATrack::isInteresting()
{
  if(isKnown())
    return (isElectron() || isMuon() || isPhoton());
  else // All unknown particles are interesting!
    return true;
}
