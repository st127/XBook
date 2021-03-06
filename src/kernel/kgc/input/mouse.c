/*
 * file:		kernel/kgc/input/mouse.c
 * auther:		Jason Hu
 * time:		2020/2/20
 * copyright:	(C) 2018-2020 by Book OS developers. All rights reserved.
 */

/* 系统内核 */
#include <book/config.h>
#include <book/arch.h>
#include <book/debug.h>
#include <book/kgc.h>
#include <kgc/color.h>
#include <video/video.h>
#include <kgc/draw.h>
#include <kgc/button.h>
#include <kgc/container/draw.h>
#include <kgc/input/mouse.h>
#include <kgc/window/window.h>
#include <kgc/window/draw.h>
#include <kgc/window/even.h>
#include <kgc/bar/bar.h>
#include <kgc/desktop/desktop.h>
#include <video/video.h>

//#define DEBUG_MOUSE


EXTERN struct List windowListHead;
EXTERN KGC_WindowMovement_t windowMovement;

PROTECT struct KGC_Mouse mouse;

PUBLIC void KGC_MouseMove(int xinc, int yinc)
{
    /* 改变鼠标位置 */
    mouse.x += xinc;
    mouse.y += yinc;
    /* 修复鼠标位置 */
    if (mouse.x < 0) {
        mouse.x = 0;
    }
    if (mouse.x > videoInfo.xResolution - 1) {
        mouse.x = videoInfo.xResolution - 1;
    }
    if (mouse.y < 0) {
        mouse.y = 0;
    }
    if (mouse.y > videoInfo.yResolution - 1) {
        mouse.y = videoInfo.yResolution - 1;
    }
    
    /* 移动鼠标 */
    KGC_ContainerSlide(mouse.container, mouse.x, mouse.y);    
}

PUBLIC void KGC_MouseMotion(KGC_MouseMotionEven_t *motion)
{
    /* 鼠标移动显示及位置处理 */
    KGC_MouseMove(motion->x, motion->y);

    /* 发送到当前窗口 */
    KGC_WindowMouseMotion(mouse.x, mouse.y);

    /* 如果有抓取的窗口，移动鼠标的时候就要移动窗口 */    
    if (windowMovement.holded) {
#if KGC_WINDOW_MOVEMENT_SHRINK == 1
        /* 把要移动的容器跟随鼠标移动 */
        KGC_ContainerSlide(windowMovement.walker, mouse.x - windowMovement.offsetX,
            mouse.y - windowMovement.offsetY);
#else
        KGC_ContainerSlide(windowMovement.holded->container, mouse.x - windowMovement.offsetX,
            mouse.y - windowMovement.offsetY);
#endif  /* KGC_WINDOW_MOVEMENT_SHRINK */
    } else {
        /* 发送到任务栏 */
        KGC_BarMouseMotion(mouse.x, mouse.y);
        /* 发送到桌面 */
        KGC_DesktopMouseMotion(mouse.x, mouse.y);

    }
}

/**
 * KGC_MouseDown - 鼠标单击按下
 * @button: 按钮
 */
PUBLIC void KGC_MouseDown(KGC_MouseButtonEven_t *button)
{
    mouse.button = button->button;
    //printk("mouse down:%d at (%d,%d)\n", button->button, mouse.x, mouse.y);
    /* 如果已经抓取了窗口，就不能检查其它 */
    if (windowMovement.holded == NULL) {
        
        /* 发送到任务栏 */
        KGC_BarMouseDown(button->button, mouse.x, mouse.y);
        /* 发送到当前窗口 */
        KGC_WindowMouseDown(button->button, mouse.x, mouse.y);
    
        /* 发送到桌面 */
        KGC_DesktopMouseDown(button->button, mouse.x, mouse.y);
    }
    
}

/**
 * KGC_MouseUp - 鼠标单击弹起
 * @button: 按钮
 */
PUBLIC void KGC_MouseUp(KGC_MouseButtonEven_t *button)
{
    /* 发送到当前窗口 */
    KGC_WindowMouseUp(button->button, mouse.x, mouse.y);
        
    if (windowMovement.holded) {
#if KGC_WINDOW_MOVEMENT_SHRINK == 1
        /* 隐藏walker */
        KGC_ContainerZ(windowMovement.walker, -1);
        /* 把walker设置为全透明 */
        KGC_WindowPaintMoving(windowMovement.walker, windowMovement.holded, 0);

        /* 窗口移动到鼠标的位置 */
        KGC_ContainerSlide(windowMovement.holded->container, mouse.x - windowMovement.offsetX,
            mouse.y - windowMovement.offsetY);
        /* 把窗口放到鼠标下面 */
        //KGC_ContainerBelowTopZ(windowMovement.holded->container);
        
        /* 切换到窗口 */
        KGC_SwitchWindow(windowMovement.holded);
                    
#endif /* KGC_WINDOW_MOVEMENT_SHRINK */
        
        windowMovement.holded = NULL;
    } else { /* 如果已经抓取了窗口，就不能检查其它 */
        /* 处理事件 */
        KGC_BarMouseUp(button->button, mouse.x, mouse.y);
        
        /* 发送到桌面 */
        KGC_DesktopMouseUp(button->button, mouse.x, mouse.y);
    }    
}

/**
 * KGC_RefreshMouse - 单独刷新鼠标
 * 
 */
PUBLIC void KGC_RefreshMouse()
{
    KGC_ContainerRefresh(mouse.container, 0, 0, KGC_MOUSE_CONTAINER_SIZE, KGC_MOUSE_CONTAINER_SIZE);
}

/* 鼠标大小是16*16，所以uint16就够了 */
PRIVATE char mouseCursorData[KGC_MOUSE_CONTAINER_SIZE][KGC_MOUSE_CONTAINER_SIZE] = {
    {1,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0,0,0,0,0},
    {1,2,1,0,0,0,0,0,0,0,0,0},
    {1,2,2,1,0,0,0,0,0,0,0,0},
    {1,2,2,2,1,0,0,0,0,0,0,0},
    {1,2,2,2,2,1,0,0,0,0,0,0},
    {1,2,2,2,2,2,1,0,0,0,0,0},
    {1,2,2,2,2,2,2,1,0,0,0,0},
    {1,2,2,2,2,2,2,2,1,0,0,0},
    {1,2,2,2,2,2,2,2,2,1,0,0},
    {1,2,2,2,2,1,1,1,1,1,1,0},
    {1,2,2,2,1,0,0,0,0,0,0,0},
    {1,2,2,1,0,0,0,0,0,0,0,0},
    {1,2,1,0,0,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,0},
};

/**
 * KGC_DrawMousecontainer - 往鼠标视图写入数据
 * 
 */
PRIVATE void KGC_DrawMouseBuffer(uint32_t *bmp)
{
    int x, y;
	for (y = 0; y < KGC_MOUSE_CONTAINER_SIZE; y++) {
		for (x = 0; x < KGC_MOUSE_CONTAINER_SIZE; x++) {
			if (mouseCursorData[y][x] == 0) {
                bmp[y * KGC_MOUSE_CONTAINER_SIZE + x] = KGC_MOUSE_UNIQUE_COLOR;
            } else if (mouseCursorData[y][x] == 1) {
				bmp[y * KGC_MOUSE_CONTAINER_SIZE + x] = KGC_MOUSE_BORDER_COLOR;
            } else if (mouseCursorData[y][x] == 2) {
                bmp[y * KGC_MOUSE_CONTAINER_SIZE + x] = KGC_MOUSE_FILL_COLOR;
            }
		}
	}
}

PUBLIC int KGC_InitMouseContainer()
{
    mouse.x = videoInfo.xResolution / 2;
    mouse.y = videoInfo.yResolution / 2;
    /* 鼠标 */
    mouse.container = KGC_ContainerAlloc();
    if (mouse.container == NULL) 
        return -1;

    KGC_ContainerInit(mouse.container, "mouse", 0, 0, KGC_MOUSE_CONTAINER_SIZE, KGC_MOUSE_CONTAINER_SIZE, NULL);

    mouse.bmp = kmalloc(KGC_MOUSE_CONTAINER_SIZE * KGC_MOUSE_CONTAINER_SIZE * 4, GFP_KERNEL);
    if (mouse.bmp == NULL) {
        kfree(mouse.container);
        return -1;
    }
    /* 绘制鼠标指针到缓冲区 */
    KGC_DrawMouseBuffer(mouse.container->buffer);
    KGC_ContainerAboveTopZ(mouse.container); 
    KGC_ContainerSlide(mouse.container, mouse.x, mouse.y);
    return 0;
}
