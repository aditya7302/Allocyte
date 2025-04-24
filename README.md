# Memory Manager - allocyte

This is a C++ project implementing a custom linux memory management system with multiple components.

## Requirements
- **For Local Compilation:**
  - GCC/G++ compiler (version 7.0 or higher)
  - Make utility
- **For Docker Deployment:**
  - Docker (installed and running)
  - Docker Hub account (optional, for pushing the image)

## Running the Project Locally (Manual Steps)
If you prefer to compile and run the project manually on your local machine, follow these steps:

1. **Install Dependencies:**
- Ensure GCC/G++, and Make are installed. On Ubuntu, install them with:

   #
       sudo apt-get update
       sudo apt-get install g++ make


3. **Navigate to the Project Directory:**
- Open a terminal and change to the project directory:

  #
      cd /path/to/allocyte


3. **Compile the Source Files:**
- Compile each source file into object files:

  #
      g++ -g -c testapp.cpp -o testapp.o
      g++ -g -c mm.cpp -o mm.o
      g++ -g -c gluethread/glthread.cpp -o gluethread/glthread.o


4. **Link the Object Files:**
- Link the object files to create the executable:

  #
      g++ -g testapp.o mm.o gluethread/glthread.o -o exe


5. **Run the Executable:**
- Execute the program:

  #
      ./exe


6. **Clean Up (Optional):**
- Remove object files and the executable if needed:

  #
      rm -f testapp.o mm.o gluethread/glthread.o exe


## Running the Project with Makefile and Docker
For a containerized and automated approach, use the provided Makefile to build and run the project via Docker. Follow these steps:

1. **Install Docker:**
- Ensure Docker is installed and running. On Ubuntu, install it with:

  #
      sudo apt-get update
      sudo apt-get install docker.io
      sudo systemctl start docker
      sudo systemctl enable docker


2. **Navigate to the Project Directory:**
- Open a terminal and change to the project directory:

  #
      cd /path/to/allocyte


3. **Build the Docker Image:**
- Use the Makefile to build the Docker image:

  #
      make docker-build

- This runs `docker build -t ayushrakesh/allocyte -f Dockerfile .`, creating an image tagged as `ayushrakesh/allocyte:latest`.


4. **Run the Docker Container:**
- Start a container from the image:

  #
      make docker-run

- This runs `docker run -it ayushrakesh/allocyte`, providing an interactive terminal to execute the program inside the container.


5. **(Optional) Push to Docker Hub:**
- If you have a Docker Hub account, log in and push the image:

  #
      docker login
      make docker-push

- This runs `docker push ayushrakesh/allocyte`, uploading the image to your Docker Hub repository.


6. **Full Workflow (Build and Run):**
- For a single command to build and run, use:

  #
      make docker

- This executes both `docker-build` and `docker-run` sequentially.

## Notes
- Ensure the `gluethread` directory and all header files (e.g., `mm.hpp`, `glthread.hpp`) are accessible in the project directory.
- For local compilation, verify that the GLIBC version on your system matches the requirements of the compiled `exe` to avoid runtime errors. If issues arise, consider building inside a Docker container.
- For Docker, the image is built and run inside a containerized environment (Ubuntu 20.04 by default), ensuring compatibility.
- See the `Makefile` for automated commands and the `Dockerfile` for container configuration.

## Troubleshooting
- **Local GLIBC Errors:** If `./exe` fails with a GLIBC version error (e.g., `GLIBC_2.34 not found`), it means the local GLIBC version is incompatible. Rebuild the project inside a Docker container using `make docker-build` and `make docker-run`.
- **Docker Issues:** If `make docker-build` fails, check the `Dockerfile` for syntax errors and ensure Docker is running (`sudo systemctl status docker`). Verify permissions with `docker ps`.
- **Missing Files:** Ensure all `.cpp` and `.hpp` files (e.g., `block_split_ass.cpp`, `mm_ass.cpp`) are present in the project directory.
