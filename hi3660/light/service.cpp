/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define LOG_TAG "android.hardware.light@2.0-service"

#include <android/hardware/light/2.0/ILight.h>
#include <hidl/LegacySupport.h>

using android::hardware::light::V2_0::ILight;

extern "C" ILight* HIDL_FETCH_ILight(const char *);

int main(int /* argc */, char ** /* argv */) {
	::android::hardware::configureRpcThreadpool(1, true);
	::android::sp<ILight> light = HIDL_FETCH_ILight("default");
	const ::android::status_t status = light->registerAsService();
	if (status != ::android::OK)
		return 1;
	::android::hardware::joinRpcThreadpool();
	return 1;
}
