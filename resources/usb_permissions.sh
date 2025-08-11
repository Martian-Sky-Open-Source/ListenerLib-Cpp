# This will add usb permissions to /etc/udev/rules.d/
#   for mojave and realsense
# To run:
# sudo bash all_usb_permissions.sh


rm -rf /etc/udev/rules.d/99-serial.rules 
touch /etc/udev/rules.d/99-serial.rules 

cat <<EOF >/etc/udev/rules.d/99-serial.rules 
KERNEL=="ttyUSB[0-9]*",MODE="0666"
KERNEL=="ttyACM[0-9]*",MODE="0666"
EOF

touch /etc/udev/rules.d/99-serial.rules 

cat <<EOF >/etc/udev/rules.d/99-realsense-libusb.rules
##Version=1.1##
# Device rules for Intel RealSense devices (R200, F200, SR300 LR200, ZR300, D400, L500, T200)
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0a80", MODE:="0666", GROUP:="plugdev", RUN+="/usr/local/bin/usb-R200-in_udev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0a66", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0aa3", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0aa2", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0aa5", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0abf", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0acb", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0ad0", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="04b4", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0ad1", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0ad2", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0ad3", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0ad4", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0ad5", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0ad6", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0af2", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0af6", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0afe", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0aff", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b00", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b01", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b03", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b07", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b0c", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b0d", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b3a", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b3d", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b48", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b49", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b4b", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b4d", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b52", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b56", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b5b", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b5c", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b64", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b68", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b6a", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b6b", MODE:="0666", GROUP:="plugdev"

# Intel RealSense recovery devices (DFU)
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0ab3", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0adb", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0adc", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0ade", MODE:="0666", GROUP:="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b55", MODE:="0666", GROUP:="plugdev"

# Intel RealSense devices (Movidius, T265)
SUBSYSTEMS=="usb", ENV{DEVTYPE}=="usb_device", ATTRS{idVendor}=="8087", ATTRS{idProduct}=="0af3", MODE="0666", GROUP="plugdev"
SUBSYSTEMS=="usb", ENV{DEVTYPE}=="usb_device", ATTRS{idVendor}=="8087", ATTRS{idProduct}=="0b37", MODE="0666", GROUP="plugdev"
SUBSYSTEMS=="usb", ENV{DEVTYPE}=="usb_device", ATTRS{idVendor}=="03e7", ATTRS{idProduct}=="2150", MODE="0666", GROUP="plugdev"

KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0ad5", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor_custom", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0ad5", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0af2", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0af2", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0afe", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor_custom", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0afe", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0aff", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor_custom", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0aff", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b00", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor_custom", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b00", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b01", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor_custom", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b01", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b3a", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b3a", RUN+="/bin/sh -c ' chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b3d", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b3d", RUN+="/bin/sh -c ' chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b4b", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b4b", RUN+="/bin/sh -c ' chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b4d", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b4d", RUN+="/bin/sh -c ' chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b56", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b56", RUN+="/bin/sh -c ' chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b5b", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b5b", RUN+="/bin/sh -c ' chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b5c", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b5c", RUN+="/bin/sh -c ' chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b64", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b64", RUN+="/bin/sh -c ' chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b68", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b68", RUN+="/bin/sh -c ' chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b6a", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b6a", RUN+="/bin/sh -c ' chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
KERNEL=="iio*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b6b", MODE:="0777", GROUP:="plugdev", RUN+="/bin/sh -c 'chmod -R 0777 /sys/%p'"
DRIVER=="hid_sensor*", ATTRS{idVendor}=="8086", ATTRS{idProduct}=="0b6b", RUN+="/bin/sh -c ' chmod -R 0777 /sys/%p && chmod 0777 /dev/%k'"
EOF

rm -rf /etc/udev/rules.d/10-royale-ubuntu.rules
touch /etc/udev/rules.d/10-royale-ubuntu.rules

cat <<EOF >/etc/udev/rules.d/10-royale-ubuntu.rules
# This is a rules file for Linux's udev.  When a USB camera is plugged in,
# it will allow all users in the "plugdev" group to access the device.
#
# Put this file in /etc/udev/rules.d/

# PicoS
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c00f", MODE="0666", SYMLINK+="camboard_pico_7119k-%k", GROUP="plugdev"

# PicoFlexx
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c012", MODE="0666", SYMLINK+="camboard_pico_flexx-%k", GROUP="plugdev"

# Eval Board
SUBSYSTEMS=="usb", ATTRS{idVendor}=="058b", ATTRS{idProduct}=="00a0", MODE="0666", SYMLINK+="camboard_evalboard-%k", GROUP="plugdev"

# UVC C2
SUBSYSTEMS=="usb", ATTRS{idVendor}=="058b", ATTRS{idProduct}=="00a7", MODE="0666", SYMLINK+="camboard_uvc_c2-%k", GROUP="plugdev"

# Animator Board UVC
SUBSYSTEMS=="usb", ATTRS{idVendor}=="058b", ATTRS{idProduct}=="00a6", MODE="0666", SYMLINK+="camboard_animatorboard_uvc-%k", GROUP="plugdev"

# Animator Board UVC for MiraLonga
SUBSYSTEMS=="usb", ATTRS{idVendor}=="058b", ATTRS{idProduct}=="00a8", MODE="0666", SYMLINK+="camboard_animatorboard_m2459_uvc-%k", GROUP="plugdev"

# Animator Board Amundsen
SUBSYSTEMS=="usb", ATTRS{idVendor}=="058b", ATTRS{idProduct}=="00a1", MODE="0666", SYMLINK+="camboard_animatorboard-%k", GROUP="plugdev"

# Skylla
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c013", MODE="0666", SYMLINK+="camboard_uvc_skylla-%k", GROUP="plugdev"

# Charybdis
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c014", MODE="0666", SYMLINK+="camboard_uvc_charybdis-%k", GROUP="plugdev"

# PicoMaxx
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c016", MODE="0666", SYMLINK+="camboard_pico_maxx1-%k", GROUP="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c017", MODE="0666", SYMLINK+="camboard_pico_maxx2-%k", GROUP="plugdev"

# PicoMonstar
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c018", MODE="0666", SYMLINK+="camboard_pico_monstar1-%k", GROUP="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c019", MODE="0666", SYMLINK+="camboard_pico_monstar2-%k", GROUP="plugdev"

# Daedalus
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c01a", MODE="0666", SYMLINK+="camboard_uvc_daedalus-%k", GROUP="plugdev"

# Willingen
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c01c", MODE="0666", SYMLINK+="willingen-%k", GROUP="plugdev"

# Rabenscheid
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c01d", MODE="0666", SYMLINK+="rabenscheid-%k", GROUP="plugdev"

# Alea
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c01e", MODE="0666", SYMLINK+="camboard_uvc_alea-%k", GROUP="plugdev"

# Apollo
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c01f", MODE="0666", SYMLINK+="apollo-%k", GROUP="plugdev"

# Skylla non-UVC
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c020", MODE="0666", SYMLINK+="skylla_non_uvc-%k", GROUP="plugdev"

# Charybdis non-UVC
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c021", MODE="0666", SYMLINK+="charybdis_non_uvc-%k", GROUP="plugdev"

# Daedalus non-UVC
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c022", MODE="0666", SYMLINK+="daedalus_non_uvc-%k", GROUP="plugdev"

# Alea non-UVC
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c023", MODE="0666", SYMLINK+="alea_non_uvc-%k", GROUP="plugdev"

# PicoMaxx non-UVC
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c024", MODE="0666", SYMLINK+="camboard_pico_maxx_non_uvc-%k", GROUP="plugdev"

# PicoMonstar non-UVC
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c025", MODE="0666", SYMLINK+="camboard_pico_monstar_non_uvc-%k", GROUP="plugdev"

# Apollo non-UVC
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c027", MODE="0666", SYMLINK+="apollo-%k", GROUP="plugdev"

# Salome
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c028", MODE="0666", SYMLINK+="salome-%k", GROUP="plugdev"

# FacePlus / Lewis non-UVC
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c02a", MODE="0666", SYMLINK+="faceplus_non_uvc-%k", GROUP="plugdev"

# FacePlus / Lewis UVC
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c02b", MODE="0666", SYMLINK+="faceplus_uvc-%k", GROUP="plugdev"

# Equinox Amundsen
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c02c", MODE="0666", SYMLINK+="equinox-%k", GROUP="plugdev"

# x1 Amundsen
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c02d", MODE="0666", SYMLINK+="x1-%k", GROUP="plugdev"

# f1 Amundsen
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c02e", MODE="0666", SYMLINK+="f1-%k", GROUP="plugdev"

# m1 Amundsen
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c02f", MODE="0666", SYMLINK+="m1-%k", GROUP="plugdev"

# U7 Amundsen
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c030", MODE="0666", SYMLINK+="u7-%k", GROUP="plugdev"

# Selene
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c031", MODE="0666", SYMLINK+="selene-%k", GROUP="plugdev"

# Orpheus
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c032", MODE="0666", SYMLINK+="orpheus-%k", GROUP="plugdev"

# pmd Module
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c033", MODE="0666", SYMLINK+="pmdmodule-%k", GROUP="plugdev"

# Gaia
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c034", MODE="0666", SYMLINK+="gaia-%k", GROUP="plugdev"

# Byt
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c035", MODE="0666", SYMLINK+="byt-%k", GROUP="plugdev"

# Holkin
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c036", MODE="0666", SYMLINK+="holkin-%k", GROUP="plugdev"

# pmd Module 277x
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c039", MODE="0666", SYMLINK+="pmdmodule277x-%k", GROUP="plugdev"

# x1.1 Amundsen
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c03a", MODE="0666", SYMLINK+="x1.1-%k", GROUP="plugdev"

# pmd Module 2877
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c040", MODE="0666", SYMLINK+="pmdmodule2877-%k", GROUP="plugdev"

# pmd Module 2875
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c041", MODE="0666", SYMLINK+="pmdmodule2875-%k", GROUP="plugdev"

# pmd Module 2977
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c042", MODE="0666", SYMLINK+="pmdmodule2977-%k", GROUP="plugdev"

# pmd Module 2877A
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c043", MODE="0666", SYMLINK+="pmdmodule2877A-%k", GROUP="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c045", MODE="0666", SYMLINK+="pmdmodule2877A-%k", GROUP="plugdev"

# pmd Module 2977 with 4-lane support
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c044", MODE="0666", SYMLINK+="pmdmodule2977_4lane-%k", GROUP="plugdev"

# Flexx2VGA
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c047", MODE="0666", SYMLINK+="pmd_Flexx2VGA-e%k", GROUP="plugdev"

# Flexx2Wide
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1c28", ATTRS{idProduct}=="c048", MODE="0666", SYMLINK+="pmd_Flexx2Wide-e%k", GROUP="plugdev"
EOF

# udevadm control --reload-rules
# udevadm trigger
# tee /sys/module/usbcore/parameters/usbfs_memory_mb >/dev/null <<<0
