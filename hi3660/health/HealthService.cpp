#include <health2/service.h>
#include <healthd/healthd.h>

void healthd_board_init(struct healthd_config * config) {
	config->batteryStatusPath = "/sys/class/power_supply/Battery/status";
	config->batteryHealthPath = "/sys/class/power_supply/Battery/health";
	config->batteryPresentPath = "/sys/class/power_supply/Battery/present";
	config->batteryCapacityPath = "/sys/class/power_supply/Battery/capacity";
	config->batteryVoltagePath = "/sys/class/power_supply/Battery/voltage_now";
	config->batteryTemperaturePath = "/sys/class/power_supply/Battery/temp";
	config->batteryTechnologyPath = "/sys/class/power_supply/Battery/technology";
	config->batteryCurrentNowPath = "/sys/class/power_supply/Battery/current_now";
	config->batteryChargeCounterPath = "/sys/class/power_supply/Battery/charge_counter";
}

int healthd_board_battery_update(struct android::BatteryProperties *) {return 0;}

int main() {
	return health_service_main();
}

