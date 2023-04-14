# Environment Setup for Operating Systems and Computer Architecture
Prof. Rodney Van Meter

# Download Docker Desktop
Mac, Windows, and Linux download:
https://www.docker.com/products/docker-desktop/

# Create a Docker Hub Account
Create an account at: https://hub.docker.com/signup
Confirm your email address
Open Docker Desktop application
Sign-in through the application (Docker Icon > Sign In)


## Pull the Docker Image
On your terminal, enter the following command: 

```
docker pull nyatsume/nyatsume/ostep-for-multiarch:latest
```

## Run the example application
1. Run the following command
```
git clone https://github.com/makotonakai/computer-architecture-2022.git
```

The directory structure should look like this:
```
/matmul
/pmatmul 
/quantum_sim 
/test 
getting-started.pdf
```
Check if all the directories exist!

1. Run the following command: 

```
cd computer-architecture-2022
docker run -it -v $PWD:/root/computer-architecture/ --privileged nyatsume/nyatsume/ostep-for-multiarch:latest /bin/bash
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
