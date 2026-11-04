# OSI Model - Seven Layers

The Open Systems Interconnection (OSI) model explains network communication using seven conceptual layers.

| Layer | Name | Main responsibility | Example |
|---|---|---|---|
| 7 | Application | Provides network services to user applications | HTTP, DNS |
| 6 | Presentation | Data format, encoding, compression, encryption | UTF-8, TLS concepts |
| 5 | Session | Establishes, manages, and ends communication sessions | Session control |
| 4 | Transport | End-to-end delivery, segmentation, reliability, ports | TCP, UDP |
| 3 | Network | Logical addressing and routing between networks | IP |
| 2 | Data Link | Framing, MAC addressing, local-link error detection | Ethernet |
| 1 | Physical | Sends raw bits over a physical medium | Cable, radio signal |

## Sender: Encapsulation

At the sender, data moves from Layer 7 down to Layer 1. Each layer adds control information (often represented as a header).

## Receiver: Decapsulation

At the receiver, data moves from Layer 1 up to Layer 7. Each layer processes and removes its corresponding control information.

## Memory trick

Top to bottom: All People Seem To Need Data Processing.
