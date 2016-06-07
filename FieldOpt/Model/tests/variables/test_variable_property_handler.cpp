#include <gtest/gtest.h>
#include "Model/properties/variable_property_handler.h"
#include "Model/properties/property_exceptions.h"

using namespace Model::Properties;

namespace {

class VariableHandlerTest : public ::testing::Test {
protected:
    VariableHandlerTest() {
        settings_ = new Utilities::Settings::Settings(driver_file_path_, output_directory_);
    }

    ~VariableHandlerTest() {}

    virtual void SetUp() {
        variable_handler_ = new VariablePropertyHandler(*settings_->model());
    }
    virtual void TearDown() {
        delete variable_handler_;
    }

    Utilities::Settings::Settings *settings_;
    QString driver_file_path_ = "../examples/driver.json";
    QString output_directory_ = "../fieldopt_output";
    VariablePropertyHandler *variable_handler_;
};

TEST_F(VariableHandlerTest, Wells) {
    EXPECT_STREQ("PROD", variable_handler_->GetWell("PROD")->name().toLatin1().constData());
    EXPECT_STREQ("INJ", variable_handler_->GetWell("INJ")->name().toLatin1().constData());
    EXPECT_THROW(variable_handler_->GetWell("NOWELL"), VariablePropertyHandlerCannotFindObjectException);
}

TEST_F(VariableHandlerTest, ProducerControls) {
    QList<int> control_times = QList<int> {0, 50, 100, 365};

    // All these should be defaulted to false (they are not set to variable at any time step)
    foreach (int control_time, control_times) {
        EXPECT_FALSE(variable_handler_->GetControl("PROD", control_time)->open());
        EXPECT_FALSE(variable_handler_->GetControl("PROD", control_time)->rate());
        EXPECT_FALSE(variable_handler_->GetControl("INJ", control_time)->open());
        EXPECT_FALSE(variable_handler_->GetControl("INJ", control_time)->rate());
        EXPECT_FALSE(variable_handler_->GetControl("INJ", control_time)->bhp());
    }
    EXPECT_THROW(variable_handler_->GetControl("PROD", 10)->open(), VariablePropertyHandlerCannotFindObjectException);
    EXPECT_THROW(variable_handler_->GetControl("INJ", 15)->open(), VariablePropertyHandlerCannotFindObjectException);

    // These have been changed for some time steps
    EXPECT_TRUE(variable_handler_->GetControl("PROD", 0)->bhp());
    EXPECT_TRUE(variable_handler_->GetControl("PROD", 50)->bhp());
    EXPECT_TRUE(variable_handler_->GetControl("PROD", 365)->bhp());

}



}

