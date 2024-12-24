/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: api.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "api.pb-c.h"
void detection__request__init(Detection__Request *message)
{
  static const Detection__Request init_value = DETECTION__REQUEST__INIT;
  *message = init_value;
}
size_t detection__request__get_packed_size(const Detection__Request *message)
{
  assert(message->base.descriptor == &detection__request__descriptor);
  return protobuf_c_message_get_packed_size((const ProtobufCMessage *)(message));
}
size_t detection__request__pack(const Detection__Request *message,
                                uint8_t *out)
{
  assert(message->base.descriptor == &detection__request__descriptor);
  return protobuf_c_message_pack((const ProtobufCMessage *)message, out);
}
size_t detection__request__pack_to_buffer(const Detection__Request *message,
                                          ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &detection__request__descriptor);
  return protobuf_c_message_pack_to_buffer((const ProtobufCMessage *)message, buffer);
}
Detection__Request *
detection__request__unpack(ProtobufCAllocator *allocator,
                           size_t len,
                           const uint8_t *data)
{
  return (Detection__Request *)
      protobuf_c_message_unpack(&detection__request__descriptor,
                                allocator, len, data);
}
void detection__request__free_unpacked(Detection__Request *message,
                                       ProtobufCAllocator *allocator)
{
  if (!message)
    return;
  assert(message->base.descriptor == &detection__request__descriptor);
  protobuf_c_message_free_unpacked((ProtobufCMessage *)message, allocator);
}
void detection__box__init(Detection__Box *message)
{
  static const Detection__Box init_value = DETECTION__BOX__INIT;
  *message = init_value;
}
size_t detection__box__get_packed_size(const Detection__Box *message)
{
  assert(message->base.descriptor == &detection__box__descriptor);
  return protobuf_c_message_get_packed_size((const ProtobufCMessage *)(message));
}
size_t detection__box__pack(const Detection__Box *message,
                            uint8_t *out)
{
  assert(message->base.descriptor == &detection__box__descriptor);
  return protobuf_c_message_pack((const ProtobufCMessage *)message, out);
}
size_t detection__box__pack_to_buffer(const Detection__Box *message,
                                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &detection__box__descriptor);
  return protobuf_c_message_pack_to_buffer((const ProtobufCMessage *)message, buffer);
}
Detection__Box *
detection__box__unpack(ProtobufCAllocator *allocator,
                       size_t len,
                       const uint8_t *data)
{
  return (Detection__Box *)
      protobuf_c_message_unpack(&detection__box__descriptor,
                                allocator, len, data);
}
void detection__box__free_unpacked(Detection__Box *message,
                                   ProtobufCAllocator *allocator)
{
  if (!message)
    return;
  assert(message->base.descriptor == &detection__box__descriptor);
  protobuf_c_message_free_unpacked((ProtobufCMessage *)message, allocator);
}
void detection__response__init(Detection__Response *message)
{
  static const Detection__Response init_value = DETECTION__RESPONSE__INIT;
  *message = init_value;
}
size_t detection__response__get_packed_size(const Detection__Response *message)
{
  assert(message->base.descriptor == &detection__response__descriptor);
  return protobuf_c_message_get_packed_size((const ProtobufCMessage *)(message));
}
size_t detection__response__pack(const Detection__Response *message,
                                 uint8_t *out)
{
  assert(message->base.descriptor == &detection__response__descriptor);
  return protobuf_c_message_pack((const ProtobufCMessage *)message, out);
}
size_t detection__response__pack_to_buffer(const Detection__Response *message,
                                           ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &detection__response__descriptor);
  return protobuf_c_message_pack_to_buffer((const ProtobufCMessage *)message, buffer);
}
Detection__Response *
detection__response__unpack(ProtobufCAllocator *allocator,
                            size_t len,
                            const uint8_t *data)
{
  return (Detection__Response *)
      protobuf_c_message_unpack(&detection__response__descriptor,
                                allocator, len, data);
}
void detection__response__free_unpacked(Detection__Response *message,
                                        ProtobufCAllocator *allocator)
{
  if (!message)
    return;
  assert(message->base.descriptor == &detection__response__descriptor);
  protobuf_c_message_free_unpacked((ProtobufCMessage *)message, allocator);
}
static const ProtobufCFieldDescriptor detection__request__field_descriptors[3] =
    {
        {
            "width",
            1,
            PROTOBUF_C_LABEL_NONE,
            PROTOBUF_C_TYPE_INT32,
            0, /* quantifier_offset */
            offsetof(Detection__Request, width),
            NULL,
            NULL,
            0,            /* flags */
            0, NULL, NULL /* reserved1,reserved2, etc */
        },
        {
            "height",
            2,
            PROTOBUF_C_LABEL_NONE,
            PROTOBUF_C_TYPE_INT32,
            0, /* quantifier_offset */
            offsetof(Detection__Request, height),
            NULL,
            NULL,
            0,            /* flags */
            0, NULL, NULL /* reserved1,reserved2, etc */
        },
        {
            "data",
            3,
            PROTOBUF_C_LABEL_NONE,
            PROTOBUF_C_TYPE_BYTES,
            0, /* quantifier_offset */
            offsetof(Detection__Request, data),
            NULL,
            NULL,
            0,            /* flags */
            0, NULL, NULL /* reserved1,reserved2, etc */
        },
};
static const unsigned detection__request__field_indices_by_name[] = {
    2, /* field[2] = data */
    1, /* field[1] = height */
    0, /* field[0] = width */
};
static const ProtobufCIntRange detection__request__number_ranges[1 + 1] =
    {
        {1, 0},
        {0, 3}};
const ProtobufCMessageDescriptor detection__request__descriptor =
    {
        PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
        "detection.Request",
        "Request",
        "Detection__Request",
        "detection",
        sizeof(Detection__Request),
        3,
        detection__request__field_descriptors,
        detection__request__field_indices_by_name,
        1, detection__request__number_ranges,
        (ProtobufCMessageInit)detection__request__init,
        NULL, NULL, NULL /* reserved[123] */
};
static const ProtobufCFieldDescriptor detection__box__field_descriptors[6] =
    {
        {
            "x",
            1,
            PROTOBUF_C_LABEL_NONE,
            PROTOBUF_C_TYPE_INT32,
            0, /* quantifier_offset */
            offsetof(Detection__Box, x),
            NULL,
            NULL,
            0,            /* flags */
            0, NULL, NULL /* reserved1,reserved2, etc */
        },
        {
            "y",
            2,
            PROTOBUF_C_LABEL_NONE,
            PROTOBUF_C_TYPE_INT32,
            0, /* quantifier_offset */
            offsetof(Detection__Box, y),
            NULL,
            NULL,
            0,            /* flags */
            0, NULL, NULL /* reserved1,reserved2, etc */
        },
        {
            "width",
            3,
            PROTOBUF_C_LABEL_NONE,
            PROTOBUF_C_TYPE_INT32,
            0, /* quantifier_offset */
            offsetof(Detection__Box, width),
            NULL,
            NULL,
            0,            /* flags */
            0, NULL, NULL /* reserved1,reserved2, etc */
        },
        {
            "height",
            4,
            PROTOBUF_C_LABEL_NONE,
            PROTOBUF_C_TYPE_INT32,
            0, /* quantifier_offset */
            offsetof(Detection__Box, height),
            NULL,
            NULL,
            0,            /* flags */
            0, NULL, NULL /* reserved1,reserved2, etc */
        },
        {
            "confidence",
            5,
            PROTOBUF_C_LABEL_NONE,
            PROTOBUF_C_TYPE_FLOAT,
            0, /* quantifier_offset */
            offsetof(Detection__Box, confidence),
            NULL,
            NULL,
            0,            /* flags */
            0, NULL, NULL /* reserved1,reserved2, etc */
        },
        {
            "label",
            6,
            PROTOBUF_C_LABEL_NONE,
            PROTOBUF_C_TYPE_STRING,
            0, /* quantifier_offset */
            offsetof(Detection__Box, label),
            NULL,
            &protobuf_c_empty_string,
            0,            /* flags */
            0, NULL, NULL /* reserved1,reserved2, etc */
        },
};
static const unsigned detection__box__field_indices_by_name[] = {
    4, /* field[4] = confidence */
    3, /* field[3] = height */
    5, /* field[5] = label */
    2, /* field[2] = width */
    0, /* field[0] = x */
    1, /* field[1] = y */
};
static const ProtobufCIntRange detection__box__number_ranges[1 + 1] =
    {
        {1, 0},
        {0, 6}};
const ProtobufCMessageDescriptor detection__box__descriptor =
    {
        PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
        "detection.Box",
        "Box",
        "Detection__Box",
        "detection",
        sizeof(Detection__Box),
        6,
        detection__box__field_descriptors,
        detection__box__field_indices_by_name,
        1, detection__box__number_ranges,
        (ProtobufCMessageInit)detection__box__init,
        NULL, NULL, NULL /* reserved[123] */
};
static const ProtobufCFieldDescriptor detection__response__field_descriptors[2] =
    {
        {
            "box_count",
            1,
            PROTOBUF_C_LABEL_NONE,
            PROTOBUF_C_TYPE_INT32,
            0, /* quantifier_offset */
            offsetof(Detection__Response, box_count),
            NULL,
            NULL,
            0,            /* flags */
            0, NULL, NULL /* reserved1,reserved2, etc */
        },
        {
            "boxes",
            2,
            PROTOBUF_C_LABEL_REPEATED,
            PROTOBUF_C_TYPE_MESSAGE,
            offsetof(Detection__Response, n_boxes),
            offsetof(Detection__Response, boxes),
            &detection__box__descriptor,
            NULL,
            0,            /* flags */
            0, NULL, NULL /* reserved1,reserved2, etc */
        },
};
static const unsigned detection__response__field_indices_by_name[] = {
    0, /* field[0] = box_count */
    1, /* field[1] = boxes */
};
static const ProtobufCIntRange detection__response__number_ranges[1 + 1] =
    {
        {1, 0},
        {0, 2}};
const ProtobufCMessageDescriptor detection__response__descriptor =
    {
        PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
        "detection.Response",
        "Response",
        "Detection__Response",
        "detection",
        sizeof(Detection__Response),
        2,
        detection__response__field_descriptors,
        detection__response__field_indices_by_name,
        1, detection__response__number_ranges,
        (ProtobufCMessageInit)detection__response__init,
        NULL, NULL, NULL /* reserved[123] */
};
static const ProtobufCMethodDescriptor detection__detection_service__method_descriptors[1] =
    {
        {"Detect", &detection__request__descriptor, &detection__response__descriptor},
};
const unsigned detection__detection_service__method_indices_by_name[] = {
    0 /* Detect */
};
const ProtobufCServiceDescriptor detection__detection_service__descriptor =
    {
        PROTOBUF_C__SERVICE_DESCRIPTOR_MAGIC,
        "detection.DetectionService",
        "DetectionService",
        "Detection__DetectionService",
        "detection",
        1,
        detection__detection_service__method_descriptors,
        detection__detection_service__method_indices_by_name};
void detection__detection_service__detect(ProtobufCService *service,
                                          const Detection__Request *input,
                                          Detection__Response_Closure closure,
                                          void *closure_data)
{
  assert(service->descriptor == &detection__detection_service__descriptor);
  service->invoke(service, 0, (const ProtobufCMessage *)input, (ProtobufCClosure)closure, closure_data);
}
void detection__detection_service__init(Detection__DetectionService_Service *service,
                                        Detection__DetectionService_ServiceDestroy destroy)
{
  protobuf_c_service_generated_init(&service->base,
                                    &detection__detection_service__descriptor,
                                    (ProtobufCServiceDestroy)destroy);
}