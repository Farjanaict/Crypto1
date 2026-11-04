# OSI Communication Simulator

An educational OSI 7-layer simulation inspired by the structure and requirements of the instructor-provided repository.

## Features
- C-based TCP client and server using real socket communication.
- Seven simulated encapsulation headers on the sender.
- Seven-layer decapsulation on the receiver.
- Python terminal visualizer showing the zig-zag flow.
- Progress checkpoints after every two layers.
- Windows (MinGW) and Linux build instructions.

## Project structure

```text
OSI_Communication_Simulator/
├── src/
│   ├── client.c
│   ├── server.c
│   ├── layers.c
│   └── layers.h
├── python/
│   └── visualizer.py
├── tests/
│   └── test_visualizer.py
├── Makefile
├── build_windows.bat
├── run_visualizer.bat
├── OSI_Model.md
└── README.md
```

## Requirements
- GCC compiler (MinGW-w64 on Windows)
- Python 3.8 or newer
- No third-party Python package is required.

## Windows setup (VS Code)

1. Install GCC/MinGW-w64 and make sure `gcc` is added to PATH.
2. Install Python and make sure `python` works in the VS Code terminal.
3. Open this project folder in VS Code.
4. Open Terminal -> New Terminal.
5. Build both C programs:

```bat
build_windows.bat
```

If you prefer to compile manually:

```bat
gcc src\server.c src\layers.c -o server.exe -lws2_32
gcc src\client.c src\layers.c -o client.exe -lws2_32
```

### Run the real client-server communication

Open **Terminal 1** and run:

```bat
server.exe
```

Open **Terminal 2** and run:

```bat
client.exe "Hello from my OSI project"
```

The server terminal displays the received packet and removes the seven simulated headers to recover the original message. Keep the server terminal open until the client connects.

### Run the Python zig-zag visualizer

In another terminal:

```bat
python python\visualizer.py
```

Or provide your own message:

```bat
python python\visualizer.py --message "My networking assignment"
```

The visualizer groups progress after every two layers. It is a companion visualization; the actual message transfer is performed by the C TCP client/server.

## Linux / Ubuntu

```bash
gcc src/server.c src/layers.c -o server
gcc src/client.c src/layers.c -o client
```

Terminal 1:

```bash
./server
```

Terminal 2:

```bash
./client "Hello from my OSI project"
```

Python visualizer:

```bash
python3 python/visualizer.py --message "Hello from Ubuntu"
```

## How the simulation works

Sender encapsulation:

`Application -> Presentation -> Session -> Transport -> Network -> Data Link -> Physical`

The client wraps the original message in seven educational headers (`L7[...]` through `L1[...]`) and sends the result through a TCP socket to `127.0.0.1:5055`.

Receiver decapsulation:

`Physical -> Data Link -> Network -> Transport -> Session -> Presentation -> Application`

The server removes the headers in reverse order and prints the recovered original message.

## Important networking note

This project **simulates** the responsibilities of all seven OSI layers in application code. The TCP socket is real, but the added OSI headers for Physical, Data Link, Network, Session, Presentation, and Application are educational simulation headers; they are not actual Ethernet/IP/TCP headers created by the operating system. TCP/IP does not map one-to-one to all seven OSI layers.

## Submission checklist

- [ ] Build the C client and server successfully.
- [ ] Run the server and client in separate terminals.
- [ ] Confirm that the server prints the original message.
- [ ] Run the Python visualizer.
- [ ] Add your own project files to your own GitHub repository.
- [ ] Take a screenshot showing the running project and your Windows username or the date/time.
- [ ] Submit your own GitHub repository URL and screenshot.

To show your Windows username and date/time in Command Prompt:

```bat
whoami
date /t
time /t
```

Do not submit the instructor's repository URL as your own repository.
