# Project building

## Docker build environment

To build the Dockerfile, run in the directory `Dockerfile/`:

> sudo docker build -t snakebuilder . && cd ..

And then run:

> sudo docker run --rm --net host -e DISPLAY=${DISPLAY} --device /dev/dri --device /dev/input -v $PWD/:/data
 -u `id -u`:`id -u` -it snakebuilder``

The `--rm` flag removes the container after work,

the `--net host` flag gives access to the display, 

the `-e DISPLAY=${DISPLAY}` sets display environment variable,

the `--device /dev/dri` provides acces to graphics card device,

the `--device /dev/input` provides access to the input device,

the `-v hostdir:containerdir` flag mounts the directory hostdir present in the host-side to the directory containerdir in the container, in this case it’s current dir mounted to `/data/`,

the `-u` flag sets the user ID to the current user.
