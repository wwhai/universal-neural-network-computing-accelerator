#include "detector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

// Function to decode detection request from a buffer (e.g., received over the network)
void decode_detection_request(const uint8_t *buffer, size_t buffer_size, DetectionRequest **request)
{
    if (!buffer || buffer_size < sizeof(int) * 3)
    {
        *request = NULL;
        return;
    }

    // Allocate memory for DetectionRequest structure
    *request = (DetectionRequest *)malloc(sizeof(DetectionRequest));
    if (!*request)
    {
        return; // Memory allocation failure
    }

    // Extract width, height, and size
    size_t offset = 0;
    (*request)->width = *(int *)(buffer + offset);
    offset += sizeof(int);
    (*request)->height = *(int *)(buffer + offset);
    offset += sizeof(int);
    (*request)->size = *(int *)(buffer + offset);
    offset += sizeof(int);

    // Allocate memory for image data and copy it
    (*request)->data = (unsigned char *)malloc((*request)->size);
    if ((*request)->data == NULL)
    {
        free(*request);
        *request = NULL;
        return;
    }
    memcpy((*request)->data, buffer + offset, (*request)->size);
}

// Function to encode detection request into a buffer (e.g., to send over the network)
void encode_detection_request(const DetectionRequest *request, uint8_t **buffer, size_t *buffer_size)
{
    if (!request || !buffer || !buffer_size)
        return;

    *buffer_size = sizeof(int) * 3 + request->size;
    *buffer = (uint8_t *)malloc(*buffer_size);
    if (!*buffer)
        return; // Memory allocation failure

    size_t offset = 0;
    // Encode width, height, and size
    memcpy(*buffer + offset, &request->width, sizeof(int));
    offset += sizeof(int);
    memcpy(*buffer + offset, &request->height, sizeof(int));
    offset += sizeof(int);
    memcpy(*buffer + offset, &request->size, sizeof(int));
    offset += sizeof(int);

    // Encode image data
    memcpy(*buffer + offset, request->data, request->size);
}

// Function to perform object detection on an image
DetectionResult *object_detect(const unsigned char *image_data, size_t image_size, bool use_cuda)
{
    DetectionResult *result = (DetectionResult *)malloc(sizeof(DetectionResult) + sizeof(result->objects[0]) * 1); // Assume 1 object detected for now
    if (!result)
        return NULL;

    result->num_objects = 1;
    result->objects[0].x = 50;
    result->objects[0].y = 60;
    result->objects[0].w = 100;
    result->objects[0].h = 150;
    result->objects[0].prop = 0.95f; // Probability
    snprintf(result->objects[0].label, sizeof(result->objects[0].label), "example_label");

    return result;
}

// Function to free memory allocated for detection result
void free_detection_result(DetectionResult *result)
{
    if (result)
    {
        free(result);
    }
}

// Function to encode detection result into a buffer (e.g., to send back to client)
void encode_detection_result(const DetectionResult *result, uint8_t **buffer, size_t *buffer_size)
{
    if (!result || !buffer || !buffer_size)
        return;

    *buffer_size = sizeof(int) + sizeof(result->objects[0]) * result->num_objects;
    *buffer = (uint8_t *)malloc(*buffer_size);
    if (!*buffer)
        return;

    size_t offset = 0;
    // Encode number of objects
    memcpy(*buffer + offset, &result->num_objects, sizeof(int));
    offset += sizeof(int);

    // Encode object data
    for (int i = 0; i < result->num_objects; ++i)
    {
        memcpy(*buffer + offset, &result->objects[i], sizeof(result->objects[0]));
        offset += sizeof(result->objects[0]);
    }
}

// Function to decode detection result from a buffer (e.g., received from the server)
void decode_detection_result(const uint8_t *buffer, size_t buffer_size, DetectionResult **result)
{
    if (!buffer || buffer_size < sizeof(int))
    {
        *result = NULL;
        return;
    }

    *result = (DetectionResult *)malloc(sizeof(DetectionResult) + sizeof((*result)->objects[0]) * 1); // Assume at least 1 object
    if (!*result)
    {
        *result = NULL;
        return; // Memory allocation failure
    }

    size_t offset = 0;
    // Decode number of objects
    (*result)->num_objects = *(int *)(buffer + offset);
    offset += sizeof(int);

    // Decode object data
    for (int i = 0; i < (*result)->num_objects; ++i)
    {
        memcpy(&(*result)->objects[i], buffer + offset, sizeof((*result)->objects[0]));
        offset += sizeof((*result)->objects[0]);
    }
}