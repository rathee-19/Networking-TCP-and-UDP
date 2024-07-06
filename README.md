# Networking Enhancements: TCP and UDP

## Overview

This project involves implementing various networking functionalities using TCP and UDP protocols, with a focus on practical applications and understanding underlying concepts. The project is divided into two parts: basic TCP/UDP communication and a simulated TCP-like protocol using UDP sockets.

## Part A: Using Library Functions

### Basic Programs

### TCP Server Program

A simple TCP server that listens for incoming connections, receives a message from the client, and sends a response back. It ensures all error checks are handled.

### TCP Client Program

A TCP client that connects to the TCP server, sends a message, and waits for a response. It handles all possible errors and ensures smooth communication.

### UDP Server Program

A UDP server that waits for messages from clients and sends responses. It includes robust error checking to handle any potential issues.

### UDP Client Program

A UDP client that sends messages to the server and waits for a response. It checks for errors and ensures reliable message delivery.

# Rock, Paper, Scissors Game

### TCP Implementation

- **Server:** Listens for two clients on different ports. Receives their choices (Rock, Paper, Scissors), determines the result, and sends the result back to both clients. Handles multiple rounds based on client responses.
- **Client A & B:** Connects to the server, sends their choice, receives the result, and prompts the user for another game based on server instructions.

### UDP Implementation

- **Server:** Similar to the TCP server but uses UDP sockets. Manages game logic and communicates results to clients.
- **Client A & B:** Uses UDP sockets to communicate with the server, handles user input, and displays the results. Manages multiple rounds of the game based on server instructions.

# Part B: Implementing TCP Functionality Using UDP

### Data Sequencing

The client/server divides data into smaller chunks, assigns sequence numbers, and sends these chunks along with the total number of chunks. The receiver reassembles the chunks based on sequence numbers.

### Retransmissions

The receiver sends an ACK packet for each received chunk. If the sender does not receive an ACK within a certain timeframe, it retransmits the chunk. This ensures reliable data transmission over UDP.

## Report

### Differences from Traditional TCP

1. **SYN Bit:** Traditional TCP uses a SYN bit to establish connections. In this implementation, the number of packets serves this purpose.
2. **Window Size:** Traditional TCP uses a sliding window for flow control. Here, packets are sent regardless of ACK reception, then retransmitted based on missing ACKs.
3. **Packet Size:** Fixed packet sizes are used, unlike variable sizes in TCP.
4. **Communication:** One-sided communication at any time, whereas TCP allows concurrent send/receive operations.

### Extending to Account for Flow Control

Flow control in TCP regulates data transmission to match the receiver's capacity using a sliding window mechanism. Here’s how to extend this implementation:

1. **Counter:** Introduce a counter to manage packet transmission.
2. **Batch Transmission:** Send a batch of packets, then wait for ACKs before proceeding.
3. **ACK Check:** Ensure ACKs are received for each batch before sending more packets.
4. **Retry Mechanism:** Resend unacknowledged packets based on the ACK check.

## Resources
-[Insturctions](https://karthikv1392.github.io/cs3301_osn/mini-projects/mp2)

- [Socket Programming Slides](https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf)
- [TCP Client-Server Example](https://github.com/nikhilroxtomar/TCP-Client-Server-Implementation-in-C)
- [UDP Client-Server Example](https://github.com/nikhilroxtomar/UDP-Client-Server-implementation-in-C)
- Man Pages
- [Level-IP: TCP/IP Stack from Scratch](https://github.com/saminiir/level-ip)