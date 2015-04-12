#include "backglow.h"
#include <cmath>
#include <iostream>
#include <cstdint>

struct constants_t{
    int32_t screenH;
    int32_t screenW;
    int32_t leds;
    int32_t ledSize;
    int32_t depth;
};

const char kernelCode []  = R"(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

struct constants_t{
    int screenH;
    int screenW;
    int leds;
    int ledSize;
    int depth;
};

__kernel void sumLines(__read_only image2d_t input, __global float4 * output, __constant struct constants_t * constants){
    int led = get_global_id(0);
    int y = get_global_id(1);
    float4 sum = 0;
    int ledSize = constants->ledSize;
    for(int i = 0; i < ledSize; ++i){
        int2 coords;
        coords.x = led * ledSize + i;
        coords.y = y;
        sum += pow(read_imagef(input, sampler, coords), 2.2f);
    }
    output[led * constants->depth + y] = sum;
}

__kernel void sumLeds(__global float4 * input, __global float * output, __constant struct constants_t * constants){
    int led = get_global_id(0);
    float4 sum = 0;
    int depth = constants->depth;
    for(int i = 0; i < depth; ++i){
        sum += input[led * depth + i];
    }

    output[led * 3 + 0] = sum.x;
    output[led * 3 + 1] = sum.y;
    output[led * 3 + 2] = sum.z;
}
)";

BackGlow::BackGlow(QString port) :
    m_Serial(port)
{
    m_ScreenWidth  = m_ScreenCap.getWidth();
    m_ScreenHeight = m_ScreenCap.getHeight();

    if (m_Serial.isReady()) {
        printf("Connected to serial port\n");
    } else {
        printf("Cannot connect to serial port!");
    }

    cl_int err = CL_SUCCESS;
    try {
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        if (platforms.size() == 0) {
            std::cout << "Platform size 0\n";
            return;
        }

        for(cl::Platform& platform : platforms){
            std::vector<cl::Device> devices;
            platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
            if(devices.size() > 0){
                device = devices[0];
                int vendorID = device.getInfo<CL_DEVICE_VENDOR_ID>();
                if(vendorID != 32902) {
                    cl_context_properties properties[] =
                    { CL_CONTEXT_PLATFORM, (cl_context_properties)(platform)(), 0};
                    context = cl::Context(CL_DEVICE_TYPE_GPU, properties);
                    device = devices[0];
                    std::cout << "Got GPU:  name"
                              << device.getInfo<CL_DEVICE_NAME>()
                              << "\n Vendor: " << device.getInfo<CL_DEVICE_VENDOR>()
                              << "\n VendorID: " << device.getInfo<CL_DEVICE_VENDOR_ID>()
                              << std::endl;
                    break;
                }
            }
        }

        std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();


        {
            cl::Program::Sources source(1, std::make_pair(kernelCode, strlen(kernelCode)));

            cl::Program program_ = cl::Program(context, source);
            try{
                program_.build(devices);
                std::cout << program_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
                kernelPass1 = cl::Kernel(program_, "sumLines", &err);
                kernelPass2 = cl::Kernel(program_, "sumLeds", &err);
            }catch (cl::Error err) {
                std::cerr << program_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
                throw;
            }
        }

        cl::ImageFormat format;
        format.image_channel_data_type = CL_UNORM_INT8;
        format.image_channel_order = CL_BGRA;

        inputImage = cl::Image2D(context, CL_MEM_READ_ONLY, format, m_ScreenWidth, m_ScreenHeight);
        middleBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, m_ScreenHeight * MAXLEDS * 16);
        outputBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, MAXLEDS * 12);
        constantBuffer = cl::Buffer(context, CL_MEM_READ_ONLY, sizeof(constants_t));

        kernelPass1.setArg(0, inputImage);
        kernelPass1.setArg(1, middleBuffer);
        kernelPass1.setArg(2, constantBuffer);


        kernelPass2.setArg(0, middleBuffer);
        kernelPass2.setArg(1, outputBuffer);
        kernelPass2.setArg(2, constantBuffer);
    }
    catch (cl::Error err) {
        std::cerr << "ERROR: " << err.what() << "(" << err.err() << ")" << std::endl;
    }
}

BackGlow::~BackGlow()
{
    unsigned char buffer[1024];
    memset(buffer, 0, 1024);
    buffer[0] = (unsigned char)(m_leds);
    m_Serial.write((char *)buffer, m_leds * 3 + 1);
}

void BackGlow::process()
{
    m_ScreenCap.capture();
    int dataSize = m_ScreenWidth * m_ScreenHeight * 4;

    float rgb[3 * MAXLEDS];
    memset(rgb, 0, sizeof(rgb));
    constants_t constants;
    constants.depth = m_depth;
    constants.leds = m_leds;
    constants.ledSize = m_ScreenWidth / m_leds;
    constants.screenH = m_ScreenHeight;
    constants.screenW = m_ScreenWidth;
    try{
        cl::CommandQueue queue(context, device);
        queue.enqueueWriteBuffer(constantBuffer, CL_TRUE, 0, sizeof(constants_t), &constants);
        queue.enqueueBarrier();

        cl::size_t<3> origin;
        origin.push_back(0); origin.push_back(0); origin.push_back(0);

        cl::size_t<3> region;
        region.push_back(m_ScreenWidth); region.push_back(m_depth); region.push_back(1);

        queue.enqueueWriteImage(inputImage, CL_FALSE, origin, region,
                                4 * m_ScreenWidth, 0, m_ScreenCap.getData());
        queue.enqueueBarrier();

        queue.enqueueNDRangeKernel(kernelPass1, cl::NullRange, cl::NDRange(m_leds, m_depth), cl::NullRange);
        queue.enqueueBarrier();

        queue.enqueueNDRangeKernel(kernelPass2, cl::NullRange, cl::NDRange(m_leds), cl::NullRange);
        queue.enqueueBarrier();

        queue.enqueueReadBuffer(outputBuffer, CL_TRUE, 0, sizeof(rgb), rgb);
        queue.finish();

    }catch (cl::Error err) {
        std::cerr << "ERROR: " << err.what() << "(" << err.err() << ")" << std::endl;
    }

    unsigned char buffer[3 * MAXLEDS + 1];
    int i = 0;
    memset(buffer, 0, sizeof(buffer));
    buffer[i] = (unsigned char)(m_leds);
    float mul = 255.0f / ((m_ScreenWidth / m_leds) * m_depth) * m_brightnes;
    for(int led = m_leds - 1; led >= 0; --led){
        buffer[++i] = (unsigned char)(rgb[led * 3 + 0] * mul * m_redIntensity);
        buffer[++i] = (unsigned char)(rgb[led * 3 + 1] * mul * m_greenIntensity);
        buffer[++i] = (unsigned char)(rgb[led * 3 + 2] * mul * m_blueIntensity);
    }

    m_Serial.write((char *)buffer, ++i);
}
