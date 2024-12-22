#ifndef DETECTOR_H
#define DETECTOR_H

#include <stddef.h>
#include <stdbool.h>

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

#endif
