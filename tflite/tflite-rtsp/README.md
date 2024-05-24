# About this sample

This Docker image provides an example of how to run an application with Tensorflow Lite 2.8.0 using Python and stream it via RTSP.
The container is capable of using NPU and GPU acceleration with the tensorflow-lite-vx-delegate.

The RTSP stream can be viewed with GStreamer on another machine.

This sample is only validated for the following hardware:

- Verdin iMX8M Plus: NPU/GPU/CPU
- Apalis iMX8Q Max/Plus: GPU/CPU
- Colibri iMX8QXP: GPU/CPU
  - The Colibri iMX8QXP is not recommended to run this sample, but is suitable for lighter machine learning models.

Please refer to [Torizon Sample: Real Time Object Detection with Tensorflow Lite](https://developer.toradex.com/torizon/how-to/machine-learning/torizon-sample-real-time-tensorflow-lite) to learn more about this sample.

## Build the Sample Project

If you just want to run the sample, you can skip to **Run the Sample Project**.

First, you need to setup Arm Emulation on your PC. To do this, run the command below on your PC terminal. For more information, refer to: [Enable Arm emulation](https://developer.toradex.com/torizon/application-development/working-with-containers/configure-build-environment-for-torizon-containers/#enable-arm-emulation).

```
$ docker run --rm -it --privileged torizon/binfmt
```

After setup Arm emulation, in your PC terminal, build the sample project:

```
$ docker build -t <your-dockerhub-username>/tflite-rtsp .
```

After the build, push the image to your Dockerhub account:

```
$ docker push <your-dockerhub-username>/tflite-rtsp
```

## Run the Sample Project

Enter your module's terminal using SSH.

Launch the sample application by using the following command, make sure to change /dev/video0 to the correct captured device for your setup.

```
# docker run -it --rm -p 8554:8554 \
  -v /dev:/dev \
  -v /tmp:/tmp \
  -v /run/udev/:/run/udev/ \
  --device-cgroup-rule='c 4:* rmw' \
  --device-cgroup-rule='c 13:* rmw' \
  --device-cgroup-rule='c 199:* rmw' \
  --device-cgroup-rule='c 226:* rmw' \
  --device-cgroup-rule='c 81:* rmw' \
  -e CAPTURE_DEVICE=/dev/video0 \
  -e USE_HW_ACCELERATED_INFERENCE=1 \
  -e USE_GPU_INFERENCE=0 \
  -e ACCEPT_FSL_EULA=1 \
  --name tflite-rtsp <image-tag>
```

Note: Toradex provides pre-built images for this sample, just replace **image-tag** in the run command with torizonextras/arm64v8-sample-tflite-rtsp:${CT_TAG_DEBIAN}.

## Run the RTSP client

On your development machine, make sure to have GStreamer installed. If you need to install it, refer to [Installing GStreamer](https://gstreamer.freedesktop.org/documentation/installing/index.html?gi-language=c).

Then, you will be able to run one of the commands below on your PC to view the sample's output, according to the desired environment:

### X11

```
$ gst-launch-1.0 rtspsrc location=rtsp://<board ip address>:8554/inference ! decodebin ! xvimagesink sync=false
```

### Wayland

```
$ gst-launch-1.0 rtspsrc location=rtsp://<module-ip>:8554/inference ! decodebin ! waylandsink sync=false
```

### Generic

```
$ gst-launch-1.0 rtspsrc location=rtsp://<module-ip>:8554/inference ! decodebin ! autovideosink
```
