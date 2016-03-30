<div id="table-of-contents">
<h2>Table of Contents</h2>
<div id="text-table-of-contents">
<ul>
<li><a href="#orgheadline1">1. Abstract Leap Motion Server</a></li>
<li><a href="#orgheadline3">2. Prerequisites</a>
<ul>
<li><a href="#orgheadline2">2.1. Download and install the LeapMotion SDK</a></li>
</ul>
</li>
<li><a href="#orgheadline4">3. Installation</a></li>
<li><a href="#orgheadline5">4. Usage</a></li>
</ul>
</div>
</div>


# Abstract Leap Motion Server<a id="orgheadline1"></a>

# Prerequisites<a id="orgheadline3"></a>

## Download and install the [LeapMotion SDK](https://developer.leapmotion.com/get-started)<a id="orgheadline2"></a>

# Installation<a id="orgheadline4"></a>

-   Clone the project
-   `cd Abstraction-LeapMotionServer`
-   `make`

# Usage<a id="orgheadline5"></a>

-   Server side
    -   Connect your Leap Motion and run `sudo leapd`
    -   `./bin/LeapServer <nb_client> <port>`
-   Client side
    -   `C/C++`
        
            #include "socklib" // disponible dans /include
            
            int main() {
                 int socketClient = CreeSocketClient ("127.0.0.1", "2222");
            
                 if (socketClient == -1) {
                      fprintf(stderr,"erreur de connexion sur la socket");
                      exit (1)
                 }
            
                 //bla bla
            
                 return 0;
            }
    -   `JAVA`
        
            import java.net.*;
            import java.io.*;
            
            public class Client {
                public static void main(String[] args) {
                    try {
                        Socket socket = new Socket ("127.0.0.1", 2222);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
