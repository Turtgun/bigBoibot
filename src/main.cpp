#include "main.h"
#include "Display.hpp"
#include "pros/misc.h"
#include "systems/DriveTrain.hpp"
#include "systems/Catapult.hpp"
#include "systems/Flaps.hpp"
#include "autonomous/Odometry.hpp"

using namespace pros;
using namespace Display;

Controller master(E_CONTROLLER_MASTER);

DriveTrain dt = DriveTrain();
Catapult cata = Catapult();
Flaps fp = Flaps();

LV_IMG_DECLARE(normal);
lv_obj_t* bgImg = lv_img_disp(&normal);

char lY,rY,rX = 0;

bool arcade;
inline lv_res_t toggleMode(lv_obj_t* btn)
{
	
    if (arcade) {
		dt.teleMove = [=]{dt.tankDrive(lY,rY);};
		lv_label_set_text(lv_obj_get_child(btn, NULL), "Tank Drive");
		} else {
		dt.teleMove = [=]{dt.arcadeDrive(lY,rX);};
		lv_label_set_text(lv_obj_get_child(btn, NULL), "Arcade Drive");
    }
	arcade = !arcade;

	btnSetToggled(btn, arcade);
    return LV_RES_OK;
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	/*lv_obj_t* odometryInfo = createLabel(lv_scr_act(), Display::DISP_CENTER, 300, 40, "Odom Info");
	Odometry odom = Odometry(&dt, &odometryInfo);*/

	dt.teleMove = [=]{dt.tankDrive(lY,rY);};
	lv_obj_t* driveBtn = createBtn(lv_scr_act(), Display::DISP_CENTER, 300, 20, "Tank Drive", LV_COLOR_MAKE(62, 180, 137), LV_COLOR_MAKE(153, 50, 204));
	lv_btn_set_action(driveBtn, LV_BTN_ACTION_CLICK, toggleMode);

	lv_obj_t* pickleT = createLabel(lv_scr_act(), DISP_CENTER, 300, 100,
		"Current pickle high scores (5 min)\nEsteban: 11\nJayleen: 10\nJI: 9");
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
	dt.arcadeDrive(127, 0);
	delay(2000);
	dt.arcadeDrive(0, 0);
	dt.arcadeDrive(-127, 0);
	delay(500);
	dt.arcadeDrive(0, 0);
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
	bool prcsM = false;
	int prcsET, fpET = 0;
	while (true) {
		// Set precision mode (dont repeat until half a second)
		if (master.get_digital(E_CONTROLLER_DIGITAL_R2) && (millis() - prcsET > 500)) {prcsM = !prcsM; prcsET = millis();}

		// Toggle flaps (dont repeat until half a second)
		if (master.get_digital(E_CONTROLLER_DIGITAL_A) && (millis() - fpET > 500)) {fp.toggle(); fpET = millis();}

		{ // Set input for the drivetrain's teleMove according to precision mode.
			lY = (prcsM) ? master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)/2 : master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
			rY = (prcsM) ? master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y)/2 : master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);
			rX = (prcsM) ? master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X)/2 : master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
		}

		dt.teleMove();

		// Automatically move the catapult down unless it's hitting the button, or continue moving down if B is pressed
		//cata.move(-127, !master.get_digital(E_CONTROLLER_DIGITAL_B));
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {cata.move(-127);} else {cata.move(-30);}

		delay(20);
	}
}