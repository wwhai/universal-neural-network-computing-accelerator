/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: api.proto */

#ifndef PROTOBUF_C_api_2eproto__INCLUDED
#define PROTOBUF_C_api_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _UNNCA__DetectRequest UNNCA__DetectRequest;
typedef struct _UNNCA__Box UNNCA__Box;
typedef struct _UNNCA__DetectResponse UNNCA__DetectResponse;
typedef struct _UNNCA__AuthRequest UNNCA__AuthRequest;
typedef struct _UNNCA__AuthResponse UNNCA__AuthResponse;
typedef struct _UNNCA__PingRequest UNNCA__PingRequest;
typedef struct _UNNCA__PingResponse UNNCA__PingResponse;
typedef struct _UNNCA__AcceleratorInfoRequest UNNCA__AcceleratorInfoRequest;
typedef struct _UNNCA__AcceleratorInfoResponse UNNCA__AcceleratorInfoResponse;
typedef struct _UNNCA__RpcRequest UNNCA__RpcRequest;
typedef struct _UNNCA__RpcResponse UNNCA__RpcResponse;


/* --- enums --- */


/* --- messages --- */

/*
 * 请求结构体定义
 */
struct  _UNNCA__DetectRequest
{
  ProtobufCMessage base;
  /*
   * width
   */
  int32_t width;
  /*
   * height
   */
  int32_t height;
  /*
   * 指向的数据（例如图像数据）
   */
  ProtobufCBinaryData data;
};
#define UNNCA__DETECT_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&unnca__detect_request__descriptor) \
    , 0, 0, {0,NULL} }


/*
 * 单个检测框定义
 */
struct  _UNNCA__Box
{
  ProtobufCMessage base;
  /*
   * 检测框的 x 坐标
   */
  int32_t x;
  /*
   * 检测框的 y 坐标
   */
  int32_t y;
  /*
   * 检测框的宽度
   */
  int32_t width;
  /*
   * 检测框的高度
   */
  int32_t height;
  /*
   * 置信度 (prop)
   */
  float confidence;
  /*
   * 标签信息，最多 40 个字符
   */
  char *label;
};
#define UNNCA__BOX__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&unnca__box__descriptor) \
    , 0, 0, 0, 0, 0, (char *)protobuf_c_empty_string }


/*
 * 返回结构体定义
 */
struct  _UNNCA__DetectResponse
{
  ProtobufCMessage base;
  /*
   * 检测到的框的数量
   */
  int32_t box_count;
  /*
   * 检测到的框列表，最多 20 个
   */
  size_t n_boxes;
  UNNCA__Box **boxes;
};
#define UNNCA__DETECT_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&unnca__detect_response__descriptor) \
    , 0, 0,NULL }


/*
 * 心跳包
 */
struct  _UNNCA__AuthRequest
{
  ProtobufCMessage base;
  /*
   * 设备唯一标识
   */
  char *uuid;
};
#define UNNCA__AUTH_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&unnca__auth_request__descriptor) \
    , (char *)protobuf_c_empty_string }


/*
 * 心跳包返回
 */
struct  _UNNCA__AuthResponse
{
  ProtobufCMessage base;
  /*
   * 状态码
   */
  int32_t code;
  /*
   * 设备唯一标识
   */
  char *msg;
};
#define UNNCA__AUTH_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&unnca__auth_response__descriptor) \
    , 0, (char *)protobuf_c_empty_string }


/*
 * 心跳包
 */
struct  _UNNCA__PingRequest
{
  ProtobufCMessage base;
  /*
   * 状态码
   */
  int32_t code;
  /*
   * 设备唯一标识
   */
  char *uuid;
};
#define UNNCA__PING_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&unnca__ping_request__descriptor) \
    , 0, (char *)protobuf_c_empty_string }


/*
 * 心跳包返回
 */
struct  _UNNCA__PingResponse
{
  ProtobufCMessage base;
  /*
   * 状态码
   */
  int32_t status;
};
#define UNNCA__PING_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&unnca__ping_response__descriptor) \
    , 0 }


/*
 * 获取设备信息
 */
struct  _UNNCA__AcceleratorInfoRequest
{
  ProtobufCMessage base;
};
#define UNNCA__ACCELERATOR_INFO_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&unnca__accelerator_info_request__descriptor) \
     }


/*
 * 获取设备信息返回
 */
struct  _UNNCA__AcceleratorInfoResponse
{
  ProtobufCMessage base;
  /*
   * 版本号
   */
  char *version;
  /*
   * 厂家
   */
  char *vendor;
  /*
   * 型号
   */
  char *model;
  /*
   * 名称
   */
  char *name;
  /*
   * 设备唯一标识
   */
  char *uuid;
};
#define UNNCA__ACCELERATOR_INFO_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&unnca__accelerator_info_response__descriptor) \
    , (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string }


typedef enum {
  UNNCA__RPC_REQUEST__REQUEST__NOT_SET = 0,
  UNNCA__RPC_REQUEST__REQUEST_AUTH_REQUEST = 1,
  UNNCA__RPC_REQUEST__REQUEST_PING_REQUEST = 2,
  UNNCA__RPC_REQUEST__REQUEST_DETECT_REQUEST = 3,
  UNNCA__RPC_REQUEST__REQUEST_ACCELERATOR_INFO_REQUEST = 4
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(UNNCA__RPC_REQUEST__REQUEST)
} UNNCA__RpcRequest__RequestCase;

/*
 * 通用的消息
 */
struct  _UNNCA__RpcRequest
{
  ProtobufCMessage base;
  UNNCA__RpcRequest__RequestCase request_case;
  union {
    UNNCA__AuthRequest *auth_request;
    UNNCA__PingRequest *ping_request;
    UNNCA__DetectRequest *detect_request;
    UNNCA__AcceleratorInfoRequest *accelerator_info_request;
  };
};
#define UNNCA__RPC_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&unnca__rpc_request__descriptor) \
    , UNNCA__RPC_REQUEST__REQUEST__NOT_SET, {0} }


typedef enum {
  UNNCA__RPC_RESPONSE__RESPONSE__NOT_SET = 0,
  UNNCA__RPC_RESPONSE__RESPONSE_AUTH_RESPONSE = 1,
  UNNCA__RPC_RESPONSE__RESPONSE_PING_RESPONSE = 2,
  UNNCA__RPC_RESPONSE__RESPONSE_DETECT_RESPONSE = 3,
  UNNCA__RPC_RESPONSE__RESPONSE_ACCELERATOR_INFO_RESPONSE = 4
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(UNNCA__RPC_RESPONSE__RESPONSE)
} UNNCA__RpcResponse__ResponseCase;

struct  _UNNCA__RpcResponse
{
  ProtobufCMessage base;
  UNNCA__RpcResponse__ResponseCase response_case;
  union {
    UNNCA__AuthResponse *auth_response;
    UNNCA__PingResponse *ping_response;
    UNNCA__DetectResponse *detect_response;
    UNNCA__AcceleratorInfoResponse *accelerator_info_response;
  };
};
#define UNNCA__RPC_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&unnca__rpc_response__descriptor) \
    , UNNCA__RPC_RESPONSE__RESPONSE__NOT_SET, {0} }


/* UNNCA__DetectRequest methods */
void   unnca__detect_request__init
                     (UNNCA__DetectRequest         *message);
size_t unnca__detect_request__get_packed_size
                     (const UNNCA__DetectRequest   *message);
size_t unnca__detect_request__pack
                     (const UNNCA__DetectRequest   *message,
                      uint8_t             *out);
size_t unnca__detect_request__pack_to_buffer
                     (const UNNCA__DetectRequest   *message,
                      ProtobufCBuffer     *buffer);
UNNCA__DetectRequest *
       unnca__detect_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   unnca__detect_request__free_unpacked
                     (UNNCA__DetectRequest *message,
                      ProtobufCAllocator *allocator);
/* UNNCA__Box methods */
void   unnca__box__init
                     (UNNCA__Box         *message);
size_t unnca__box__get_packed_size
                     (const UNNCA__Box   *message);
size_t unnca__box__pack
                     (const UNNCA__Box   *message,
                      uint8_t             *out);
size_t unnca__box__pack_to_buffer
                     (const UNNCA__Box   *message,
                      ProtobufCBuffer     *buffer);
UNNCA__Box *
       unnca__box__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   unnca__box__free_unpacked
                     (UNNCA__Box *message,
                      ProtobufCAllocator *allocator);
/* UNNCA__DetectResponse methods */
void   unnca__detect_response__init
                     (UNNCA__DetectResponse         *message);
size_t unnca__detect_response__get_packed_size
                     (const UNNCA__DetectResponse   *message);
size_t unnca__detect_response__pack
                     (const UNNCA__DetectResponse   *message,
                      uint8_t             *out);
size_t unnca__detect_response__pack_to_buffer
                     (const UNNCA__DetectResponse   *message,
                      ProtobufCBuffer     *buffer);
UNNCA__DetectResponse *
       unnca__detect_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   unnca__detect_response__free_unpacked
                     (UNNCA__DetectResponse *message,
                      ProtobufCAllocator *allocator);
/* UNNCA__AuthRequest methods */
void   unnca__auth_request__init
                     (UNNCA__AuthRequest         *message);
size_t unnca__auth_request__get_packed_size
                     (const UNNCA__AuthRequest   *message);
size_t unnca__auth_request__pack
                     (const UNNCA__AuthRequest   *message,
                      uint8_t             *out);
size_t unnca__auth_request__pack_to_buffer
                     (const UNNCA__AuthRequest   *message,
                      ProtobufCBuffer     *buffer);
UNNCA__AuthRequest *
       unnca__auth_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   unnca__auth_request__free_unpacked
                     (UNNCA__AuthRequest *message,
                      ProtobufCAllocator *allocator);
/* UNNCA__AuthResponse methods */
void   unnca__auth_response__init
                     (UNNCA__AuthResponse         *message);
size_t unnca__auth_response__get_packed_size
                     (const UNNCA__AuthResponse   *message);
size_t unnca__auth_response__pack
                     (const UNNCA__AuthResponse   *message,
                      uint8_t             *out);
size_t unnca__auth_response__pack_to_buffer
                     (const UNNCA__AuthResponse   *message,
                      ProtobufCBuffer     *buffer);
UNNCA__AuthResponse *
       unnca__auth_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   unnca__auth_response__free_unpacked
                     (UNNCA__AuthResponse *message,
                      ProtobufCAllocator *allocator);
/* UNNCA__PingRequest methods */
void   unnca__ping_request__init
                     (UNNCA__PingRequest         *message);
size_t unnca__ping_request__get_packed_size
                     (const UNNCA__PingRequest   *message);
size_t unnca__ping_request__pack
                     (const UNNCA__PingRequest   *message,
                      uint8_t             *out);
size_t unnca__ping_request__pack_to_buffer
                     (const UNNCA__PingRequest   *message,
                      ProtobufCBuffer     *buffer);
UNNCA__PingRequest *
       unnca__ping_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   unnca__ping_request__free_unpacked
                     (UNNCA__PingRequest *message,
                      ProtobufCAllocator *allocator);
/* UNNCA__PingResponse methods */
void   unnca__ping_response__init
                     (UNNCA__PingResponse         *message);
size_t unnca__ping_response__get_packed_size
                     (const UNNCA__PingResponse   *message);
size_t unnca__ping_response__pack
                     (const UNNCA__PingResponse   *message,
                      uint8_t             *out);
size_t unnca__ping_response__pack_to_buffer
                     (const UNNCA__PingResponse   *message,
                      ProtobufCBuffer     *buffer);
UNNCA__PingResponse *
       unnca__ping_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   unnca__ping_response__free_unpacked
                     (UNNCA__PingResponse *message,
                      ProtobufCAllocator *allocator);
/* UNNCA__AcceleratorInfoRequest methods */
void   unnca__accelerator_info_request__init
                     (UNNCA__AcceleratorInfoRequest         *message);
size_t unnca__accelerator_info_request__get_packed_size
                     (const UNNCA__AcceleratorInfoRequest   *message);
size_t unnca__accelerator_info_request__pack
                     (const UNNCA__AcceleratorInfoRequest   *message,
                      uint8_t             *out);
size_t unnca__accelerator_info_request__pack_to_buffer
                     (const UNNCA__AcceleratorInfoRequest   *message,
                      ProtobufCBuffer     *buffer);
UNNCA__AcceleratorInfoRequest *
       unnca__accelerator_info_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   unnca__accelerator_info_request__free_unpacked
                     (UNNCA__AcceleratorInfoRequest *message,
                      ProtobufCAllocator *allocator);
/* UNNCA__AcceleratorInfoResponse methods */
void   unnca__accelerator_info_response__init
                     (UNNCA__AcceleratorInfoResponse         *message);
size_t unnca__accelerator_info_response__get_packed_size
                     (const UNNCA__AcceleratorInfoResponse   *message);
size_t unnca__accelerator_info_response__pack
                     (const UNNCA__AcceleratorInfoResponse   *message,
                      uint8_t             *out);
size_t unnca__accelerator_info_response__pack_to_buffer
                     (const UNNCA__AcceleratorInfoResponse   *message,
                      ProtobufCBuffer     *buffer);
UNNCA__AcceleratorInfoResponse *
       unnca__accelerator_info_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   unnca__accelerator_info_response__free_unpacked
                     (UNNCA__AcceleratorInfoResponse *message,
                      ProtobufCAllocator *allocator);
/* UNNCA__RpcRequest methods */
void   unnca__rpc_request__init
                     (UNNCA__RpcRequest         *message);
size_t unnca__rpc_request__get_packed_size
                     (const UNNCA__RpcRequest   *message);
size_t unnca__rpc_request__pack
                     (const UNNCA__RpcRequest   *message,
                      uint8_t             *out);
size_t unnca__rpc_request__pack_to_buffer
                     (const UNNCA__RpcRequest   *message,
                      ProtobufCBuffer     *buffer);
UNNCA__RpcRequest *
       unnca__rpc_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   unnca__rpc_request__free_unpacked
                     (UNNCA__RpcRequest *message,
                      ProtobufCAllocator *allocator);
/* UNNCA__RpcResponse methods */
void   unnca__rpc_response__init
                     (UNNCA__RpcResponse         *message);
size_t unnca__rpc_response__get_packed_size
                     (const UNNCA__RpcResponse   *message);
size_t unnca__rpc_response__pack
                     (const UNNCA__RpcResponse   *message,
                      uint8_t             *out);
size_t unnca__rpc_response__pack_to_buffer
                     (const UNNCA__RpcResponse   *message,
                      ProtobufCBuffer     *buffer);
UNNCA__RpcResponse *
       unnca__rpc_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   unnca__rpc_response__free_unpacked
                     (UNNCA__RpcResponse *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*UNNCA__DetectRequest_Closure)
                 (const UNNCA__DetectRequest *message,
                  void *closure_data);
typedef void (*UNNCA__Box_Closure)
                 (const UNNCA__Box *message,
                  void *closure_data);
typedef void (*UNNCA__DetectResponse_Closure)
                 (const UNNCA__DetectResponse *message,
                  void *closure_data);
typedef void (*UNNCA__AuthRequest_Closure)
                 (const UNNCA__AuthRequest *message,
                  void *closure_data);
typedef void (*UNNCA__AuthResponse_Closure)
                 (const UNNCA__AuthResponse *message,
                  void *closure_data);
typedef void (*UNNCA__PingRequest_Closure)
                 (const UNNCA__PingRequest *message,
                  void *closure_data);
typedef void (*UNNCA__PingResponse_Closure)
                 (const UNNCA__PingResponse *message,
                  void *closure_data);
typedef void (*UNNCA__AcceleratorInfoRequest_Closure)
                 (const UNNCA__AcceleratorInfoRequest *message,
                  void *closure_data);
typedef void (*UNNCA__AcceleratorInfoResponse_Closure)
                 (const UNNCA__AcceleratorInfoResponse *message,
                  void *closure_data);
typedef void (*UNNCA__RpcRequest_Closure)
                 (const UNNCA__RpcRequest *message,
                  void *closure_data);
typedef void (*UNNCA__RpcResponse_Closure)
                 (const UNNCA__RpcResponse *message,
                  void *closure_data);

/* --- services --- */

typedef struct _UNNCA__DetectionService_Service UNNCA__DetectionService_Service;
struct _UNNCA__DetectionService_Service
{
  ProtobufCService base;
  void (*auth)(UNNCA__DetectionService_Service *service,
               const UNNCA__AuthRequest *input,
               UNNCA__AuthResponse_Closure closure,
               void *closure_data);
  void (*detect)(UNNCA__DetectionService_Service *service,
                 const UNNCA__DetectRequest *input,
                 UNNCA__DetectResponse_Closure closure,
                 void *closure_data);
  void (*ping)(UNNCA__DetectionService_Service *service,
               const UNNCA__PingRequest *input,
               UNNCA__PingResponse_Closure closure,
               void *closure_data);
  void (*accelerator_info)(UNNCA__DetectionService_Service *service,
                           const UNNCA__AcceleratorInfoRequest *input,
                           UNNCA__AcceleratorInfoResponse_Closure closure,
                           void *closure_data);
};
typedef void (*UNNCA__DetectionService_ServiceDestroy)(UNNCA__DetectionService_Service *);
void unnca__detection_service__init (UNNCA__DetectionService_Service *service,
                                     UNNCA__DetectionService_ServiceDestroy destroy);
#define UNNCA__DETECTION_SERVICE__BASE_INIT \
    { &unnca__detection_service__descriptor, protobuf_c_service_invoke_internal, NULL }
#define UNNCA__DETECTION_SERVICE__INIT(function_prefix__) \
    { UNNCA__DETECTION_SERVICE__BASE_INIT,\
      function_prefix__ ## auth,\
      function_prefix__ ## detect,\
      function_prefix__ ## ping,\
      function_prefix__ ## accelerator_info  }
void unnca__detection_service__auth(ProtobufCService *service,
                                    const UNNCA__AuthRequest *input,
                                    UNNCA__AuthResponse_Closure closure,
                                    void *closure_data);
void unnca__detection_service__detect(ProtobufCService *service,
                                      const UNNCA__DetectRequest *input,
                                      UNNCA__DetectResponse_Closure closure,
                                      void *closure_data);
void unnca__detection_service__ping(ProtobufCService *service,
                                    const UNNCA__PingRequest *input,
                                    UNNCA__PingResponse_Closure closure,
                                    void *closure_data);
void unnca__detection_service__accelerator_info(ProtobufCService *service,
                                                const UNNCA__AcceleratorInfoRequest *input,
                                                UNNCA__AcceleratorInfoResponse_Closure closure,
                                                void *closure_data);

/* --- descriptors --- */

extern const ProtobufCMessageDescriptor unnca__detect_request__descriptor;
extern const ProtobufCMessageDescriptor unnca__box__descriptor;
extern const ProtobufCMessageDescriptor unnca__detect_response__descriptor;
extern const ProtobufCMessageDescriptor unnca__auth_request__descriptor;
extern const ProtobufCMessageDescriptor unnca__auth_response__descriptor;
extern const ProtobufCMessageDescriptor unnca__ping_request__descriptor;
extern const ProtobufCMessageDescriptor unnca__ping_response__descriptor;
extern const ProtobufCMessageDescriptor unnca__accelerator_info_request__descriptor;
extern const ProtobufCMessageDescriptor unnca__accelerator_info_response__descriptor;
extern const ProtobufCMessageDescriptor unnca__rpc_request__descriptor;
extern const ProtobufCMessageDescriptor unnca__rpc_response__descriptor;
extern const ProtobufCServiceDescriptor unnca__detection_service__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_api_2eproto__INCLUDED */
