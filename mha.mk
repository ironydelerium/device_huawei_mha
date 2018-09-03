TARGET_KERNEL_USE=4.4

TARGET_PREBUILT_KERNEL := out/target/product/mha/kernel-output/arch/arm64/boot/Image.gz
TARGET_USES_MKE2FS     := false

# Inherit the full_base and device configurations
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, device/huawei/mha/hi3660/device-hi3660.mk)
$(call inherit-product, device/huawei/mha/device-common.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)
# $(call inherit-product, $(SRC_TARGET_DIR)/product/product_launched_with_p.mk)

# Overrides
PRODUCT_NAME := mha
PRODUCT_DEVICE := hi3660
PRODUCT_BRAND := Huawei
PRODUCT_MODEL := Huawei Mate 9

