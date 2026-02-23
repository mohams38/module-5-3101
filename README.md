# module-5-3101
# Programming Assignment 5 - Module 5 Networking Using C

## Problem Statement
For this assignment, I had to build a client-server program in C using socket programming. The server has to accept a filename from the client and send back the file contents as text. The file content cannot be more than 255 characters. The program also has to handle errors the right way, like invalid file names, file not found, and files that are too large.

## Describe the Solution
My solution uses a TCP server and client written in C. The server listens on port 9001, waits for a client to connect, and then receives a filename request. After that, the server checks if the filename is valid. If the filename is invalid (like using `..`), it sends back an error message. If the file does not exist, it sends a file not found error. If the file is bigger than 255 characters, it sends a file size error.

If the file is valid and within the size limit, the server reads the file and sends the contents back to the client. On the client side, the program connects to the server, asks the user to enter a filename, sends that filename, and prints whatever response comes back from the server. I also made the server run in a loop so it can keep handling requests without needing to restart each time.

## Pros and Cons of my solution

### Pros
- Meets the assignment requirements
- Uses C socket programming for client-server communication
- Handles the required error cases
- Easy to test on my computer
- Server keeps running for multiple requests

### Cons
- Only handles one client at a time
- Only supports text files up to 255 characters


## Screenshots 
- A successful request using `test.txt`
- File not found error using `missing.txt`
- Invalid filename error using `../secret.txt`
- File size error using `big.txt`
