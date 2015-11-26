/******************************************************************************
 *
 *
 *
 * Created: 09.11.2015 2015 by einar
 *
 * This file is part of the FieldOpt project.
 *
 * Copyright (C) 2015-2015 Einar J.M. Baumann <einar.baumann@ntnu.no>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *****************************************************************************/

#include "GTest/Model/test_fixture_model_base.h"
#include "Model/model.h"

namespace {

class ModelTest : public ModelBaseTest {
protected:
    ModelTest() {
        model_ = new Model::Model(*settings_->model());
    }

    Model::Model *model_;
};

TEST_F(ModelTest, Constructor) {
    EXPECT_TRUE(true);
}

TEST_F(ModelTest, ChildObjects) {
    EXPECT_NO_THROW(model_->reservoir());
    EXPECT_NO_THROW(model_->wells());
    EXPECT_NO_THROW(model_->variables());
    EXPECT_GE(model_->wells()->size(), 2);
    EXPECT_GE(model_->variables()->BinaryVariableSize(), 0);
    EXPECT_GE(model_->variables()->IntegerVariableSize(), 12);
    EXPECT_GE(model_->variables()->RealVariableSize(), 5);
}

TEST_F(ModelTest, Variables) {
    // As of 2015.11.10, the variables are:
    // 3 real variables (bhp at three time steps for the producer)
    EXPECT_TRUE(variable_handler_->GetControl("PROD", 0)->bhp());
    EXPECT_TRUE(variable_handler_->GetControl("PROD", 50)->bhp());
    EXPECT_TRUE(variable_handler_->GetControl("PROD", 365)->bhp());
    EXPECT_EQ(3, model_->wells()->at(0)->controls()->size());
    EXPECT_FLOAT_EQ(2000.0, model_->variables()->GetRealVariable(0)->value());
    EXPECT_FLOAT_EQ(2000.0, model_->variables()->GetRealVariable(1)->value());
    EXPECT_FLOAT_EQ(2000.0, model_->variables()->GetRealVariable(2)->value());

    // 2 real variables for the transmissibility of the producer's two perforations
    EXPECT_TRUE(variable_handler_->GetPerforation(0)->transmissibility_factor());
    EXPECT_TRUE(variable_handler_->GetPerforation(1)->transmissibility_factor());
    EXPECT_FLOAT_EQ(1.0, model_->variables()->GetRealVariable(3)->value());
    EXPECT_FLOAT_EQ(1.0, model_->variables()->GetRealVariable(4)->value());

    // 12 integer variables for the positions for the producer's four well blocks
    EXPECT_TRUE(variable_handler_->GetWellBlock(0)->position());
    EXPECT_TRUE(variable_handler_->GetWellBlock(1)->position());
    EXPECT_TRUE(variable_handler_->GetWellBlock(2)->position());
    EXPECT_TRUE(variable_handler_->GetWellBlock(3)->position());
    for (int i = 0; i < 11; ++i) {
        EXPECT_GE(model_->variables()->GetIntegerVariable(i)->value(), 0);
    }
}

}