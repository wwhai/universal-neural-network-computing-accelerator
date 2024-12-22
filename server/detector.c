#include "detector.h"
#include <stdlib.h>
#include "gpu_utils.h"

// 在此实现使用 GPU 的目标检测逻辑
DetectionResult *object_detect(const unsigned char *image_data, size_t image_size)
{
    DetectionResult *result = NULL;

    return result;
}

// 释放检测结果结构体的内存
void free_detection_result(DetectionResult *result)
{
    if (result == NULL)
        return;
    free(result);
}
