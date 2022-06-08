## Build docker container

```
    docker build -t synaptics .
```

## Run docker container (interactive for now)
```
    docker run --rm -it -u $UID -v $PWD:/app synaptics
```

## Build the project
```
    docker run --rm -u $UID -v $PWD:/app synaptics /bin/bash -c "cd SDK && make mcusdk image"
```

## Clean project
```
    docker run --rm -u $UID -v $PWD:/app synaptics /bin/bash -c "cd SDK && make clean-everything"
```

## Upload image using your local machine

Flashing the firmware image to the KA10000 platform is done using an I2C connection. The CX31993 bridge board
converts USB to I2C and is controlled by the `i2c_flash_f3` uploader tool.


### Linux (Unix)
Make sure to have installed following packages: g++, libudev-dev and libusb-1.0.0-dev. And always run the tool using `sudo`
From the main folder you can use this one-liner:
```
    cd SDK/fcp/build/ ; sudo ./i2c_flash_f3_linux partition_flash.img ; cd ../../../
```


### Windows
For Windows you can use the executable from the `fcp/build` folder to upload the image:
```
    i2c_flash_f3.exe partition_flash.img
```


## Update your NN Classifier Model

In the Studio, navigate to **Deployment**, choose **Tensia Flow library**, choose **Quantized (int8)** optimization and click **Build**.
Extract the contents into `SDK/mcusdk/platform/tahiti/m33/app/firmware/edge_impulse`. Be sure to remove the previous model folders (`edge-impulse-sdk`, `model-parameters` and `tensaiflow-model`).

**NOTE**: only Quantized (int8) Grayscale image models are supported.
