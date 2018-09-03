#define LOG_TAG "VibratorService"

#include <inttypes.h>
#include <log/log.h>

#include <android/hardware/vibrator/1.0/IVibrator.h>
#include <hidl/Status.h>
#include <hidl/MQDescriptor.h>

#define SYSFS_PATH "/sys/class/timed_output/vibrator/"

#define ENABLE 		SYSFS_PATH "enable"
#define STRENGTH	SYSFS_PATH "set_amplitude"

namespace android {
namespace hardware {
namespace vibrator {
namespace V1_0 {
namespace implementation {

struct Vibrator : public IVibrator {
	Vibrator() {}

	Return<Status> on(uint32_t timeout_ms) override {
		int fd = open(ENABLE, O_WRONLY);
		char buf[24];

		if (fd == -1) {
			ALOGE("on(): open(\"" ENABLE "\") failed: (%d) %s", errno, strerror(errno));
			return Status::UNKNOWN_ERROR;
		}

		int len = snprintf(buf, 24, "%u", timeout_ms);
		write(fd, buf, len);
		close(fd);
		return Status::OK;
	}

	Return<Status> off() override{
		int fd = open(ENABLE, O_WRONLY);
		const char *buf = "0";

		if (fd == -1) {
			ALOGE("off(): open(\"" ENABLE "\") failed: (%d) %s", errno, strerror(errno));
			return Status::UNKNOWN_ERROR;
		}

		write(fd, buf, 1);
		close(fd);
		return Status::OK;
	}

	Return<bool> supportsAmplitudeControl() override {
		return true;
	}

	Return<Status> setAmplitude(uint8_t amplitude) override {
		int fd = open(STRENGTH, O_WRONLY);
		char buffer[4];
		uint16_t amp = amplitude << 4;

		if (fd == -1) {
			ALOGE("setAmplitude: open(\"" STRENGTH "\") failed: (%d) %s", errno, strerror(errno));
			return Status::UNKNOWN_ERROR;
		}

		int len = snprintf(buffer, 4, "%u", (unsigned int) amp);
		write(fd, buffer, len);
		close(fd);
		return Status::OK;
	}

	Return<void> perform(Effect, EffectStrength, perform_cb hidl_cb) override {
		hidl_cb(Status::UNSUPPORTED_OPERATION, 0);
		return Void();
	}
};

} // implementation
} // V1_0
} // vibrator
} // hardware
} // android

extern "C" android::hardware::vibrator::V1_0::IVibrator* HIDL_FETCH_IVibrator(const char* /* name */) {
	return new android::hardware::vibrator::V1_0::implementation::Vibrator();
}

