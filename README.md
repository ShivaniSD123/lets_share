<h1>lets_share</h1>

<h2>Description</h2>
<p>This project is a simple peer-to-peer file sharing system built entirely in C++ using TCP sockets.
It allows one system to act as a server and another as a client, enabling reliable transfer of files (like PDFs, images, or text files) over a local network.
<br>
The system demonstrates low-level network programming concepts, including:
<ul>
<li>
Socket creation and binding</li>
<li>
Connection handling (server–client model)</li>
<li>
Binary data transfer</li>
<li>
File I/O handling and buffer management</li>
</ul>

<p>
<h2>Features</h2>
<ul>
<li>
Transfer any file (binary or text) between two systems</li>
<li>Uses TCP for reliable delivery (no packet loss or corruption)</li>
<li>Works across LAN (by using server’s local IP address)</li>
<li>Automatic file creation and saving on receiver’s end</li>
<li>Minimal command-line interface — clean and easy to use</li>
</p>
<h2>How it Works</h2>
<p>
<h4>Server Side</h4>
<ul>
<li>
Listens on a specific port (1234 by default).</li>
<li>
Waits for a client connection.</li>
<li>
Receives the file name and file data in binary format.</li>
<li>
Saves the received file with a _copied suffix.</li>
</ul>
</p>

<h4>Client Side</h4>
<p>
<ul>
<li>
Connects to the server using its IP address and port.</li>
<li>
Sends the file name, file size, and file contents.</li>
<li>
Can send any file accessible by its absolute path.</li>
</ul>
</p>
<p>
<h2>Requirements</h2>
<p>
<ul>
<li>C++17 or later</li>
<li>Linux/Unix environment (uses sys/socket.h, netinet/in.h>, etc.)</li>

<li>Basic build tools (g++)</li>
</ul>
</p>
<p>
<h2>Further Improvements</h2>
<ul>
<li>
Support for multiple simultaneous clients</li>
<li>
Progress bar for file transfers</li>
<li>
Resume capability for interrupted transfers</li>
<li>
Encryption or authentication for secure sharing</li>
</ul>
</p>