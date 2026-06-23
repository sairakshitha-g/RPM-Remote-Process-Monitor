# RPM – Remote Process Monitor

**Real-Time QNX Process Monitoring System**

A mini project built on QNX Neutrino RTOS that lets you monitor and control system processes remotely through a web-based dashboard — no command-line knowledge required.

## Overview

Traditional QNX process monitoring relies on command-line tools like `pidin`, which require manual execution, technical knowledge, and offer no graphical or remote access. RPM solves this by collecting real-time process data (PID, CPU usage, memory usage, thread count, process state) on the QNX target and streaming it to a browser-based dashboard, where users can view live system metrics and terminate processes with a click.

## Features

- 📊 Real-time CPU and memory usage visualization
- 🧾 Live process table with PID, name, CPU%, memory, and thread count
- 🛑 One-click process termination ("Kill") directly from the dashboard
- 🌐 Remote, browser-based access — no SSH or terminal required
- 🌳 Process tree view for parent-child relationships
- 🌙 Dark mode dashboard UI

## System Architecture

```
QNX Operating System (procfs, pidin, system metrics)
        ↓ native QNX APIs
QNX RPM Backend (C application — queries process info, CPU/memory stats)
        ↓ IPC / system calls
WebSocket / HTTP Server (handles requests, maintains live connection)
        ↓ JSON over HTTP/WebSocket
Web UI Layer (HTML / CSS / JavaScript)
        ↓
Web Browser Dashboard (process list, charts, kill controls)
```

## Hardware Requirements

| Component | Purpose |
|---|---|
| Raspberry Pi 4 | Runs QNX Neutrino RTOS; acts as the server |
| microSD card | Stores the QNX OS image |
| 5V Power Adapter | Powers the Raspberry Pi |
| Ethernet Cable | Wired connection between Pi (server) and client |

## Software Requirements

- QNX Neutrino RTOS
- QNX Momentics IDE (with `qcc` cross-compiler toolchain)
- C programming language (backend)
- HTML, CSS, JavaScript, Chart.js (frontend dashboard)

## Project Structure

```
RPM-Remote-Process-Monitor/
├── server/                 # QNX C backend source
│   └── rpm_server.c        # HTTP server, process data collection via pidin/procfs
├── dashboard/              # Web frontend
│   ├── index.html
│   ├── style.css
│   └── script.js
├── docs/                   # Project report and documentation
│   └── Final_Document_project.pdf
├── screenshots/            # Result images from testing
│   ├── 5.1_hardware_setup.png
│   ├── 5.2.1_client_server_program.png
│   ├── 5.2.2_pidin_commands.png
│   ├── 5.2.3_server_waiting.png
│   ├── 5.2.4_dashboard_files.png
│   ├── 5.3.1_cpu_usage_visualization.png
│   ├── 5.3.2_dashboard_analysis.png
│   ├── 5.3.3_process_table.png
│   └── 5.3.4_system_information.png
└── README.md
```

## How It Works

1. The QNX backend server starts and listens on port `8080`.
2. The client (web dashboard) sends an HTTP request to an endpoint (`/processes`, `/info`, or `/kill/<pid>`).
3. The server validates the request and executes the appropriate QNX system call or `pidin` command.
4. Process data is retrieved from the OS and converted into JSON.
5. The JSON response is sent back to the client.
6. The dashboard updates in real time with the latest CPU, memory, and process information.

## Project Output / Screenshots

### Hardware Setup
Raspberry Pi 4 running QNX, connected via Ethernet to the host system.

![Hardware Setup](screenshots/5.1_hardware_setup.png)

### Server Build & Execution (QNX Momentics IDE)
The backend server source code being built and run successfully.

![Client Server Program](screenshots/5.2.1_client_server_program.png)

### Pidin Command Output
Server fetching live process details from the QNX system using `pidin`.

![Pidin Commands](screenshots/5.2.2_pidin_commands.png)

### Server Listening for Requests
Server running and waiting for client connections on port 8080.

![Server Waiting for Request](screenshots/5.2.3_server_waiting.png)

### CPU Usage Visualization
Real-time CPU usage graph on the dashboard, updated continuously from live backend data.

![CPU Usage Visualization](screenshots/5.3.1_cpu_usage_visualization.png)

### Dashboard Analysis View
Memory usage bar, top CPU-consuming processes, and process tree shown together.

![Dashboard Analysis and Visualization](screenshots/5.3.2_dashboard_analysis.png)

### Process Table with Control Actions
Full process list with PID, CPU%, memory, thread count, and a **Kill** button to terminate any process directly from the browser.

![Process Table and Control Functionality](screenshots/5.3.3_process_table.png)

### System Information (QNX Momentics IDE)
CPU utilization and process resource details viewed directly from the IDE's system tools.

![System Information](screenshots/5.3.4_system_information.png)

## Setup & Usage

### 1. Backend (QNX Server)
1. Open the project in **QNX Momentics IDE**.
2. Build the `rpm_server` project using the QNX SDP toolchain.
3. Deploy the compiled binary to the target (Raspberry Pi 4 running QNX).
4. Run the server — it will start listening on port `8080`:
   ```
   Server running on port 8080
   ```

### 2. Frontend (Dashboard)
1. Open `dashboard/index.html` in a web browser.
2. Enter the target device's IP address (e.g., `10.0.0.1`) in the dashboard.
3. The dashboard will connect to the server and start displaying live process data.

## Applications

- Real-time remote monitoring of CPU, memory, and process performance
- Remote debugging and troubleshooting on QNX systems
- Fault detection through abnormal process behavior alerts
- Monitoring across multiple distributed QNX devices
- Resource optimization by identifying high-usage processes

## Future Scope

- AI-based anomaly detection and failure prediction
- Cloud-based centralized monitoring for multiple devices
- Authentication and encryption for secure access
- Mobile app access and historical data analysis

## Team

| Name | Roll No. |
|---|---|
| T. Sumana Sri | 23251A04F9 |
| G. Sai Rakshitha | 23251A04G3 |
| K. Rupa Sri | 23251A04H3 |
| V. Indhu | 23251A04J9 |
| M. Sahithi | 24255A0416 |

**Guide:** Dr. B. Pavani, Assistant Professor, ECE Dept, GNITS

Department of Electronics & Communication Engineering
G. Narayanamma Institute of Technology & Science (For Women), Hyderabad
Academic Year 2025–26

## License

This project was developed for academic purposes as part of a B.Tech mini project (Mini Project 2).
