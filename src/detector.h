#ifndef DETECTOR_H
#define DETECTOR_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    int width;
    int height;
    int size;
    unsigned char *data;
} DetectionRequest;
//
void decode_detection_request(const uint8_t *buffer, size_t buffer_size, DetectionRequest **request);
void encode_detection_request(const DetectionRequest *request, uint8_t **buffer, size_t *buffer_size);

typedef struct
{
    int num_objects;
    struct
    {
        int x, y, w, h;
        float prop;
        char label[40];
    } objects[];
} DetectionResult;

DetectionResult *object_detect(const unsigned char *image_data, size_t image_size, bool use_cuda);
void free_detection_result(DetectionResult *result);
void encode_detection_result(const DetectionResult *result, uint8_t **buffer, size_t *buffer_size);
void decode_detection_result(const uint8_t *buffer, size_t buffer_size, DetectionResult **result);

#endif
