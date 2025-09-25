

sudo apt install libwayland-dev wayland-protocols

https://gitlab.freedesktop.org/wayland/wayland-protocols/-/tree/main?ref_type=heads

echo $XDG_SESSION_TYPE // make sure it is wayland


systemctl status gdm
nano /etc/gdm3/custom.conf 
waylandenable=true

# freeze gdm

sudo apt purge gdm gdm3
sudo apt install gdm3 ubuntu-desktop
sudo systemctl restart gdm

# 

🔧 Method 1: Temporary (for testing)

Run:

sudo modprobe nvidia NVreg_PreserveVideoMemoryAllocations=1


This loads the driver with the parameter, but it will be lost after reboot.

🔧 Method 2: Persistent via modprobe config

Create a config file:

sudo nano /etc/modprobe.d/nvidia.conf


Add the line:

options nvidia NVreg_PreserveVideoMemoryAllocations=1