
#include "main.h"

//MOTOR PORTS BROKEN = 10 12
int8_t DRIVE_MOTOR_FL = 15;
int8_t DRIVE_MOTOR_FR = -19;
int8_t DRIVE_MOTOR_BL = 11;
int8_t DRIVE_MOTOR_BR = -20;
int TILTER = -1;
int LIFT = 4;
int LEFT_ROLLER = 6;
int RIGHT_ROLLER = -8;


//TARGETS FOR MACROS
int midTowerTarget=2223;
//increqsing value means tilter goes forward less
int trayTarget=2030;
int trayMid=1000;
int lowTowerTarget = 2006;


//MOTOR DECLARATION
Motor tilter(TILTER);
Motor lift(LIFT);
Motor rollerL(LEFT_ROLLER);
Motor rollerR(RIGHT_ROLLER);
Motor driveFL(DRIVE_MOTOR_FL);
Motor driveFR(DRIVE_MOTOR_FR);
Motor driveBL(DRIVE_MOTOR_BL);
Motor driveBR(DRIVE_MOTOR_BR);

//POT DECLARATION
Potentiometer liftPot('B');
Potentiometer trayPot('C');

//CONTROLLER
Controller controller;

//BUTTONS
ControllerButton intakeIn(ControllerDigital::R1);
ControllerButton intakeOut(ControllerDigital::R2);

ControllerButton magazineForward(ControllerDigital::X);
ControllerButton magazineBackward(ControllerDigital::B);

ControllerButton liftUp(ControllerDigital::A);
ControllerButton liftDown(ControllerDigital::Y);

ControllerButton driveToggle(ControllerDigital::L1);

ControllerButton lowTowerButton(ControllerDigital::right);
ControllerButton stackButton(ControllerDigital::left);
ControllerButton midTowerButton(ControllerDigital::down);

ControllerButton override(ControllerDigital::up);

//DRIVE
auto drive = ChassisControllerBuilder()
.withMotors({DRIVE_MOTOR_FL, DRIVE_MOTOR_BL}, {DRIVE_MOTOR_FR, DRIVE_MOTOR_BR})
.withDimensions(AbstractMotor::gearset::green, {{4_in, 12_in}, imev5GreenTPR})
.build();

auto tilterAuto = AsyncPosControllerBuilder()
.withMotor(tilter)
.withMaxVelocity(50)
.build();

//USER CONTROL FUNCTIONS
void rollers(int speed){
	rollerL.moveVelocity(speed);
	rollerR.moveVelocity(speed);
}

void intakeControl(){
	if(intakeIn.isPressed()){
		rollers(200);
	}
	else if(intakeOut.isPressed()){
		rollers(-200);
	}
	else{
		rollers(0);
	}
}

void magazineControl(){
	if(magazineForward.isPressed())
	{
		tilter.moveVelocity(50);
  }
	else if(magazineBackward.isPressed())
	{
		tilter.moveVelocity(-100);
  }
	else
	{
		tilter.moveVelocity(0);
	}
}

void liftControl(){
	if(liftUp.isPressed())
	{
			lift.moveVelocity(80);
	}
	else if(liftDown.isPressed())
	{
		lift.moveVelocity(-80);
	}
	else
	{
		lift.moveVelocity(0);
	}
}

void driveControl(){
	if(driveToggle.isPressed())
    	drive->getModel()->arcade(controller.getAnalog(ControllerAnalog::leftY)/2,(controller.getAnalog(ControllerAnalog::rightX)/2));
	else
			drive->getModel()->arcade((controller.getAnalog(ControllerAnalog::leftY)), (controller.getAnalog(ControllerAnalog::rightX)/2));
}

//MACROS
void stack()
{
	int error=trayTarget-trayPot.get();
	while(fabs(error)>10)
	{
		if(override.isPressed())
		{
			break;
		}
		error=trayTarget-trayPot.get();
		if(error<70)
			tilter.moveVelocity(50);
		else
			tilter.moveVelocity(100);
	}
}

void midTowerMacro()
{
	int error = midTowerTarget-liftPot.get();
	while(fabs(error)>10)
	{
		if(override.isPressed())
		{
			break;
		}
		error = midTowerTarget-liftPot.get();
		lift.moveVelocity(100);
	}
}

void lowTowerMacro()
{
	int error = lowTowerTarget-liftPot.get();
	while(fabs(error)>10)
	{
		if(override.isPressed())
		{
			break;
		}
		error = lowTowerTarget-liftPot.get();
		lift.moveVelocity(100);
	}
}

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	tilter.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerL.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerR.setBrakeMode(AbstractMotor::brakeMode::hold);
	lift.setBrakeMode(AbstractMotor::brakeMode::hold);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	tilter.setBrakeMode(AbstractMotor::brakeMode::hold);


	//BACK BLUE AUTON
	lift.moveVelocity(-100);
	rollers(-200);
	pros::delay(1200);
	rollers(200);
	drive->setMaxVelocity(75);
	drive->moveDistance(45_in);
	drive->waitUntilSettled();
	rollers(0);
	drive->moveDistance(-26.5_in);
	drive->waitUntilSettled();
	drive->turnAngle(-100_deg);
	drive->moveDistance(15_in);
	drive->waitUntilSettled();
	rollers(-70);
	pros::delay(720);
	rollers(0);
	tilter.moveRelative(2600,100);
	pros::delay(2000);
	rollers(-125);
	drive->moveDistance(-15_in);
	drive->waitUntilSettled();
	rollers(0);

	//BACK RED AUTON
	/*lift.moveVelocity(-100);
	rollers(-200);
	pros::delay(1000);
	rollers(200);
	drive->setMaxVelocity(75);
	drive->moveDistance(45_in);
	drive->waitUntilSettled();
	rollers(0);
	drive->moveDistance(-25_in);
	drive->waitUntilSettled();
	drive->turnAngle(110_deg);
	drive->moveDistance(17_in);
	drive->waitUntilSettled();
	pros::delay(720);
	rollers(0);
	stack();
	pros::delay(800);
	drive->moveDistance(0.5_in);
	drive->waitUntilSettled();
	rollers(-125);
	drive->moveDistance(-15_in);
	drive->waitUntilSettled();
	rollers(0);*/

	//ONE POINT
	/*rollers(-200);
	pros::delay(1200);
	lift.moveVelocity(-100);
	drive->moveDistance(20_in);
	drive->waitUntilSettled();
	drive->moveDistance(-1_ft);
	drive->waitUntilSettled();
	rollers(0);*/
	}


/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	while (true) {
		driveBL.setBrakeMode(AbstractMotor::brakeMode::brake);
		driveBR.setBrakeMode(AbstractMotor::brakeMode::brake);
		driveFL.setBrakeMode(AbstractMotor::brakeMode::brake);
		driveFR.setBrakeMode(AbstractMotor::brakeMode::brake);

		driveControl();
		intakeControl();
		magazineControl();
		liftControl();

		if(stackButton.isPressed()){
			stack();
		}

		if(midTowerButton.isPressed())
		{
			midTowerMacro();
		}

		if(lowTowerButton.isPressed())
		{
			lowTowerMacro();
		}
		pros::delay(20);
	}
}
