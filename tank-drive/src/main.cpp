#include "main.h"
#include "srcMain.h"
#include <vector>

pros::Controller master (CONTROLLER_MASTER);


/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
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
void autonomous() {}

class MotorSet {
	private:
	 	std::vector<pros::Motor> left_motors;
		std::vector<pros::Motor> right_motors;

	public:
		MotorSet(std::vector<int> left_motors_ports, std::vector<int> right_motors_ports) {
			left_motors = std::vector<pros::Motor> {
				pros::Motor (left_motors_ports[0]),
				pros::Motor (left_motors_ports[1]),
				pros::Motor (left_motors_ports[2]),
			};

			right_motors = std::vector<pros::Motor> {
				pros::Motor (right_motors_ports[0]),
				pros::Motor (right_motors_ports[1]),
				pros::Motor (right_motors_ports[2]),

			};
		}		

		void move_left_wheels(int32_t voltage) {
			for (pros::Motor motor : left_motors) {
				motor.move(voltage);
			}
		}

		void move_right_wheels(int32_t voltage) {
			for (pros::Motor motor : left_motors) {
				motor.move(voltage);
			}
		}
};

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
	MotorSet motors (ports::left_ports, ports::right_ports);

	while (true) {
		motors.move_left_wheels(master.get_analog(ANALOG_LEFT_Y));
		motors.move_right_wheels(master.get_analog(ANALOG_RIGHT_Y));

		pros::delay(2);
	}
}
