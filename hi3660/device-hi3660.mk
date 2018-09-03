#
# Copyright (C) 2011 The Android Open-Source Project
# Copyright (C) 2018 Chris Vandomelen (irony.delerium@gmail.com)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

PRODUCT_COPY_FILES +=	\
			$(LOCAL_PATH)/fstab.hi3660:root/fstab.hi3660 \
			device/huawei/mha/init.common.rc:root/init.hi3660.rc \
			device/huawei/mha/init.hi3660.power.rc:root/init.hi3660.power.rc \
			device/huawei/mha/init.common.usb.rc:root/init.hi3660.usb.rc \
			device/huawei/mha/ueventd.common.rc:root/ueventd.hi3660.rc \
			frameworks/native/data/etc/android.hardware.vulkan.level-0.xml:system/etc/permissions/android.hardware.vulkan.level.xml \
			frameworks/native/data/etc/android.hardware.vulkan.version-1_0_3.xml:system/etc/permissions/android.hardware.vulkan.version.xml

# Bluetooth configuration
# This file really should be in vendor/etc/bluetooth/bt_vendor.conf, but since I'm not yet building
# a split-vendor image, the bluetooth stack looks for it in /system/etc/bluetooth.
#
# Actually put it in the proper place for a split-vendor image though, too.
PRODUCT_COPY_FILES += \
			$(LOCAL_PATH)/bluetooth/bt_vendor.conf:system/etc/bluetooth/bt_vendor.conf \
			$(LOCAL_PATH)/bluetooth/bt_vendor.conf:system/vendor/etc/bluetooth/bt_vendor.conf

PRODUCT_PACKAGES += libbt-vendor

# Healthd
PRODUCT_PACKAGES += android.hardware.health@2.0-service.override
DEVICE_FRAMEWORK_MANIFEST_FILE += system/libhidl/vintfdata/manifest_healthd_exclude.xml
PRODUCT_PACKAGES += android.hardware.health@2.0-service.hi3660

# Vibrator
PRODUCT_PACKAGES += android.hardware.vibrator@1.0-service.hi3660

# Backlight & notification LEDs
PRODUCT_PACKAGES += android.hardware.light@2.0-service.hi3660

# Copy boot script
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/boot.sh:system/bin/mha-boot.sh

# Copy kernel
PRODUCT_COPY_FILES += \
	out/target/product/mha/kernel-output/arch/arm64/boot/Image.gz:kernel

# Graphics
# TODO: Replae the framebuffer based gralloc module with drm-gralloc and drm-hwcomposer
PRODUCT_PACKAGES += gralloc.hi3660

# Power
PRODUCT_PACKAGES += power.hi36660

# TinyALSA
PRODUCT_PACKAGES += tinymix tinycap tinypcminfo

# Include vendor binaries
$(call inherit-product-if-exists, vendor/huawei/mha/device-vendor.mk)
$(call inherit-product-if-exists, vendor/gapps/device-vendor.mk)

