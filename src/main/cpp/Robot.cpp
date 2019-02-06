#include <Robot.h>
#include <WPILib.h>
#include <iostream>
#include <frc/Timer.h>
#include <TimedRobot.h>
#include <frc/Solenoid.h>
#include <frc/Joystick.h>
#include <ctre/Phoenix.h>
#include <frc/ADXRS450_Gyro.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/smartdashboard/SmartDashboard.h>

//Declarations

// Right Side Motors
WPI_TalonSRX BackRightBack{15};
WPI_VictorSPX BackRightMid{14};
WPI_VictorSPX BackRightFront{13};
// Left Side Motors
WPI_TalonSRX BackLeftBack{0};
WPI_VictorSPX BackLeftMid{1};
WPI_VictorSPX BackLeftFront{2};
// Speed Controller Groups
frc::SpeedControllerGroup RightMotors{BackRightFront,BackRightMid,BackRightBack};
frc::SpeedControllerGroup LeftMotors{BackLeftFront, BackLeftMid, BackLeftBack};
// Drive Train
frc::DifferentialDrive DriveTrain{LeftMotors, RightMotors};

// Gyro
frc::ADXRS450_Gyro Gyro{}; 

//Cargo Intake
VictorSPX FrontLeftMid{4};

//Pneumatics/ Lift
frc::Solenoid CargoIntake{0};
bool SolenoidButton = false;

//HatchLock

frc::Solenoid HatchIntake{1};

frc::Solenoid HatchIntake{1};

bool SolenidButton = false;


// Joystick & Racewheel
frc::Joystick JoyAccel1{0}, Xbox{1}, RaceWheel{2};

//Straightens out the bot
float LastSumAngle;

/*Called on robot connection*/
void Robot::RobotInit() {
  m_chooser.SetDefaultOption(kAutoNameDefault, kAutoNameDefault);
  m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);

  RightMotors.SetInverted(true);
  LeftMotors.SetInverted(false);

  CargoIntake.Set(false);

  Gyro.Reset();
}

/*Called on every robot packet, no matter what mode*/
void Robot::RobotPeriodic() {}

/*Called when Autonomous is enabled*/
void Robot::AutonomousInit() {
  m_autoSelected = m_chooser.GetSelected();
  std::cout << "Auto selected: " << m_autoSelected << std::endl;
  if (m_autoSelected == kAutoNameCustom) {
  } else {
  }
}

/*Called every robot packet in auto*/
void Robot::AutonomousPeriodic() {
  if (m_autoSelected == kAutoNameCustom) {
  } else {
  }
}

/*Called when teleop is enabled*/
void Robot::TeleopInit() {}

/*Called every robot packet in teleop*/
void Robot::TeleopPeriodic() {
  //Gets axis for each controller
  double JoyY = JoyAccel1.GetY();
  double WheelX = RaceWheel.GetX();

  //Power get's cut from one side of the bot to straighten out when driving straight
  float sumAngle = Gyro.GetAngle();
  float derivAngle = sumAngle - LastSumAngle;
  float correctionAngle = (sumAngle *.1) + (derivAngle *.2);

  // Intake Lift
  if (Xbox.GetRawButton(5)){
    /* Create a variable because it is impossible to press the button for only one robot packet.
    ** The variable is checked to be false, then inverses the intake and sets the variable is true.
    ** As long as the variable is true, nothing is run. The variable is set back to false when the
    ** button is released.
    */
    if (!SolenoidButton){
      CargoIntake.Set(!CargoIntake.Get());
      SolenoidButton = true;
    }
  } else {
    SolenoidButton = false;
  }

  // Intakes the ball
  if (Xbox.GetRawButton(3)){
    FrontLeftMid.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -.5);
  // Spits the ball
  } else if (Xbox.GetRawButton(1)){
    FrontLeftMid.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 1);
  } else {
    FrontLeftMid.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
  }

  // Use Arcade Drive if we're touching the joystick
  if (JoyAccel1.GetY() > 0.02 || JoyAccel1.GetY < -0.02) {
    DriveTrain.ArcadeDrive(-WheelX, JoyY);
  } else {
    // Point turning 
    if (RaceWheel.GetRawButton(5)){
      RightMotors.Set(WheelX);
      LeftMotors.Set(WheelX);
    }
  }
  
  //Straightens out bot here when driving straight
  LastSumAngle = sumAngle;

}

/*Called every robot packet in testing mode*/
void Robot::TestPeriodic() {}

/*Starts the bot*/
#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif