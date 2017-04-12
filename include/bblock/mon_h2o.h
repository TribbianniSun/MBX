#ifndef MON_H2O_H
#define MON_H2O_H

#include "monomer.h"


typedef std::set<std::pair<size_t, size_t> > excluded_set_type;

////////////////////////////////////////////////////////////////////////////////

namespace bblock { // Building Block :: h2o

////////////////////////////////////////////////////////////////////////////////

class h2o : public monomer {

  public:
    h2o();
    h2o(double * coords, std::vector<std::string> names);
    ~h2o();
    
    void set_xyz(double * coords);

//  private:

    
};

} // namespace bblock

////////////////////////////////////////////////////////////////////////////////

#endif // MON_H2O_H