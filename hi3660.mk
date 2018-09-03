TARGET_KERNEL_USE=4.4

# Inherit the full_base and device configurations
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, device/huawei/mha/device-hi3660.mk)
$(call inherit-product, device/linaro/hikey/device-common.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)

# Overrides
PRODUCT_NAME := Mate 9
PRODUCT_DEVICE := hi3660
PRODUCT_BRAND := Huawei
PRODUCT_MODEL := Huawei Mate 9

