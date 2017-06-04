#include "monomer.h"

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: Monomer

////////////////////////////////////////////////////////////////////////////////

Monomer::Monomer() {
}

Monomer::~Monomer() {
}


size_t Monomer::GetNumSites(){ return n_sites;}
size_t Monomer::GetFirstIndex(){ return first_index;}
size_t Monomer::GetNumRealSites(){ return n_real_sites;}
std::string Monomer::GetMonomerId(){ return mon_id;}

void Monomer::GetRealSitesXyz(double * coords) {
  std::copy(xyz.get(), xyz.get() + 3*n_real_sites, coords);
}

void Monomer::GetXyz(double * coords) {
  std::copy(xyz.get(), xyz.get() + 3*n_sites, coords);
}

void Monomer::GetXyzInSys(double * syscoords, double * coords) {
  std::copy(syscoords + 3 * first_index, 
            syscoords + 3 * first_index + 3 * n_sites, coords);
}

void Monomer::SetFirstIndex(size_t n) {
  first_index = n;
}

void Monomer::SetXyz(double * coords) {
  std::copy(coords, coords + n_sites * 3, xyz.get() );
}

void Monomer::GetGrads(double * grads) {
  std::copy(grd.get(),grd.get() + n_sites, grads);
}

std::vector<std::string> Monomer::GetAtomNames() { return at_names;}

////////////////////////////////////////////////////////////////////////////////

} // Building Block :: Monomer

////////////////////////////////////////////////////////////////////////////////
