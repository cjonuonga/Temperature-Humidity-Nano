// Copy this file to "secrets.h" (same folder) and fill in your own values.
// secrets.h is in .gitignore so your Wi-Fi password never ends up on GitHub.

#define WIFI_SSID     "your-wifi-name"
#define WIFI_PASSWORD "your-wifi-password"

// Replace 192.168.1.50 with your smart plug's IP address
// (find it in your router's device list or the plug's app).
// Use the pair that matches your plug and delete the others.

// Shelly Plus / Plug S Gen2 / Gen3 (newer Shelly plugs)
#define PLUG_ON_URL  "http://192.168.1.50/rpc/Switch.Set?id=0&on=true"
#define PLUG_OFF_URL "http://192.168.1.50/rpc/Switch.Set?id=0&on=false"

// Shelly Plug / Plug S Gen1 (older Shelly plugs)
// #define PLUG_ON_URL  "http://192.168.1.50/relay/0?turn=on"
// #define PLUG_OFF_URL "http://192.168.1.50/relay/0?turn=off"

// Any plug flashed with Tasmota firmware
// #define PLUG_ON_URL  "http://192.168.1.50/cm?cmnd=Power%20On"
// #define PLUG_OFF_URL "http://192.168.1.50/cm?cmnd=Power%20Off"
