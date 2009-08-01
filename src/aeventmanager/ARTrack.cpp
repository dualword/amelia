#include "ARTrack.h"

#include <math.h>

//I'm leaving temporarily some of the definition lists that allow the identification of particles. Still not sure if we can use them for reconstructed tracks though

const int ARTrack::codelist[]=
{
    0, 11, 12, 13, 14, 15, 16, 22, 23, 24, 111, 113, 130,
    211, 221, 310, 321, 411, 421, 431, 2112, 2212, 3112, 3122, 3212, 3222,
    3312, 3322, 3334, 4122, -11, -12, -13, -14, -15, -16, -24, -211, -321,
    -411, -421, -431, -2112, -2212, -3112, -3122, -3212, -3222, -3312,
    -3322, -3334, -4122
};

const int ARTrack::chargelist[]=
{
    0, -1, 0, -1, 0, -1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1,
    0, 1, 0, 1, -1, 0, 0, 1, -1, 0, -1, 1, 1, 0, 1, 0, 1, 0, -1, -1, -1, -1, 0,
    -1, 0, -1, 1, 0, 0, -1, 1, 0, 1, -1
};

const QString ARTrack::namelist[]=
    { "0", "e-", "nu_e", "mu-", "nu_mu", "tau-", "nu_tau",
      "gamma", "Z0", "W+", "pi0", "rho0", "K_L0", "pi+", "eta", "K_S0", "K+",
      "D+", "D0", "D_s+", "n0", "p+", "Sigma-", "Lambda0", "Sigma0", "Sigma+",
      "Xi-", "Xi0", "Omega-", "Lambda_c+", "e+", "nu_ebar", "mu+", "nu_mubar",
      "tau+", "nu_taubar", "W-", "pi-", "K-", "D-", "Dbar0", "D_s-", "nbar0",
      "pbar-", "Sigmabar+", "Lambdabar0", "Sigmabar0", "Sigmabar-", "Xibar+",
      "Xibar0", "Omegabar+", "Lambda_cbarminus"
    };


ARTrack::ARTrack():ATrack("Reconstructed Track",ATrack::eRTrack)
{ }

bool ARTrack::isElectron()
{
    return (code == 11 || code == -11);
}

bool ARTrack::isMuon()
{
    return (code == 13 || code == -13);
}

bool ARTrack::isPhoton()
{
    return (code == 22);
}

bool ARTrack::isNeutrino()
{
    return (code == 12 || code == -12 ||
            code == 14 || code == -14 ||
            code == 16 || code == -16);
}

bool ARTrack::isNeutralHadron()
{
    return (code == 23 || code == 111 || code == 113 || code == 130 || code == 221 || code ==310
            || code == 421 || code == -421
            || code == 2212 || code == -2212
            || code == 3122 || code == -3122
            || code == 3212 || code == -3212
            || code == 3322 || code == -3322);
}

bool ARTrack::isChargedHadron()
{
    return (code == 15 || code == -15 ||
            code == 24 || code == -24 ||
            code == 211 || code == -211 ||
            code == 321 || code == -321 ||
            code == 411 || code == -411 ||
            code == 431 || code == -431 ||
            code == 2212 || code == -2212 ||
            code == 3112 || code == -3112 ||
            code == 3222 || code == -3222 ||
            code == 3312 || code == -3312 ||
            code == 3334 || code == -3334 ||
            code == 4122 || code == -4122);
}

bool ARTrack::isHadron()
{
    return isNeutralHadron() || isChargedHadron();
}

void ARTrack::setCode(int _code)
{
    for ( unsigned int j = 0; j < 52; j++ )
    {
        if ( codelist[j] == _code )
        {
            setCharge(chargelist[j]);
            setName(namelist[j]);
        }
    }

    code=_code;
}

int ARTrack::getIndex()
{
    for ( unsigned int j = 0; j < 52; j++ )
    {
        if ( codelist[j] == code )
        {
            return j;
        }
    }
    return 0;
}

std::vector<irr::core::vector3df> ARTrack::getPolyline()
{
    return polyLine;
}



bool ARTrack::isInteresting()
{
    return (isElectron() || isMuon() || isPhoton());
}
