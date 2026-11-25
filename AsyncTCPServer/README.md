# Multithreaded Asynchronous TCP Server

A multithreaded asynchronous TCP chat server built in **C++20** using **Boost.Asio**.

The server supports multiple simultaneous client connections, username registration, public and private messaging, command processing, and graceful shutdown. It demonstrates asynchronous network programming, multithreading, synchronization, and safe resource management in C++.

## Features

- **Asynchronous TCP Server:** Accepts multiple client connections using `async_accept()`.
- **Configurable Multithreading:** Runs a shared `io_context` across a configurable number of worker threads.
- **Client Sessions:** Manages each client independently using asynchronous read and write operations.
- **Username Registration:** Validates usernames and prevents duplicate registrations.
- **Broadcast Messaging:** Delivers public messages to all other registered clients.
- **Private Messaging:** Sends messages to specific users.
- **Client Commands:** Supports `/users`, `/quit`, and `/msg`.
- **Asynchronous Write Queue:** Preserves outgoing message order and prevents overlapping writes on the same socket.
- **Thread Safety:** Uses Boost.Asio strands and `std::mutex` to synchronize concurrent operations.
- **Graceful Shutdown:** Stops accepting new connections, requests active sessions to close, and waits for worker threads to finish.
- **Automated Testing:** Includes Boost.Test cases covering registration, commands, messaging, message ordering, and concurrent client operations.

## Technologies Used

| Technology           | Purpose                                  |
| -------------------- | ---------------------------------------- |
| C++20                | Core implementation                      |
| Boost.Asio           | Asynchronous TCP networking              |
| Boost.Test           | Automated testing                        |
| `std::thread`        | Multithreaded execution                  |
| `std::mutex`         | Synchronization of shared data           |
| Boost.Asio Strand    | Serialized asynchronous handlers         |
| Smart Pointers       | Object lifetime and ownership management |
| `std::unordered_map` | Registered client storage                |
| `std::deque`         | Outgoing message queue                   |
| `std::string_view`   | Efficient read-only string parameters    |
| `std::format`        | Message formatting                       |

## Project Structure

The main application is organized into the following components:

```text
AsyncTCPServer/
├── AsyncTCPServer.cpp
├── TcpServer.h
├── TcpServer.cpp
├── ClientSession.h
├── ClientSession.cpp
├── ClientManager.h
├── ClientManager.cpp
├── CommandHandler.h
├── CommandHandler.cpp
└── AsyncTCPServerTests/
    ├── ClientManagerTests.cpp
    ├── CommandHandlerTests.cpp
    └── NetworkingTests.cpp
```

The `Tests` directory is a logical grouping of the supplied test files; the actual Visual Studio project layout may differ.

## Architecture

The server separates responsibilities into four main components.

### TcpServer

Responsible for accepting TCP connections and managing the server lifecycle.

- Listens on the configured port.
- Accepts connections asynchronously.
- Creates a `ClientSession` for each accepted connection.
- Tracks active sessions using weak pointers.
- Serializes acceptor operations through an accept strand.
- Stops accepting new connections and requests session shutdown.

### ClientSession

Manages communication with an individual client.

- Owns the client's TCP socket.
- Requests and validates a username.
- Reads newline-delimited messages asynchronously.
- Processes commands and forwards public messages.
- Maintains a FIFO outgoing message queue.
- Uses a strand to serialize session operations.
- Closes the socket and unregisters the client when disconnected.

### ClientManager

Manages the shared registry of connected users.

- Registers clients with unique usernames.
- Removes disconnected clients.
- Returns the list of registered usernames.
- Broadcasts messages to other connected clients.
- Routes private messages to specific recipients.
- Protects the shared client registry with a mutex.

### CommandHandler

Parses client commands independently of networking logic.

Supported command types:

- `Users`
- `Quit`
- `PrivateMessage`
- `Invalid`

## Communication Flow

1. The server starts listening for TCP connections.
2. A client connects to the configured port.
3. `TcpServer` creates a dedicated `ClientSession`.
4. The session prompts the client to enter a username.
5. `ClientManager` validates username uniqueness during registration.
6. The client sends public messages or executes supported commands.
7. Outgoing messages are queued and sent asynchronously.
8. When a client disconnects, its session is closed and the username is removed from the registry.

## Requirements

- Windows with Visual Studio and C++ development tools
- C++20-compatible compiler and standard library
- Boost C++ Libraries
- Boost.Asio
- Boost.Test for automated testing

## Building and Running

### 1. Clone the repository

```bash
git clone -b boost-asio-tcp-server https://github.com/ShorouqDerieh/Asal-Internship.git
```

### 2. Configure the project

Open the Visual Studio solution and ensure that:

- The project uses the **C++20** language standard.
- Boost header directories are configured.
- Any required Boost library directories and dependencies are configured.

### 3. Build

Build the server project using Visual Studio.

### 4. Run

The application requires two command-line arguments:

```text
AsyncTCPServer <port> <threads>
```

Example:

```text
AsyncTCPServer.exe 9091 4
```

This starts the server on TCP port `9091` using four worker threads.

| Argument  | Description              | Valid Range |
| --------- | ------------------------ | ----------- |
| `port`    | TCP listening port       | 1–65535     |
| `threads` | Number of worker threads | 1–64        |

Invalid arguments are rejected with an error message.

### 5. Stop the Server

Press **Enter** in the server console to initiate shutdown.

The server closes its acceptor, requests active client sessions to stop, and joins the worker threads before exiting.

## Username Registration

When a client connects, the server sends:

```text
Enter your username:
```

A valid username must:

- Contain 1–20 characters.
- Use English letters (`A–Z`, `a–z`), digits (`0–9`), or underscores (`_`).
- Be unique among currently registered clients.

Example:

```text
Enter your username:
Alice
Username registered successfully.
```

Duplicate names are rejected:

```text
Username already taken. Try another name.
```

## Client Commands

| Command                     | Description                               |
| --------------------------- | ----------------------------------------- |
| `/users`                    | Display connected usernames               |
| `/quit`                     | Disconnect from the server                |
| `/msg <username> <message>` | Send a private message to a specific user |

Invalid commands return:

```text
Invalid command.
```

Sending a private message to an unknown username returns:

```text
User not found.
```

## Example Communication

**Alice connects:**

```text
Enter your username:
Alice
Username registered successfully.
```

**Bob connects:**

```text
Enter your username:
Bob
Username registered successfully.
```

**Alice broadcasts a message:**

```text
Hello everyone!
```

**Bob receives:**

```text
Alice: Hello everyone!
```

**Alice sends a private message:**

```text
/msg Bob Hello Bob!
```

**Bob receives:**

```text
[Private] Alice: Hello Bob!
```

**Alice requests connected users:**

```text
/users
```

Example response:

```text
Connected users:
- Alice
- Bob
```

The order of users is not guaranteed because the registry uses `std::unordered_map`.

## Connecting with PowerShell

Open a PowerShell terminal and create a TCP connection:

```powershell
$client = [System.Net.Sockets.TcpClient]::new()
$client.Connect("127.0.0.1", 9091)

$stream = $client.GetStream()
$reader = [System.IO.StreamReader]::new($stream)
$writer = [System.IO.StreamWriter]::new($stream)
$writer.AutoFlush = $true
```

Read the registration prompt:

```powershell
$reader.ReadLine()
```

Register a username:

```powershell
$writer.WriteLine("Alice")
$reader.ReadLine()
```

Send a public message:

```powershell
$writer.WriteLine("Hello everyone!")
```

Send a private message:

```powershell
$writer.WriteLine("/msg Bob Hello!")
```

Request connected users:

```powershell
$writer.WriteLine("/users")
```

Read incoming messages:

```powershell
$reader.ReadLine()
```

Each call reads one line and waits until data is available.

Multiple PowerShell terminals can be used to simulate multiple simultaneous clients.

## Thread Safety

### Shared io_context

All worker threads execute handlers associated with the same `boost::asio::io_context`, allowing asynchronous operations to be processed concurrently.

### Strands

Each `ClientSession` uses a strand to serialize handlers that access session-specific state.

`TcpServer` also uses an accept strand to coordinate acceptor operations and shutdown.

### Mutex

`ClientManager` uses `std::mutex` to protect the shared client registry during registration, lookup, removal, and recipient selection.

### Snapshot-Based Messaging

Broadcast and private-message operations select recipients while holding the mutex and release the lock before invoking `send()`.

This reduces lock contention and avoids holding the client-registry mutex while scheduling session operations.

### Asynchronous Write Queue

Each client session maintains a `std::deque<std::string>` for outgoing messages.

Only one asynchronous write is active per session at a time. When a write completes, the next queued message is sent.

## Graceful Shutdown

The server supports shutdown initiated from the console.

When Enter is pressed:

1. `TcpServer::stop()` schedules shutdown through the accept strand.
2. The server stops accepting new connections.
3. The listening acceptor is closed.
4. Active sessions receive shutdown requests.
5. Sessions close their sockets and unregister users.
6. Pending asynchronous operations are canceled or completed.
7. Worker threads finish and are joined by the main thread.

## Testing

The project includes automated tests using **Boost.Test**.

### ClientManagerTests.cpp

Covers:

- Client registration
- Duplicate username rejection
- Client removal
- Username registration after removal
- Concurrent registration and removal across multiple threads

### CommandHandlerTests.cpp

Covers:

- `/users` command parsing
- `/quit` command parsing
- Private-message command parsing
- Unknown commands
- Missing usernames
- Missing private-message content
- Unexpected command arguments

### NetworkingTests.cpp

Covers:

- Username registration and validation over TCP
- Duplicate username handling over TCP
- Message broadcasting
- Private messaging
- Unknown private-message recipients
- Asynchronous write queue ordering

Networking tests use local loopback TCP connections to exercise client-session communication.

### Running Tests

Build and execute the dedicated Boost.Test target in Visual Studio.

The command-handler test file defines the Boost.Test module and provides the included test runner. The remaining test files register additional test cases.

## Key Concepts Demonstrated

- Asynchronous and event-driven programming
- TCP client-server communication
- Multithreading and thread synchronization
- Resource Acquisition Is Initialization (RAII)
- Smart pointer ownership and lifetime management
- `std::enable_shared_from_this`
- Move semantics
- Mutexes and strands
- Asynchronous message queues
- Separation of responsibilities
- Automated unit and networking tests
