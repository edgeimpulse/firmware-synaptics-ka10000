#!/bin/bash

set -e
DBG="EXTERNAL"

function help()
{
    echo "------- Usage : ./make_host_code.sh <target> <variant> <interface>  --------"
    echo "------- Note  : Before make apps, all_libs need to pre-builded  --------"
    echo "/make_host_code.sh all_libs                  : build all pre-require libs"
    echo "./make_host_code.sh all_apps                  : build all applications"
    echo "./make_host_code.sh all                       : build all pre-require libs for application" 
	echo "./make_host_code.sh host_demo linux_rpi i2c   : build host_demo using I2C interface for linux_rpi"
    echo "./make_host_code.sh host_demo linux_rpi usb   : build host_demo using USB interface for linux_rpi"
    echo "./make_host_code.sh i2c_flash linux_rpi       : build i2c_flash for linux_rpi"
    echo "./make_host_code.sh bg_upgrade linux_rpi i2c  : build bg_upgrade using I2C interface for linux_rpi"
    echo "./make_host_code.sh bg_upgrade linux_rpi usb  : build bg_upgrade using USB interface for linux_rpi"
    echo "./make_host_code.sh write_persistent_storage linux_rpi i2c  : build write_persistent_storage using I2C interface for linux_rpi"
    echo "./make_host_code.sh write_persistent_storage linux_rpi usb  : build write_persistent_storage using USB interface for linux_rpi"
    echo "./make_host_code.sh caf_thin_client linux_rpi i2c : build caf_thin_client using I2C interface for linux_rpi"
    echo "./make_host_code.sh caf_thin_client linux_rpi usb : build caf_thin_client using USB interface for linux_rpi"
}

function build_single()
{
  interface_flag=""
  if [[ $1 = "libbg_upgrade" ]];
  then
    fpath="../api_src/top_level_api/bg_upgrade_api"
  elif [[ $1 = "libi2c_flash" ]]
  then {
    fpath="../api_src/top_level_api/i2c_flash_api"
  }
  elif [[ $1 = "libdsp_ctrl_status" ]]
  then {
    fpath="../api_src/top_level_api/dsp_ctrl_status_api"
  }
  elif [[ $1 = "libpersistent_storage_api" ]]
  then {
    fpath="../api_src/top_level_api/persistent_storage_api"
  }
  elif [[ $1 = "libsendcmd_i2c" ]]
  then {
	fpath="../api_src/mid_level_api/sendcmd_i2c"
  }
  elif [[ $1 = "libsendcmd_usb" ]]
  then {
	fpath="../api_src/mid_level_api/sendcmd_usb"
  }
  elif [[ $1 = "libi2c_interface" ]]
  then {
	fpath="../api_src/interface_api/i2c_interface/i2c_dev"
  }
  elif [[ $1 = "libusb_interface" ]]
  then {
	fpath="../api_src/interface_api/usb_interface/libusb"
  }
  elif [[ $1 = "api_i2c_flash"  ||    $1 = "i2c_flash" ]] ; then  
      fpath="../demo/api_i2c_flash" 
  elif [[  $1 = "bg_upgrade" ||  $1 = "dsp_ctrl_status"  || $1 = "host_demo" ||  $1 = "write_persistent_storage" ||  $1 = "api_caf_thin_client"  ||    $1 = "caf_thin_client"  || $1 = "thin_client" ]];
  then
    fpath="../demo/$1"
    if [[ $1 = "host_demo" ]] ; then 
      fpath="../demo/dsp_ctrl_status" 
    fi 
    if [[ $1 = "api_caf_thin_client"  ||    $1 = "caf_thin_client"  || $1 = "thin_client" ]] ; then 
      fpath="../application/api_caf_thin_client" 
    fi
    #echo "par number: $#"
    if [ $# -ne 3 ]; then 
        echo "ERROR: Need to assign variant and interface type : i2c or usb. e.g  ./make_host_code.sh bg_upgrade  linux_rpi  usb"
        echo "run ./make_host_code.sh -h for more information"
        #help
      exit -1
    else 
      if [[ $3 = "I2C" || $3 = "i2c" ]]; then 
       interface_flag="EXTRA_CPPFLAGS=-DENABLE_I2C_CONTROL"
      elif [[ $3 = "USB"  || $3 = "usb" ]]; then 
       interface_flag="EXTRA_CPPFLAGS=-DENABLE_USB_CONTROL"
      else 
        echo "ERROR: Need to assign variants and interface type : i2c or usb. e.g  ./make_host_code.sh bg_upgrade  linux_rpi  usb"
        echo "run ./make_host_code.sh -h for more information"
        exit -1 
      fi 
    fi
  fi

  echo "fpath=$fpath"
  if [ -d $fpath  ] && [[ $2 = all || $2 = ALL ]]; 
  then
    echo -e "\n*======== Build $1, VARIANT=$VARIANT $interface_flag  ========*"
    make -C $fpath VARIANT=$VARIANT  $interface_flag
    if [[ $DBG = "INTERNAL" ]] ;
    then 
      echo -e "\n*======== Build $1, VARIANT=linux_armv7-a $interface_flag ========*"
      make -C $fpath VARIANT=linux_armv7-a $interface_flag
      echo -e "\n*======== Build $1, VARIANT=linux_armv7-a_hf $interface_flag ========*"
      make -C $fpath VARIANT=linux_armv7-a_hf $interface_flag
      echo -e "\n*======== Build $1, VARIANT=linux64_armv8-a $interface_flag ========*"
      make -C $fpath VARIANT=linux64_armv8-a $interface_flag
      echo -e "\n*======== Build $1, VARIANT=android_armv7-a $interface_flag ========*"
      make -C $fpath VARIANT=android_armv7-a $interface_flag
      echo -e "\n*======== Build $1, VARIANT=android64_armv8-a $interface_flag ========*"
      make -C $fpath VARIANT=android64_armv8-a $interface_flag

   fi    
  elif [ -d $fpath  ] && [ $# -lt 4 ] && [[ $2 = "linux_rpi"  || $2 = "linux_x86" || $2 = "linux_armv7"  || $2 = "linux_armv7-a_hf" || $2 = "linux64_armv8-a" ||  $2 = "android_armv7-a" ||  $2 = "android64_armv8-a" || $2 = "arm-cortex-m4" ]];
  then  
      echo -e "\n*======== make $fpath VARIANT=$2 $interface_flag ========*"
      make -C $fpath  VARIANT=$2 $interface_flag 
  else 
    "Invalud target name $1 $2 $3"
    exit 1
  fi
  } # end fucntion build_single

  
COMPILE_ENV=`uname -a`

if [[ "$COMPILE_ENV" =~ "Cygwin" ]]
then
VARIANT=linux_rpi 
fi

if [[ "$COMPILE_ENV" =~ "raspberrypi" ]]
then
VARIANT=linux_rpi 
fi

if [[ "$COMPILE_ENV" =~ "x86" ]]
then
VARIANT=linux_x86 
fi

  
if [[ $# -lt 1 ]] ;
then
  echo "ERROR: the nubmer of command options is not enough "
  help
  exit -1 ; 
fi 

if [[ $1 = "-h" ||  $1 = "--help" ]];
then 
  help
elif [[ $# -eq 1 && $1 = "all_libs" ]];
then
  echo -e "\n*======== Build all pre-require libs ========*"
  echo -e "\n*======== Build Interface-Level_API libs ========*"
  build_single "libi2c_interface" "all"
  build_single "libusb_interface" "all"
  echo -e "\n*======== Build Mid-Level_API libs ========*"
  build_single "libsendcmd_i2c" "all"
  build_single "libsendcmd_usb" "all"
  echo -e "*======== Build Top-Level_API libs ========*"
  build_single "libdsp_ctrl_status" "all"
  if [[ $DBG = "INTERNAL" ]] ;
  then   
    build_single "libi2c_flash" "all"
    build_single "libbg_upgrade" "all"
    build_single "libpersistent_storage_api" "all"
  fi
elif [[ $# -eq 1 && $1 = "all_apps" ]];
then
  echo -e "\n*======== Build Applications ========*"
  build_single "host_demo" "all" "I2C"
  build_single "host_demo" "all" "USB"
  build_single "api_i2c_flash" "all"
  build_single "bg_upgrade" "all" "I2C"
  build_single "bg_upgrade" "all" "USB"
  build_single "write_persistent_storage" "all" "USB"
  build_single "write_persistent_storage" "all" "I2C"
  build_single "api_caf_thin_client"  "all" "I2C"
  build_single "api_caf_thin_client"  "all" "USB"
elif [[ $# -eq 1 && $1 = "all" ]];
then
  echo -e "\n*======== Build Interface-Level_API libs ========*"
  build_single "libi2c_interface" "all"
  build_single "libusb_interface" "all"
  echo -e "\n*======== Build Mid-Level_API libs ========*"
  build_single "libsendcmd_i2c" "all"
  build_single "libsendcmd_usb" "all"
  echo -e "*======== Build Top-Level_API libs ========*"
  build_single "libdsp_ctrl_status" "all"
  
  if [[ $DBG = "INTERNAL" ]] ;
  then   
    build_single "libi2c_flash" "all"
    build_single "libbg_upgrade" "all"
    build_single "libpersistent_storage_api" "all"
  fi

  echo -e "\n*======== Build Applications ========*"
  build_single "host_demo" "all" "I2C"
  build_single "host_demo" "all" "USB"
  build_single "api_i2c_flash" "all"
  build_single "bg_upgrade" "all" "I2C"
  build_single "bg_upgrade" "all" "USB"
  build_single "write_persistent_storage" "all" "USB"
  build_single "write_persistent_storage" "all" "I2C"
  build_single "api_caf_thin_client"  "all" "I2C"
  build_single "api_caf_thin_client"  "all" "USB"

elif [[ $# -gt 1  && $3 -lt 4 ]];
then
  echo "************build $1 $2 $3 *******************"
  build_single $1 $2 $3
else  
  echo "ERROR: command options error "
  help
  exit -1 ; 
fi

exit  0
