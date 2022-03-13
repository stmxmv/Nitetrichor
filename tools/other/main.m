// -*- Nitetrichor -*-
//===--------------------------- `target name` ---------------------------------===//
//
// main.m
// 
// Created by Molybdenum on 11/1/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//===----------------------------------------------------------------------===//


#if ! __has_feature(objc_arc)
#error "ARC is off"
#endif
#import <objc/objc-runtime.h>
#import <Foundation/Foundation.h>
#include <net/if.h>
#include <sys/types.h>
#include <net/if_utun.h>
#import <sys/ioctl.h>


#include <sys/sys_domain.h>
#include <sys/socket.h>
#include <sys/kern_control.h>
#include <net/if_utun.h>       // for UTUN_CONTROL_NAME

int tun(void)  {
    struct sockaddr_ctl sc;
    struct ctl_info ctlInfo;

    memset(&ctlInfo, 0, sizeof(ctlInfo));
    strlcpy(ctlInfo.ctl_name, UTUN_CONTROL_NAME, sizeof(ctlInfo.ctl_name));

    int fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    if (fd == -1) { /* perror .. */ return -1;}
    if (ioctl(fd, CTLIOCGINFO, &ctlInfo) < 0) { /* perror.. */ return -1; }

    sc.sc_id = ctlInfo.ctl_id; sc.sc_len = sizeof(sc);
    sc.sc_family = AF_SYSTEM;  sc.ss_sysaddr = AF_SYS_CONTROL;
    sc.sc_unit = 5; /* To create utun1, just in case utun0 is in use */

    /* utun%d device will be created, where "%d" is unit number -1 */
    if (connect(fd, (struct sockaddr *)&sc, sizeof(sc)) == -1) {
        perror ("connect(AF_SYSCONTROL)");
        close(fd);
        return -1;
    }

    return fd;
}

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        char *text = "Hello, World!";
        CGDirectDisplayID display = kCGDirectMainDisplay; // 1
        CGError err = CGDisplayCapture (display); // 2
        if (err == kCGErrorSuccess)
        {
            CGContextRef ctx = CGDisplayGetDrawingContext (display); // 3
            if (ctx != NULL)
            {
                CGContextSelectFont (ctx, "Times-Roman", 48, kCGEncodingMacRoman);
                CGContextSetTextDrawingMode (ctx, kCGTextFillStroke);
                CGContextSetRGBFillColor (ctx, 1, 1, 1, 0.75);
                CGContextSetRGBStrokeColor (ctx, 1, 1, 1, 0.75);
                CGContextShowTextAtPoint (ctx, 40, 40, text, strlen(text)); // 4
                sleep (4); // 5
            }
            CGDisplayRelease (display); // 6
        }
    }
    return 0;
}