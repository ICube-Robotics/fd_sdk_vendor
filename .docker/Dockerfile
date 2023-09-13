ARG ROS_DISTRO="humble"
FROM ros:${ROS_DISTRO}

COPY . /ros2_dev/src/fd_sdk_vendor
RUN apt update && apt upgrade -y
RUN apt install libusb-1.0-0-dev
RUN apt install -y python3-pip
RUN apt install -y git tree clang-format-14 cppcheck
RUN pip install pre-commit
