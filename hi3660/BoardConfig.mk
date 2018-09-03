include device/huawei/mha/BoardConfigCommon.mk

TARGET_BOOTLOADER_BOARD_NAME := hi3660
TARGET_BOARD_PLATFORM := hi3660

TARGET_CPU_VARIANT := cortex-a73
TARGET_2ND_CPU_VARIANT := cortex-a73

TARGET_NO_DTIMAGE := true

BOARD_KERNEL_CMDLINE := loglevel=4 initcall_debug=n page_tracker=on slub_min_objects=16
BOARD_KERNEL_CMDLINE += unmovable_isolate1=2:192M,3:224M,4:256M printktimer=0xfff0a000,0x534,0x538
BOARD_KERNEL_CMDLINE += androidboot.selinux=enforcing buildvariant=$(TARGET_BUILD_VARIANT)

BOARD_MKBOOTIMG_ARGS := --base 0x78000 --pagesize 2048 --kernel_offset 0x78000 --ramdisk_offset 0x7b88000
BOARD_MKBOOTIMG_ARGS += --second_offset 0xf00000 --tags_offset 0x7988000 

# BOARD_BUILD_SYSTEM_ROOT_IMAGE := true

BOARD_SYSTEMIMAGE_PARTITION_SIZE   := 2768240640   # 2640MB
BOARD_USERDATAIMAGE_PARTITION_SIZE := 25769803776 # 24GB
BOARD_CACHEIMAGE_PARTITION_SIZE    := 8388608       # 8MB
BOARD_FLASH_BLOCK_SIZE             := 512

BOARD_HAVE_BLUETOOTH_BCM           := true
# BOARD_HAVE_BLUETOOTH_BCM_A2DP_OFFLOAD := true
BOARD_CUSTOM_BT_CONFIG             := device/huawei/mha/hi3660/bluetooth/vnd_hi3660.txt


