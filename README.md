# Device

Huawei Mate 9 - testing device is MHA-L29C567.

This is not for any other devices: it is not a Treble build, it is a fully custom build. (This
should explain all of the broken features below.)

In the future, I plan to make this build a full Treble image with a 9.0 vendor, but in the meantime,
it's built as a single image, in the style of older Nougat and early Oreo builds.

The image, if the instructions below are followed, is pre-rooted (it's an "eng" build). adb is on
by default and runs as root. Developer settings is broken

The stock fstab here uses the Oreo partition layout:
* `/dev/block/sdd50` -> `/system`
* `/dev/block/sdd13` -> `/cache`
* `/dev/block/sdd55` -> `/data`

Completely untested, but it should also work on a Nougat partition layout, though you
will have to build your own boot image and update device/huawei/mha/hi3660/fstab.hi3660
appropriately.

---
 
## Working features:

- Basic graphics hardware acceleration & 3d support.
- Wifi
- Bluetooth (partial: kernel wakelock permanently held, preventing deep sleep. Generally functional,
  you will need to bring over your MAC address and configure it yourself.)
- Touchscreen
- Notification LED
- Screen brightness
- USB
- Fast charging
- Vibration
- Battery information

## Broken:

- Reboot / shutdown: always cycles to eRecovery. (BFM is disabled, so the loader doesn't know that
  boot was successful.
- Audio via wired headset, earpiece, speaker. (Bluetooth audio works. Over USB-C, untested.)
- Cellular. (No RIL yet.)
- SD card.
- Camera
- Fingerprint sensor
- NFC
- Any and all sensors.

---

# Build instructions

## Set up the repo
- `$ repo init -u https://github.com/ironydelerium/platform_manifest`
- `$ repo sync -j 4`

## Build the kernel
- `$ pushd device/huawei/mha-kernel`
- `$ ./make_image.sh`
- `$ popd`

## Put the kernel modules into place
- `$ pushd out/target/product/mha/kernel-output/`
- `$ find . -name '*.ko' -exec cp {} ../../../../../device/huawei/mha/modules/ ';'`
- `$ popd`

## Extract proprietary firmware blobs
- `$ pushd vendor/huawei/mha/proprietary`
- `$ ./mali/extract.sh`
- `$ ./encoders/extract.sh`
- `$ popd`

## Extract GApps
- See vendor/gapps/README.md to download BiTGApps Pie.
- `$ pushd vendor/gapps`
- `$ unzip BiTGApps-arm64-9.x.x-0.1-unsigned.zip`
- `$ popd`

## Make
- `$ . ./build/envsetup.sh`
- `$ lunch mha-eng`
- `$ make -j 4`

## Get the rest of your images
- `$ pushd out/target/product/hi3660`
- `$ touch empty`
- `$ mkbootimg --kernel empty --ramdisk ramdisk.img -o hi3660.boot.img`
- `$ popd`

## Output
Your images are:
- kernel: `out/target/product/mha/kernel-output/kernel.img`
- ramdisk: `out/target/product/hi3660/hi3660.boot.img`
- system: `out/target/product/hi3660/system.img`

## Install:
- Make sure you have your stock images before doing this. The kernel
  built here is specifically tweaked for this ROM, as the graphics blobs
  used above are actually originally for the HiKey960 development board,
  not the ones that are stock on the Mate 9. The hardware is the same,
  but the drivers speak to the kernel using a slightly different variant
  of the Mali interface than is used by the stock kernel. As such, the
  kernel built according to the instructions above will likely not work 
  at all on a stock or Treble ROM.
- Factory reset with stock recovery
- Reboot to fastboot
- `$ fastboot flash kernel out/target/product/mha/kernel-output/kernel.img`
- `$ fastboot flash ramdisk out/target/product/hi3660/hi3660.boot.img`
- `$ fastboot flash system out/target/product/hi3660/system.img`
- `$ fastboot reboot`
- You'll sit for a slightly extended period on the "Your device is not trusted"
  message as compared to other ROMs. This is normal; just let it go. Eventually
  you should see the Android splash screen.

# Magisk
Yes, the ROM is pre-rooted, but only through the shell. Magisk is still recommended 
because it works around a few kinks in the image (the big one being that Developer
Options is completely inaccessible).

## Installing
- Install the latest Magisk Manager (5.9.1 as of this writing).
- `$ adb push out/target/product/hi3660/hi3660.boot.img /data/media/0/Download`
- Open Magisk Manager, select Install -> Patch Boot Image; select 'hi3660.boot.img' 
  from your Downloads folder.
- `$ adb pull /sdcard/Download/patched_boot.img`
- `$ adb reboot`
- It's obnoxious but wait for the system to reboot. It will reboot to eRecovery.
  _This is normal_ for the moment - see the note above under "Broken" for rebooting.
  Make sure you're plugged into your computer and, while holding the volume down key, 
  select "Reboot" when the option appears. Continue holding until the fastboot screen
  appears.
- `$ fastboot flash ramdisk patched_boot.img`
- `$ fastboot reboot`

# TWRP

There is not a current build of TWRP that works with this image. The TWRP
images for MHA that are on XDA will boot, but you'll be staring at the
TWRP splash screen; you won't be able to flash anything.

