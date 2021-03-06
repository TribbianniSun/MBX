/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

#include "testutils.h"

#include "setup_topology.h"
#include "potential/force_field/inversion.h"
#include "potential/force_field/topology.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

constexpr double TOL = 1E-6;

TEST_CASE("Test dummy h4 inversion") {
    // Create the system
    SETUP_TOPOLOGY

    SECTION("Inversion-harmonic") {
        std::string functional_form = "harm";
        eff::Inversion inversion(inversion_connectivity, inversion_indexes, functional_form);
        inversion.SetParameters(harm_linear_parameters, harm_nonlinear_parameters);
        SECTION("Harmonic Energy") {
            REQUIRE(inversion.GetEnergy(inversion_phi) == Approx(ff_inversion_harm_energy).margin(TOL));
        }
        SECTION("Harmonic Gradient") {
            REQUIRE(inversion.GetTopologyGradient(inversion_phi) == Approx(ff_inversion_harm_grad).margin(TOL));
        }
    }

    SECTION("Inversion-none") {
        std::string functional_form = "none";
        eff::Inversion inversion(inversion_connectivity, inversion_indexes, functional_form);
        inversion.SetParameters(none_linear_parameters, none_nonlinear_parameters);
        SECTION("None Energy") {
            REQUIRE(inversion.GetEnergy(inversion_phi) == Approx(ff_inversion_none_energy).margin(TOL));
        }
        SECTION("None Gradient") {
            REQUIRE(inversion.GetTopologyGradient(inversion_phi) == Approx(ff_inversion_none_grad).margin(TOL));
        }
    }

    SECTION("Inversion-undefined-func-form") {
        std::string functional_form = "abcd";
        bool not_possible_to_setup_inversion = false;
        try {
            eff::Inversion inversion(inversion_connectivity, inversion_indexes, functional_form);
        } catch (CUException &e) {
            not_possible_to_setup_inversion = true;
        }
        REQUIRE(not_possible_to_setup_inversion);
    }
}
