<div id="table-of-contents">
<h2>Table of Contents</h2>
<div id="text-table-of-contents">
<ul>
<li><a href="#orgheadline1">1. Abstract Leap Motion Server</a></li>
<li><a href="#orgheadline2">2. Prerequisites</a></li>
<li><a href="#orgheadline3">3. Installation</a></li>
<li><a href="#orgheadline4">4. Usage</a></li>
</ul>
</div>
</div>

# Abstraction Controller Server<a id="orgheadline1"></a>
This project aims to provide a generic abstraction of any existing controller. The client-side code should keep a minimum size, as it only sees a generic joystick : the server, on which all the drivers are installed, retrieves and translates all the data from the controller into buttons and axes.

Currently, the only implemented controller is the Leap Motion.
We are working right now on at least <a href="http://compass.microsoft.com/assets/f2/28/f228e338-5ecb-4dae-8cf6-f3f2a6c98de6.jpg?n=pop1.jpg">three</a> <a href="https://content.ebgames.com.au/website/images/detail/153839_detail.jpg">other</a> <a href="http://cdn.ubergizmo.com/photos/2010/6/dj-hero-2-coming.jpg">controllers</a>.

# Prerequisites<a id="orgheadline2"></a>

-   Download and install the [LeapMotion SDK](https://developer.leapmotion.com/get-started)

# Installation<a id="orgheadline3"></a>

-   Clone the project
-   `cd Abstraction-ControllerServer`
-   `make`

# Usage<a id="orgheadline4"></a>

-   Server side
    -   Connect your Leap Motion and run `sudo leapd`
    -   `./bin/LeapServer <nb_client> <port>`
-   Client side
    -   `C/C++`
        
            #include "socklib.h" // disponible dans /include
            
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
