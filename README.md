# Edge Impulse firmware for Synaptics Katana KA10000

[Edge Impulse](https://www.edgeimpulse.com) enables developers to create the next generation of intelligent device solutions with embedded Machine Learning. This repository contains the Edge Impulse firmware for the Synaptics Katana KA10000 development board. This device supports all Edge Impulse device features, including ingestion, remote management and inferencing.

> **Note:** Do you just want to use this development board with Edge Impulse? No need to build this firmware. See the instructions [here](https://docs.edgeimpulse.com/docs/synaptics-katana-ka10000) for a prebuilt image and instructions. Or, you can use the [data forwarder](https://docs.edgeimpulse.com/docs/cli-data-forwarder) to capture data from any sensor.

## Requirements

### Hardware

* [Synaptics Katana KA10000](https://www.synaptics.com/company/news/katana)

### Software

* [TENSAI Flow compiler](https://cdn.edgeimpulse.com/build-system/tensaicc)
* [GNU Arm Embedded Toolchain 7-2017-q4-major](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
* [Edge Impulse CLI](https://docs.edgeimpulse.com/docs/cli-installation)


## How to build

### Command Line build (Linux)

1. Get the [TENSAI Flow compiler](https://cdn.edgeimpulse.com/build-system/tensaicc)

Download, save the compiler (in `/opt/tensaicc`) and make executable as follows:

```
wget https://cdn.edgeimpulse.com/build-system/tensaicc -O /opt/tensaicc && \
    chmod +x /opt/tensaicc
```

Set up the environment variable `TENSAI_COMPILER_DIR` to `/opt`.

1. Get GNU Arm Embedded Toolchain

Download [GNU Arm Embedded Toolchain 7-2017-q4-major](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads). Set up the `PATH`environment variable to the toolchain, e.g:

``` 
wget https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/7-2017q4/gcc-arm-none-eabi-7-2017-q4-major-linux.tar.bz2 && \
    tar xjf gcc-arm-none-eabi-7-2017-q4-major-linux.tar.bz2 -C /opt/ && rm gcc-arm-none-eabi-7-2017-q4-major-linux.tar.bz2
export PATH="/opt/gcc-arm-none-eabi-7-2017-q4-major/bin:${PATH}"
```

1. Clone this repo and build

```
    git clone git@github.com:edgeimpulse/firmware-synaptics-ka10000.git
    cd firmware-synaptics-ka10000/SDK
    make mcusdk image
```

### Using Docker

1. Build docker container

```
    docker build -t synaptics .
```

1. Clean the project

```
    docker run --rm -u $UID -v $PWD:/app synaptics /bin/bash -c "cd SDK && make clean-mcusdk"
```

1. Build the application
```
    docker run --rm -u $UID -v $PWD:/app synaptics /bin/bash -c "cd SDK && make mcusdk image"
```

### Upload image using your local machine

Flashing the firmware image to the KA10000 platform is done using an I2C connection. The CX31993 bridge board
converts USB to I2C and is controlled by the `i2c_flash_f3` uploader tool.


#### Linux (Unix)
Make sure to have installed following packages: g++, libudev-dev and libusb-1.0.0-dev. And always run the tool using `sudo`
From the main folder you can use this one-liner:
```
    cd SDK/fcp/build/ ; sudo ./i2c_flash_f3_linux partition_flash.img ; cd ../../../
```


#### Windows
For Windows you can use the executable from the `fcp/build` folder to upload the image:
```
    i2c_flash_f3.exe partition_flash.img
```

## Update your NN Classifier Model

In the Studio, navigate to **Deployment**, choose **TENSAI Flow library**, choose **Quantized (int8)** optimization and click **Build**.
Extract the contents into `SDK/mcusdk/platform/tahiti/m33/app/firmware/edge_impulse`. Be sure to remove the previous model folders (`edge-impulse-sdk`, `model-parameters` and `tensaiflow-model`).

**NOTE**: only Quantized (int8) Grayscale image models are supported.
