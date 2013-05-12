/*
 * video_capture.c
 *
 *  Created on: Mar 20, 2013
 *      Author: yang
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>


#include "include/video_capture.h"

//open camera
static open_camera()
{
    struct stat st;

    //query the file is exit
    if (-1 == g_vc_config.device_name, &st)
    {
        printf("Cann't identify %s\n", g_vc_config.device_name);
        exit(EXIT_FAILURE);
    }

    //test the file is a character device
    if (!S_ISCHR(st.st_mode))
    {
        printf("%s is not a device file\n", g_vc_config.device_name);
        exit(EXIT_FAILURE);
    }

    //open the camera device
    g_vc_config.camera_fd = open(g_vc_config.device_name, O_RDWR, 0);

    //test whether open successfully or not
    if (-1 == g_vc_config.camera_fd)
    {
        printf("can't open %s\n", g_vc_config.device_name);
        exit(EXIT_FAILURE);
    }
}


//close camera
static void close_camera()
{
    //close the camera device and test the result
    if (-1 == close(g_vc_config.camera_fd))
    {
        printf("close device failure\n");
        exit(EXIT_FAILURE);
    }
}


//map the driver memory from kernel space to user space
static void init_mmap()
{
    struct v4l2_requestbuffers req;
    int32_t 	i = 0;

    CLEAR(req);

    req.count = BUFFER_COUNT;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    //get capture memory
    if (-1 == ioctl(g_vc_config.camera_fd, VIDIOC_REQBUFS, &req))
    {
        if (EINVAL == errno)
        {
            printf("%s does not support memory map\n", g_vc_config.device_name);
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("get memory failure!\n");
            exit(EXIT_FAILURE);
        }
    }

    if (req.count < 2)
    {
        printf("can't get enough memory\n");
        exit(EXIT_FAILURE);
    }

    g_vc_config.buffers = calloc(req.count, sizeof(*(g_vc_config.buffers)));

    if (NULL == g_vc_config.buffers)
    {
        printf("out of memory!\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < req.count; ++i)
    {
        struct v4l2_buffer buf;

        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (-1 == ioctl(g_vc_config.camera_fd, VIDIOC_QUERYBUF, &buf))
        {
            printf("can't get info of video query buffer\n");
            exit(EXIT_FAILURE);
        }

        g_vc_config.buffers[i].length = buf.length;
        g_vc_config.buffers[i].start = mmap(NULL,
                buf.length, PROT_READ|PROT_WRITE,
                MAP_SHARED, g_vc_config.camera_fd, buf.m.offset);

        //test memory map is useful
        if (MAP_FAILED == g_vc_config.buffers[i].start)
        {
            printf("map memory failure\n");
            exit(EXIT_FAILURE);
        }
    }
}


//set and get video capture FPS
static void set_capture_fps()
{
    struct v4l2_streamparm setfps={0};

    setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    setfps.parm.capture.timeperframe.numerator = 1;
    setfps.parm.capture.timeperframe.denominator = g_vc_config.fps;

    if (-1 == ioctl(g_vc_config.camera_fd, VIDIOC_S_PARM, &setfps))
    {
        printf("set video param error in set_capture_fps function\n");
        return;
    }

    g_vc_config.fps = (int)setfps.parm.capture.timeperframe.denominator;

    printf("now the video FPS is %d\n", g_vc_config.fps);
}


//start capture the picture from camera
static void start_capturing()
{
    int32_t            i = 0;
    enum v4l2_buf_type type;

    for (i = 0; i < BUFFER_COUNT; ++i)
    {
        struct v4l2_buffer buf;

        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (-1 == ioctl(g_vc_config.camera_fd, VIDIOC_QBUF, &buf))
        {
            printf("video query buffer failure\n");
            exit(EXIT_FAILURE);
        }
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    //start capture the video
    if (-1 == ioctl(g_vc_config.camera_fd, VIDIOC_STREAMON, &type))
    {
        printf("video stream open failure\n");
        exit(EXIT_FAILURE);
    }
}


//stop capture the picture
static void stop_capturing()
{
    enum v4l2_buf_type type;

    type = V4L2_BUF_TYPE_VBI_CAPTURE;

    //stop capturing picture
    if (-1 == ioctl(g_vc_config.camera_fd, VIDIOC_STREAMOFF, &type))
    {
        printf("stop capture the video device\n");
        exit(EXIT_FAILURE);
    }
}


//initialize the camera device
static void init_camera()
{
    struct v4l2_capability 	cap;
    struct v4l2_format 		fmt;

    //get the video device capability
    if (-1 == ioctl(g_vc_config.camera_fd, VIDIOC_QUERYCAP, &cap))
    {
        if (EINVAL == errno)
        {
            printf("%s is not a v4l2 device\n", g_vc_config.device_name);
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("query cap failure\n");
            exit(EXIT_FAILURE);
        }
    }

    //judge the device whether is a video capture device or not
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
        printf("%s is not a video capture device\n", g_vc_config.device_name);
        exit(EXIT_FAILURE);
    }

    //test the device support video streaming or not
    if (!(cap.capabilities & V4L2_CAP_STREAMING))
    {
        printf("%s is not support streaming\n", g_vc_config.device_name);
        exit(EXIT_FAILURE);
    }

#ifdef 0
    printf("\nVIDOOC_QUERYCAP\n");
    printf("the camera driver is %s\n", cap->driver);
    printf("the camera card is %s\n", cap->card);
    printf("the camera bus info is %s\n", cap->bus_info);
    printf("the version is %d\n", cap->version);
#endif

    CLEAR(fmt);

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = g_vc_config.width;
    fmt.fmt.pix.height = g_vc_config.height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    //set the video format
    if (-1 == ioctl(g_vc_config.camera_fd, VIDIOC_S_FMT, &fmt))
    {
        printf("can't set video format\n");
        exit(EXIT_FAILURE);
    }
}


//free the camera resource
static void uninit_camera()
{
    int32_t     i = 0;

    for (i = 0; i < BUFFER_COUNT; ++i)
    {
        if (-1 == munmap(g_vc_config.buffers[i].start, g_vc_config.buffers[i].length))
        {
            printf("can't free the mapped memory\n");
            exit(EXIT_FAILURE);
        }
    }

    free(g_vc_config.buffers);
    g_vc_config.buffers = NULL;
}



//initialize the camera device
int32_t video_capture_init(int32_t width, int32_t height, int32_t fps)
{
    //set width,height,fps first
    g_vc_config.width = width;
    g_vc_config.height = height;
    g_vc_config.fps = fps;

    //set camera device name
    g_vc_config.device_name = "";

    //set frame count to zero
    g_vc_config.frame_count = 0;

    //open camera device
    open_camera();

    //set capture fps
    set_capture_fps();

    //init the camera facture
    init_camera();

    //memory mapping
    init_mmap();

    //start capturing frame
    start_capturing();
}


int32_t video_capture_get_frame(uint8_t *pic_out, int32_t *length)
{
    struct v4l2_buffer buf;
    CLEAR(buf);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    if (-1 == ioctl(g_vc_config.camera_fd, VIDIOC_DQBUF, &buf))
    {
        switch(errno)
        {
        case EAGAIN:
            return 0;
        case EIO:
        default:
            printf("read frame from camera wrong\n");
            return BASICERROR;
        }
    }

    memcpy(pic_out, g_vc_config.buffers[buf.index].start, g_vc_config.buffers[buf.index].length);
    *length = g_vc_config.buffers[buf.index].length;

    if (-1 == ioctl(g_vc_config.camera_fd, VIDIOC_QBUF, &buf))
    {
        printf("read frame frame camera put buf into the frame queue error");
        return BASICERROR;
    }

    return SUCCESS;
}


int32_t video_capture_deinit()
{
    //stop capture the camera
    stop_capturing();

    //release the resource camera device has token
    uninit_camera();

    //close the camera
    close_camera();
}
