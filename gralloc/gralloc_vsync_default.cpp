/*
 * Copyright (C) 2014 ARM Limited. All rights reserved.
 *
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "gralloc_priv.h"
#include "gralloc_vsync.h"
#include "gralloc_vsync_report.h"
#include <sys/ioctl.h>
#include <errno.h>

#define HISIFB_VSYNC_CTRL		_IOW('M', 0x02, __u32)
#define FBIO_WAITFORVSYNC       _IOW('F', 0x20, __u32)

int gralloc_vsync_enable(framebuffer_device_t *dev)
{
	private_module_t *m = reinterpret_cast<private_module_t *>(dev->common.module);
	int enable = 1;
	int ret;
    ALOGI("gralloc_vsync_enable(): enabling vsync");
	ret = ioctl(m->framebuffer->shallow_fbdev_fd, HISIFB_VSYNC_CTRL, &enable);
	if (ret == -1) {
		ALOGE("gralloc_vsync_enable(): %d", errno);
	}
	return 0;
}

int gralloc_vsync_disable(framebuffer_device_t *dev)
{
	private_module_t *m = reinterpret_cast<private_module_t *>(dev->common.module);
	int enable = 0;
	int ret;

    ALOGI("gralloc_vsync_disable(): disabling vsync");
	ret = ioctl(m->framebuffer->shallow_fbdev_fd, HISIFB_VSYNC_CTRL, &enable);
	if (ret == -1) {
		ALOGE("gralloc_vsync_enable(): %d", errno);
	}
	return 0;
}

int gralloc_wait_for_vsync(framebuffer_device_t *dev)
{
	GRALLOC_UNUSED(dev);
	return 0;
}
