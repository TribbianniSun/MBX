#include "testutils.h"

#include "bblock/system.h"
#include "setupbromidewater5.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>

constexpr double TOL = 1E-6;

TEST_CASE("Test the system class.") {

    // Create the bromide -- water system
    SETUP_BROMIDE_WATER_5

    // Now we create a system that will be the same as the one read
    bblock::System my_system;

    // Add monomers to the system
    size_t count = 0;
    for (size_t i = 0; i < n_monomers; i++) {
        std::vector<double> xyz(real_coords.begin() + 3 * count, real_coords.begin() + 3 * count + 3 * n_atoms_vector[i]);
        std::vector<std::string> ats(atom_names.begin() + count, atom_names.begin() + count + n_atoms_vector[i]);
        std::string monid = monomer_names[i];
        my_system.AddMonomer(xyz, ats, monid);
        count += n_atoms_vector[i];
    }

    // Initialize the system to fill in the information
    my_system.Initialize();

    SECTION("GetNumMon()") {
        size_t n = my_system.GetNumMon();
        REQUIRE(n == n_monomers);
    }

    SECTION("GetNumRealSites()") {
        size_t n = my_system.GetNumRealSites();
        REQUIRE(n == n_atoms);
    }

    SECTION("GetNumSites()") {
        size_t n = my_system.GetNumSites();
        REQUIRE(n == n_sites);
    }

    SECTION("GetRealAtomNames()") {
        std::vector<std::string> v = my_system.GetRealAtomNames();
        REQUIRE(VectorsAreEqual(v,atom_names));
    }

    SECTION("GetMonNumAt()") {
        for (size_t i = 0; i < n_monomers; i++) {
            size_t n = my_system.GetMonNumAt(i);
            REQUIRE(n == n_atoms_vector[i]);
        }
    }

    SECTION("GetFirstIndex()") {
        for (size_t i = 0; i < n_monomers; i++) {
            size_t n = my_system.GetFirstInd(i);
            REQUIRE(n == first_index[i]);
        }
    }

    SECTION("GetXyz()") {
        std::vector<double> xyz = my_system.GetXyz();
        REQUIRE(VectorsAreEqual(xyz,coords,TOL));
    }

    SECTION("GetRealXyz()") {
        std::vector<double> xyz = my_system.GetRealXyz();
        REQUIRE(VectorsAreEqual(xyz,real_coords,TOL));
    }

    SECTION("GetCharges()") {
        std::vector<double> chg = my_system.GetCharges();
        REQUIRE(VectorsAreEqual(chg,charges,TOL));
    }

    SECTION("GetRealCharges()") {
        std::vector<double> chg = my_system.GetRealCharges();
        REQUIRE(VectorsAreEqual(chg,real_charges,TOL));
    }

    SECTION("GetPolarizabilities()") {
        std::vector<double> polar = my_system.GetPolarizabilities();
        REQUIRE(VectorsAreEqual(polar,pol,TOL));
    }

    SECTION("GetRealPolarizabilities()") {
        std::vector<double> polar = my_system.GetRealPolarizabilities();
        REQUIRE(VectorsAreEqual(polar,real_pol,TOL));
    }

    SECTION("GetPolarizabilityFactors()") {
        std::vector<double> polarfac = my_system.GetPolarizabilityFactors();
        REQUIRE(VectorsAreEqual(polarfac,polfac,TOL));
    }

    SECTION("GetRealPolarizabilityFactors()") {
        std::vector<double> polarfac = my_system.GetRealPolarizabilityFactors();
        REQUIRE(VectorsAreEqual(polarfac,real_polfac,TOL));
    }

    SECTION("SetXyz()") {
        // Get current XYZ
        std::vector<double> xyz = my_system.GetXyz();
        SECTION("Proper behavior of the function") {
            // Modify xyz adding 1 to all coordinates
            for (size_t i =0; i < xyz.size(); i++)
                xyz[i] += 1.0;
            // Setting it
            my_system.SetXyz(xyz);
            // Getting it
            std::vector<double> new_xyz = my_system.GetXyz();
            REQUIRE(VectorsAreEqual(xyz,new_xyz,TOL));
            // Reset xyz to original coordinates
            my_system.SetXyz(coords);
        }

        SECTION("Set non-valid xyz") {
            std::vector<double> v(2, 1.0);
            bool xyz_cannot_be_set = false;
            try {
                my_system.SetXyz(v);
            } catch (CUException &e) {
                xyz_cannot_be_set = true;
            }
            REQUIRE(xyz_cannot_be_set);
        }
    }

    SECTION("SetRealXyz()") {
        // Get current XYZ
        std::vector<double> xyz = my_system.GetRealXyz();
        SECTION("Proper behavior of the function") {
            // Modify xyz adding 1 to all coordinates
            for (size_t i =0; i < xyz.size(); i++)
                xyz[i] += 1.0;
            // Setting it
            my_system.SetRealXyz(xyz);
            // Getting it
            std::vector<double> new_xyz = my_system.GetRealXyz();
            REQUIRE(VectorsAreEqual(xyz,new_xyz,TOL));
            // Reset xyz to original coordinates
            my_system.SetXyz(coords);
        }

        SECTION("Set non-valid xyz") {
            std::vector<double> v(2, 1.0);
            bool xyz_cannot_be_set = false;
            try {
                my_system.SetRealXyz(v);
            } catch (CUException &e) {
                xyz_cannot_be_set = true;
            }   
            REQUIRE(xyz_cannot_be_set);
        }
    }

    SECTION("Initialize()") {
        SECTION("Initialize an already initialized system") {
            bool system_cannot_be_initialized = false;
            try {
                my_system.Initialize();
            } catch (CUException &e) {
                system_cannot_be_initialized = true;
            }
            REQUIRE(system_cannot_be_initialized);
        }
    }

    SECTION("AddMonomer()") {
        SECTION("Add monomer to an initialized system") {
            bool monomer_not_added_to_initialized_system = false;
            try {
                std::vector<double> v(3, 0.0);
                std::string id_v = "cl";
                std::vector<std::string> at_v = {"Cl"};
                my_system.AddMonomer(v, at_v, id_v);
            } catch (CUException &e) {
                monomer_not_added_to_initialized_system = true;
            }
            REQUIRE(monomer_not_added_to_initialized_system);
        }
    }

    SECTION("AddMonomerInfo()") {
        SECTION("Initialize system with less than 3 coordinates") {
            // Creating a fake system
            std::vector<double> v(2, 1.0);
            std::string id_v = "cl";
            std::vector<std::string> at_v = {"Cl"};

            bblock::System s_wrong;
            s_wrong.AddMonomer(v, at_v, id_v);

            bool system_too_short = false;

            // Testing that with less than 3 coordinates you cannot have a system
            try {
                s_wrong.Initialize();
            } catch (CUException &e) {
                system_too_short = true;
            }

            REQUIRE(system_too_short);
        }

        SECTION("Initialize system with more coordinates than expected") {
            // Creating a fake system
            std::vector<double> v(4, 1.0);
            std::string id_v = "cl";
            std::vector<std::string> at_v = {"Cl"};

            bblock::System s_wrong;
            s_wrong.AddMonomer(v, at_v, id_v);

            bool system_too_long = false;

            // Testing that with less than 3 coordinates you cannot have a system
            try {
                s_wrong.Initialize();
            } catch (CUException &e) {
                system_too_long = true;
            }

            REQUIRE(system_too_long);
        }
    }
    
    SECTION("GetPairList()") {
        SECTION("Get clusters of more than 3 molecules") {
            bool not_possible_to_get_clusters = false;
            try {
                my_system.GetPairList(5, 3.0, 0, 1);
            } catch (CUException &e) {
                not_possible_to_get_clusters = true;
            }
            REQUIRE(not_possible_to_get_clusters);
        }
    }
    
    SECTION("SetPBC()") {
        SECTION("Wrong box size") {
            bool not_possible_to_set_box = false;
            try {
                std::vector<double> box(5, 10.0);
                my_system.SetPBC(box);
            } catch (CUException &e) {
                not_possible_to_set_box = true;
            }
            REQUIRE(not_possible_to_set_box);
        }
    }

     //   REQUIRE( == Approx(finiteDifferenceForce).margin(TOL));
}











