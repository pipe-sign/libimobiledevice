/*
 * idevice.h
 * Device discovery and communication interface -- header file.
 *
 * Copyright (c) 2008 Zach C. All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __DEVICE_H
#define __DEVICE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#else
#include <gnutls/gnutls.h>
#include <gnutls/x509.h>
#endif

#ifdef WIN32
#define LIBIMOBILEDEVICE_API __declspec( dllexport )
#else
#ifdef HAVE_FVISIBILITY
#define LIBIMOBILEDEVICE_API __attribute__((visibility("default")))
#else
#define LIBIMOBILEDEVICE_API
#endif
#endif

#include "common/userpref.h"
#include "libimobiledevice/libimobiledevice.h"

#define DEVICE_VERSION(maj, min, patch) (((maj & 0xFF) << 16) | ((min & 0xFF) << 8) | (patch & 0xFF))

struct ssl_data_private { // ssl 安全连接移动设备 需要的输入数据文件
#ifdef HAVE_OPENSSL
	SSL *session;
	SSL_CTX *ctx;
#else
	gnutls_certificate_credentials_t certificate; // 证书凭据
	gnutls_session_t session; // 会话
	gnutls_x509_privkey_t root_privkey; // 根私钥
	gnutls_x509_crt_t root_cert; // 根证书
	gnutls_x509_privkey_t host_privkey; // 本程序宿主机私钥
	gnutls_x509_crt_t host_cert; // 本程序宿主机证书
#endif
};
typedef struct ssl_data_private *ssl_data_t;  // ssl 数据结构/类型的简短命名/昵称/别名 使用的时候不用输入struct关键字 _t为别名后缀，意指来源为typedef操作 

struct idevice_connection_private { // 抽象与设备的连接
	idevice_t device; // 设备信息（udid，version,mux_id,conn_type,conn_data）
	enum idevice_connection_type type; // 连接类型
	void *data; // 缓存内存空间
	ssl_data_t ssl_data; // ssl证书等信息的抽象
};

struct idevice_private { // 设备信息抽象
	char *udid; // 设备唯一标识
	uint32_t mux_id; // 互斥id，用于连接排序？
	enum idevice_connection_type conn_type; // 连接类型
	void *conn_data; // 连接数据缓存空间
	int version; // 设备版本
};

#endif
