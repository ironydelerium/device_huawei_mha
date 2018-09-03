#!/system/bin/sh

# Start screen updates
echo 1 > /sys/class/graphics/fb0/frame_update

# Set up NVE
echo -n /dev/block/sdd4 > /sys/module/hisi_nve/parameters/nve

# Set up Bluetooth
insmod /vendor/lib/modules/bluesleep.ko
insmod /vendor/lib/modules/bluetooth_power.ko

chcon u:object_r:sysfs_bluetooth_writable:s0 /sys/class/rfkill/*/state
chown bluetooth:bluetooth /sys/class/rfkill/*/state

echo 1 > /proc/bluetooth/sleep/btwake
echo 1 > /proc/bluetooth/sleep/btwrite
echo 1 > /proc/bluetooth/sleep/proto

# Set up leds
chcon u:object_r:sysfs_leds:s0 /sys/class/leds/*/max_brightness
chcon u:object_r:sysfs_leds:s0 /sys/class/leds/*/brightness
chown system:system /sys/class/leds/*/brightness
chown system:system /sys/class/leds/*/max_brightness

# FIXME: Workaround for webview
chmod 0666 /sys/kernel/debug/tracing/trace_marker

