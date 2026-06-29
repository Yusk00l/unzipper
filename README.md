# UnZipper

An automated, background background utility designed for Linux Mint that monitors your downloads folder and handles ZIP archives automatically. 

> **Note:** This is my first project of this kind! It was built quickly, but it is fully functional. I plan to add more features in the future.
> **Note:** AI was used to make this Intruction look good as mine looked like a wall of ancient lauguage.
---

## ✨ Features

1. **Auto-Extraction**: Instantly extracts any ZIP file dropped into your `Downloads` folder. 
   * *Disclaimer: Passwords are not supported. Password-protected ZIPs will be skipped.*
2. **Automatic Permissions**: Configures extracted files so they can be read, written, and executed as programs.
   * *Disclaimer: Disabled by default.*
3. **Auto-Clean**: Automatically deletes the original ZIP archive after a successful extraction.

---

## 🚀 Installation & Setup

For the smoothest setup, run these commands **one by one**.

### Step 1: Move the Binary
Create your local bin directory (if it doesn't exist) and move the tool into it:
```bash
mkdir -p ~/.local/bin
mv UnZipperTool ~/.local/bin/
```

### Step 2: Create the Systemd Service
Create the systemd user directory:
```bash
mkdir -p ~/.config/systemd/user
```

Open a new service file using the `nano` text editor:
```bash
nano ~/.config/systemd/user/unzipper.service
```

Paste the following configuration inside the file:
```ini
[Unit]
Description=Automated Zip Extractor and Watcher
After=default.target

[Service]
Type=simple
ExecStart=%h/.local/bin/UnZipperTool
Restart=on-failure
RestartSec=5

[Install]
WantedBy=default.target 
```

**To save and exit Nano:**
1. Press `Ctrl + O`
2. Press `ENTER` to confirm
3. Press `Ctrl + X` to exit

### Step 3: Enable and Start the Service
Load the new service into your system and set it to run automatically on boot:
```bash
systemctl --user daemon-reload
systemctl --user enable unzipper.service
systemctl --user start unzipper.service
```

---

## 🛠️ Managing the Service

Use these commands to monitor or control the background process.

* **Check Service Status**:
  ```bash
  systemctl --user status unzipper.service
  ```
* **View Live Logs** (See when ZIPs are detected and processed):
  ```bash
  journalctl --user -u unzipper.service -f
  ```
* **Temporarily Stop the Service**:
  ```bash
  systemctl --user stop unzipper.service
  ```
* **Disable Service on Boot**:
  ```bash
  systemctl --user disable unzipper.service
  ```

---

## ⚙️ Enabling Automatic Permissions (`--chmod`)

By default, the tool only extracts files. If you want it to automatically set read, write, and executable permissions on unzipped files, you can use the `--chmod` flag.

### Option A: Test Temporarily
Run the tool directly in your current terminal window. *Note: The tool will stop if you close the terminal.*
```bash
# Run with permissions enabled
./UnZipperTool --chmod

# Run without permissions enabled
./UnZipperTool
```

### Option B: Enable Permanently (Recommended)
To make the background service always apply permissions, update the service file:

1. Open the file:
   ```bash
   nano ~/.config/systemd/user/unzipper.service
   ```
2. Replace the contents with this updated configuration (note the `--chmod` added to `ExecStart`):
   ```ini
   [Unit]
   Description=Automated Zip Extractor and Watcher
   After=default.target

   [Service]
   Type=simple
   ExecStart=%h/.local/bin/UnZipperTool --chmod
   Restart=on-failure
   RestartSec=5

   [Install]
   WantedBy=default.target
   ```
3. Save and exit (`Ctrl + O`, `ENTER`, `Ctrl + X`).
4. Apply the changes and restart the background process:
   ```bash
   systemctl --user daemon-reload && systemctl --user restart unzipper.service
   ```
