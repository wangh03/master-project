/******************************************************************************
   Copyright (C) 2015-2016 Einar J.M. Baumann <einar.baumann@gmail.com>

   This file is part of the FieldOpt project.

   FieldOpt is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   FieldOpt is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with FieldOpt.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef CONSTRAINTHANDLER_H
#define CONSTRAINTHANDLER_H
#include "constraint.h"
#include "bhp_constraint.h"
#include "well_spline_length.h"
#include "interwell_distance.h"
#include "combined_spline_length_interwell_distance.h"
#include "combined_spline_length_interwell_distance_reservoir_boundary.h"
#include "reservoir_boundary.h"
#include "rate_constraint.h"
#include "Optimization/case.h"
#include "Model/properties/variable_property_container.h"
#include "Settings/optimizer.h"
#include "Reservoir/grid/grid.h"

#include <QList>
#ifdef WITH_EXPERIMENTAL_CONSTRIANTS
// Includes for constraints in the experimental_constraints dir go here.
#endif

namespace Optimization {
    namespace Constraints {

        /*!
         * \brief The ConstraintHandler class facilitates the
         * initialization and usage of multiple constraints.
         */
        class ConstraintHandler
        {
        public:
            ConstraintHandler(QList<Settings::Optimizer::Constraint> constraints,
                              Model::Properties::VariablePropertyContainer *variables,
                              Reservoir::Grid::Grid *grid);
            bool CaseSatisfiesConstraints(Case *c); //!< Check if a Case satisfies _all_ constraints.
            void SnapCaseToConstraints(Case *c); //!< Snap all variables to _all_ constraints.

            QList<Constraint *> constraints() const { return constraints_; }

        private:
            QList<Constraint *> constraints_;
        };

    }
}

#endif // CONSTRAINTHANDLER_H
