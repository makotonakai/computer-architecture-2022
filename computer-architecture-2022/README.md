# Environment Setup for Operating Systems and Computer Architecture
Prof. Rodney Van Meter

## Download Docker
Mac: https://hub.docker.com/editions/community/docker-ce-desktop-mac

Windows: https://hub.docker.com/editions/community/docker-ce-desktop-windows

Ubuntu: https://docs.docker.com/desktop/install/linux-install/

## Create a Docker Hub Account
1. Create an account at: https://hub.docker.com
2. Confirm your email address
3. Sign-in! For macs, you will have a docker icon on the upper-left side of your menu bar.
Docker Icon > Sign In

## Pull the Docker Image
On your terminal, enter the following command: 

```
docker pull nikkokun/computer-architecture:latest
```

## Run the example application
1. Get the source code from SFC SFS
2. Unzip the folder
The directory structure should look like this:

```
/matmul
/pmatmul 
/quantum_sim 
/test 
getting-started.pdf
```
   
3. Check if all the directories exist!
4. Run the following command: 

```
cd computer-architecture2019
docker run -it -v $PWD:/root/computer-architecture/ --privileged nikkokun/computer-architecture:latest /bin/bash
```

This will provide you an Ubuntu 18.04 bash shell. To exit the shell, enter: ctrl+d 5. Double-check if all the directories exist!

```
ls -a 
```
It should print out:

```
/matmul
/pmatmul 
/quantum_sim 
/test 
getting-started.pdf
```

Any changes you make while you are in the Ubuntu shell will reflected on disk! Meaning any changes you make on the files will persist, even if you log out of the shell. This also goes for any changes you make on your host environment.
6. Go to the test directory and see what’s inside
```
cd /test ls -a
```
You should have the following file: main.cpp

7. Compile and run!
```
g++ main.cpp -o main -fopenmp 
```

8. Run it:
```
./main 
```
