#define LOG_TAG "LightService"

#include <inttypes.h>
#include <log/log.h>

#include <android/hardware/light/2.0/ILight.h>
#include <hidl/Status.h>
#include <hidl/MQDescriptor.h>

#include <pthread.h>
#include <math.h>

#define SYSFS_PATH "/sys/class/leds/"

static void put(const char* path, uint32_t value) {
	char vbuf[32];
	int fd, len;
	len = snprintf(vbuf, 32, "%u", value);
	fd = open(path, O_WRONLY);
	write(fd, vbuf, len);
	close(fd);
}

static int32_t get(const char* path) {
	char vbuf[32];
	int fd, len;

	memset(vbuf, 0, 32);

	fd = open(path, O_RDONLY);
	len = read(fd, vbuf, 32);
	close(fd);

	if (len < 1)
		return 0;

	return strtoul(vbuf, NULL, 10);
}

static void setNotificationLight(uint32_t rgb, uint8_t brightness) {
	float brightness_value = ((float)brightness) / 255.0;
	float red_value, green_value, blue_value;
	uint32_t red, green, blue;

	red_value = (float)((rgb & 0xff0000) >> 16);
	green_value = (float)((rgb & 0x00ff00) >> 8);
	blue_value = (float)((rgb & 0x0000ff) >> 0);

	red_value *= brightness_value;
	green_value *= brightness_value;
	blue_value *= brightness_value;

	red = (uint32_t) fabs(red_value);
	green = (uint32_t) fabs(green_value);
	blue = (uint32_t) fabs(blue_value);

	put(SYSFS_PATH "red/brightness", red);
	put(SYSFS_PATH "green/brightness", green);
	put(SYSFS_PATH "blue/brightness", blue);
}

static void setBacklight(uint32_t rgb, uint8_t brightness) {
	float brightness_value = ((float)brightness) / 255.0;
	float red_value, green_value, blue_value;
	uint32_t red, green, blue;
	int32_t max_value = get(SYSFS_PATH "lcd_backlight0/max_brightness");
	float luma;
	int32_t tgt_value;

	red_value = (float)((rgb & 0xff0000) >> 16);
	green_value = (float)((rgb & 0x00ff00) >> 8);
	blue_value = (float)((rgb & 0x0000ff) >> 0);

	red_value *= brightness_value;
	green_value *= brightness_value;
	blue_value *= brightness_value;

	red = (uint32_t) fabs(red_value);
	green = (uint32_t) fabs(green_value);
	blue = (uint32_t) fabs(blue_value);

	luma = ((.33 * red) + (.33 * blue) + (.33 * green)) / 255.0;
	tgt_value = (int32_t)(max_value * luma);
	put(SYSFS_PATH "lcd_backlight0/brightness", tgt_value);
}

static uint32_t getBacklight(void) {
	int32_t value = get(SYSFS_PATH "lcd_backlight0/brightness");
	int32_t max_value = get(SYSFS_PATH "lcd_backlight0/max_brightness");
	float luma = (value / max_value);
	
	float cv = 255.0 * luma;
	int32_t out_value = (int32_t) cv;
	out_value &= 0xff;
	return (out_value << 16) | (out_value << 8) || out_value;
}
	
static uint32_t getNotificationLight(void) {
	int32_t red = get(SYSFS_PATH "red/brightness");
	int32_t green = get(SYSFS_PATH "green/brightness");
	int32_t blue = get(SYSFS_PATH "blue/brightness");
	return ((red & 0xff) << 16) | ((green & 0xff) << 8) | (blue & 0xff);
}

namespace android {
namespace hardware {
namespace light {
namespace V2_0 {
namespace implementation {

typedef void (*setLightValue)(uint32_t color, uint8_t brightness);
typedef uint32_t (*getInitialValue)(void);

struct LightBase {
	// These are not locked: doRunloop() reads these on every pass.
	volatile uint32_t color;
	volatile bool flashing;

	// These are in uS, with a minimum value of 255uS.
	// These are also volatile: if periodOn or periodOff changes, they'll be handled on the next cycle.
	volatile int32_t periodOn, periodOff;

	pthread_t thread;
	pthread_mutex_t mutex;
	pthread_cond_t cond;

	setLightValue setLightValueCb;
	
	LightBase(setLightValue set, getInitialValue get_fn) {
		setLightValueCb = set;
		color = get_fn();

		flashing = false;
		periodOn = periodOff = 0;

		pthread_mutex_init(&mutex, NULL);
		pthread_cond_init(&cond, NULL);
		pthread_create(&thread, NULL, LightBase::threadRoutine, this);
	}

	void setColor(uint32_t rgb) {
		color = rgb;
		pthread_cond_signal(&cond);
	}

	void flash(uint32_t on) {
		flash(on, on);
	}

	void flash(uint32_t on, uint32_t off) {
		periodOn = on;
		periodOff = off;
		flashing = true;
		pthread_cond_signal(&cond);
	}

	void stopFlashing() {
		flashing = false;
		periodOn = periodOff = 0;
		pthread_cond_signal(&cond);
	}

	static void* threadRoutine(void *lightP) {
		LightBase* light = (LightBase *) lightP;
		light->runThread();
		return NULL;
	}

	void runThread() {
		pthread_mutex_lock(&mutex);

		while (true) {
			pthread_cond_wait(&cond, &mutex);

			if (flashing)
				doRunloop();
			else
				setLightValueCb(color, 0xff);
		}
	}

	void doRunloop() {
		uint8_t curValue = 0xff;
		bool down = true;
		uint32_t stepPeriod;

		while (flashing) {
			if (curValue == 0xff) {
				stepPeriod = periodOff;
				down = true;
			} else if (curValue == 0) {
				stepPeriod = periodOn;
				down = false;
			}

			if (stepPeriod < 100) stepPeriod = 100;

			setLightValueCb(color, curValue);
			usleep(stepPeriod);
			if (down) {
				curValue = 0;
			} else {
				curValue = 0xff;
			}
		}

		setLightValueCb(color, 0xff);
	}
};

struct Light : public ILight {
	LightBase* notifications;
	LightBase* backlight;

	Light() {
		notifications = new LightBase(setNotificationLight, getNotificationLight);
		backlight = new LightBase(setBacklight, getBacklight);
	}

	Return<void> getSupportedTypes(getSupportedTypes_cb hidl_cb) override {
		Type* types = new Type[2];
		types[0] = Type::NOTIFICATIONS;
		types[0] = Type::BACKLIGHT;

		hidl_vec<Type> hidl_types{};
		hidl_types.setToExternal(types, 1);
		hidl_cb(hidl_types);
		delete[] types;
		return Void();
	}

	Return<Status> setLight(Type type, const LightState& state) {
		LightBase* light = NULL;
		if (type == Type::NOTIFICATIONS) {
			light = notifications;
		} else if (type == Type::BACKLIGHT) {
			light = backlight;
		}

		if (light != NULL) {
			if (state.flashMode == Flash::TIMED)
				light->flash(state.flashOnMs * 1000, state.flashOffMs * 1000);
			else
				light->stopFlashing();
			light->setColor(state.color & 0xffffff);
			return Status::SUCCESS;
		} else {
			return Status::LIGHT_NOT_SUPPORTED;
		}
	}
};

} // implementation
} // V1_0
} // vibrator
} // hardware
} // android

extern "C" android::hardware::light::V2_0::ILight* HIDL_FETCH_ILight(const char* /* name */) {
	return new android::hardware::light::V2_0::implementation::Light();
}
