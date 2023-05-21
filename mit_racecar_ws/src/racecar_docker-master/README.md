# Racecar Docker

This code defines a docker image to interface with the MIT Racecar.
The image is built from a Debian base, includes the latest version of ROS, and the [racecar simulator](https://github.com/mit-racecar/racecar_simulator). It can be interfaced through a terminal or graphically through a browser or VNC client.

## Running the Image

If you do not already have docker, follow the install instructions for your OS [here](https://docs.docker.com/install/).

Start the docker image by running:

    sudo docker run -ti --net=host racecar/racecar

The first time you run this command you will need to wait a little while for the image to download.
Future runs should be instantaneous and won't require an internet connection.
The image is currently ~2.25GB (ROS is big).

### Troubleshooting

Unfortunately, due to the way that networking is implemented in macOS, the `--net=host` flag does not work
\[[1](https://docs.docker.com/docker-for-mac/networking/),[2](https://github.com/docker/for-mac/issues/2716)\].
You can partially fix this by running:

    sudo docker run -tip 6080:6080 -p 5900:5900 racecar/racecar

If you are running Windows you may also need to run the following before running docker to get the terminal to work properly:

     alias docker="winpty docker"

See the [Additional Docker Options](https://github.com/mit-racecar/racecar_docker#additional-docker-options) section for more useful docker flags.

## Using the Image

When you run the command above, you will be presented with a new bash shell in the folder `racecar_ws`.
This shell has ROS installed (e.g. try running `roscore`).
It also has the programs `screen` and `tmux` installed.
These programs allow you to run many shells from within the same window.

In addition to the terminal interface, you can interact with the image visually through either your browser or through VNC.
This allows you to use programs like `rviz`.

To use the image in the browser, navigate to [http://localhost:6080/vnc.html](http://localhost:6080/vnc.html). Hit the "Connect" button and you're in!

Alternatively, you can interface with the image using any VNC client with address `localhost:5900`.

Some operating systems (e.g. windows) don't like the `localhost` variable. If you can't connect you may have to type in the docker image's actual IP address. Find the IP address by typing `hostname -I` in the image's terminal.
Then visit either of the links above, replacing `localhost` with this IP.

The visual interface has two buttons that launch a terminal and `rviz` respectively.
By default, clicking on the terminal button when a terminal is already open minimizes that window.
To open multiple terminals, type <kbd>CTRL</kbd> and then click on the terminal icon.

### Running the Racecar Simulator

To get started with the simulator, first run the following in any shell:

    roslaunch racecar_simulator simulate.launch

Then open `rviz`.
You should see a blue car on a black and white background (a map) and some colorful dots (simulated lidar).
If you click the green 2D Pose Estimate arrow on the top you can change the position of the car.
Alternatively use a joystick to drive the car as described below.

## Additional Docker Options

### Running on a specific car

By default the image is set up to use ROS locally. If you want to connect the image to another `rosmaster` (e.g. a racecar) you need to change some ROS variables. You can do this automatically by running:

    sudo docker run -ti --net=host racecar/racecar CAR_NUMBER

This sets the correct `ROS_IP`, `ROS_MASTER_URI`, and `/etc/hosts` variables, assuming that the car's IP is `192.168.1.CAR_NUMBER`. When you launch `rviz` it will display topics published on the racecar. Note that this won't work on macOS due to the networking issues described above.

You will also be able to `ssh` into the racecar from within the docker image by typing:

    ssh racecar@racecar

### Mounting a local drive

Docker images do not save changes made to them by default which can be dangerous when writing lots of code.
Plus, the docker image may not have your favorite text editor, window manager, etc. installed.
To solve both of these issues, you can mount a local folder into the docker image.
This will make sure your changes are written and give you the freedom to edit the code in whatever environment you would like.

We recommend that you mount into the `/racecar_ws/src` folder.
This is typically where all of your code will live while working with the racecar or the racecar simulator.
You can do this by adding the following to your docker run command:

    sudo docker run -tiv /full/path/to/local/folder:/racecar_ws/src --net=host racecar/racecar

### Using a Joystick

To use a joystick in the image (e.g. to use with the simulator),
you need to forward inputs from that USB device into docker.
Most joysticks map to `/dev/input/js0` by default, so you can add that device with:

    sudo docker run -ti --net=host --device=/dev/input/js0 racecar/racecar

## Building the Image From Scratch

To build the image from scratch, run:

    git clone https://github.com/mit-racecar/racecar_docker.git
    cd racecar_docker
    sudo docker build -t racecar .

Then run with:

    sudo docker run -ti --net=host racecar
