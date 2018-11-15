// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#define ulong linux_uint

//#include <linux/time.h>
#include <time.h>
#include <dlfcn.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
//#include <errno.h>

#include <dirent.h>
#include <sys/vfs.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#undef ulong
