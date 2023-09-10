#include "main.h"


// UPDATE MOTOR PORTS & PISTON WIRES.
pros::Motor left_motor_b(1);
pros::Motor left_motor_m(2);
pros::Motor left_motor_f(3);
pros::MotorGroup left_motorgroup({left_motor_b, left_motor_m, left_motor_f});

pros::Motor right_motor_b(1, true);
pros::Motor right_motor_m(2, true);
pros::Motor right_motor_f(3, true);
pros::MotorGroup right_motorgroup({right_motor_b, right_motor_m, right_motor_f});

pros::Motor intake_motor(1);

pros::Optical intake_color(1);

pros::ADIDigitalOut pistons('A');

pros::Motor flywheel_motor(1);

pros::ADIDigitalOut angle_changer('A');

pros::Controller master(pros::E_CONTROLLER_MASTER);

void initialize() {}


void disabled() {}


void competition_initialize() {}


void autonomous() {}


void run_intake() {
	static bool intake = true;

	if (intake == true) {
		while (true) {
			if (90 <= intake_color.get_hue() <= 150) {
				intake = !intake;
				break;
			}

			// Set point to manually break the cycle. 
			if (master.get_digital_new_press(DIGITAL_X)) {
				break;
			}

			intake_motor.move(127);

			pros::delay(10);
		}

	} else {
		while (true) {
			if (intake_color.get_hue() < 90 || intake_color.get_hue() > 150) {
				intake = !intake;
				break;
			}

			if (master.get_digital_new_press(DIGITAL_X)) {
				break;
			}

			intake_motor.move(-127);

			pros::delay(10);
		}
	}
}

void move_flaps() {
	static bool flaps_in = false;

	flaps_in = !flaps_in;

	if (flaps_in) {
		pistons.set_value(true);

	} else {
		pistons.set_value(false);
	}
}

void run_flywheel() {
	static bool flywheel_on = false;
	
	flywheel_on = !flywheel_on;

	if (flywheel_on) {
		flywheel_motor.move(127);
	} else {
		flywheel_motor.move(-127);
	}
}

void change_angle() {
	static bool angled = false;
	angled = !angled;

	if (angled) {
		angle_changer.set_value(true);
	} else {
		angle_changer.set_value(false);
	}
}

void opcontrol() {
	while (true) {
		int power = master.get_analog(ANALOG_LEFT_Y);
		int turn = master.get_analog(ANALOG_RIGHT_X);

		left_motorgroup.move(power + turn);
		right_motorgroup.move(power - turn);

		if (master.get_digital_new_press(DIGITAL_R1)) {
			run_intake();
		}
		if (master.get_digital_new_press(DIGITAL_R2)) {
			move_flaps();
		}
		if (master.get_digital_new_press(DIGITAL_L2)) {
			run_flywheel();
		}
		if (master.get_digital_new_press(DIGITAL_L1)) {
			change_angle();
		}


		pros::delay(10);
	}
}
