/*
 * This file is part of the Marmalade SDK Code Samples.
 *
 * Copyright (C) 2001-2012 Ideaworks3D Ltd.
 * All Rights Reserved.
 *
 * This source code is intended only as a supplement to Ideaworks Labs
 * Development Tools and/or on-line documentation.
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 */

/**
 * @page ExampleS3EHelloWorld S3E Hello World Example
 *
 * The following example, in typical Hello World style, displays the phrase
 * "Hello, World!" on screen.
 *
 * The functions required to achieve this are:
 * Printing the text to screen:
 *  - s3eDebugPrint()
 *
 * Handling the text:
 *  - s3eDeviceCheckQuitRequest()
 *  - s3eSurfaceClear()
 *  - s3eSurfaceShow()
 *
 * All examples will follow this basic pattern; a brief description of what
 * the example does will be given followed by a list of all the important
 * functions and, perhaps, classes.
 *
 * Should the example be more complex, a more detailed explanation of what the
 * example does and how it does it will be added. Note that most examples
 * use an example framework to remove boilerplate code and allow the projects
 * to be made up of a single source file for easy viewing. This framework can
 * be found in the examples/s3e/ExamplesMain directory.
 *
 * @include s3eHelloWorld.cpp
 */
#include "s3e.h"
#include "hash_map"

extern void vservManagerSkipAd();
extern int vservManagerFetchingAdData();
extern void vservManagerInit(std::hash_map<char*,char*> p_hashMap);

int32 keyPressedState = S3E_KEY_INVALID;
char* buttonStatus = NULL;

std::hash_map<char*, char*> hashMap;
void hashInit() {
	hashMap["ShowAt"] = "both";
	hashMap["ViewMandatory"] = "false";
	hashMap["ZoneID"] = "YOUR_ZONE_ID"; // given Zoneid by vserv
}


// Rotate the layout on rotation
static int HelloWorldRotationCallBack(void* systemData, void* userData)
{
	s3eSurfaceClear(0, 0, 255);
    const int width  = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
    const int height = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);
	return 0;
}



void DrawButtonRect(int x, int y, int width, int height, uint8 r, uint8 g, uint8 b)
{
    int right = x + width;
    int bottom = y + height;
    int pitch = s3eSurfaceGetInt(S3E_SURFACE_PITCH);
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (right > (int32)s3eSurfaceGetInt(S3E_SURFACE_WIDTH))
        right = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
    if (bottom > (int32)s3eSurfaceGetInt(S3E_SURFACE_HEIGHT))
        bottom = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);

    uint16* pSurface = (uint16*)s3eSurfacePtr();
    pitch /= 2;
    uint16 colour = (uint16)s3eSurfaceConvertRGB(r,g,b);

    if (((right - x) & 0x3) == 0)
    {
        for (int _y = y; _y < bottom; _y++)
        {
            uint16* p = pSurface + _y*pitch + x;
            uint16* pEnd = p + right - x;

            do
            {
                *p++ = colour;
                *p++ = colour;
                *p++ = colour;
                *p++ = colour;
            } while (p != pEnd);
        }
    }
    else
    {
        for (int _y = y; _y < bottom; _y++)
        {
            uint16* p = pSurface + _y*pitch + x;
            uint16* pEnd = p + right - x;

            do
            {
                *p++ = colour;
            } while (p != pEnd);
        }
    }
}

int CheckButtonStatus(char* pName) 
{
	if(buttonStatus != NULL && pName != NULL){
		if(strcmp(pName,buttonStatus) == 0) {
			return keyPressedState;
		}
	}
	return 0;
}


void createButton(char* p_btnName)
{
	// Draw button area
	int fontWidth = s3eDebugGetInt(S3E_DEBUG_FONT_WIDTH);
	int x,y,width;
	if(p_btnName != NULL && strlen(p_btnName) > 1)
		width = strlen(p_btnName)*fontWidth+10;
	int height = 40;
	//x = IwGxGetScreenWidth() - width;
	x = s3eSurfaceGetInt(S3E_SURFACE_WIDTH) - width;
	y = 2;
	 if (!(s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_UP))
    {
        int pointerx = s3ePointerGetX();
        int pointery = s3ePointerGetY();
		if (pointerx >=x && pointerx <= x+width && pointery >=y && pointery <= y+height)
        {
            if (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN)
            {
                keyPressedState = S3E_KEY_STATE_DOWN;
            }
            if (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_PRESSED)
            {
                keyPressedState = S3E_KEY_STATE_PRESSED;
            }
			if (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_RELEASED)
            {
                keyPressedState = S3E_KEY_STATE_RELEASED;
				s3eDebugTraceLine("-------------Example Update Key Released-------------------");
            }
        }
    }

	if (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_RELEASED)
	{
		char color[64] = "`xe0ff00"; 
		char* dispText = strcat(color,p_btnName);
		s3eDebugPrint(x+5, 20,  dispText, 0);
	}

	 if (s3ePointerGetInt(S3E_POINTER_AVAILABLE))
    {
        if (keyPressedState == S3E_KEY_STATE_DOWN)
		{
			char color[64] = "`xe0ff00"; 
			char* dispText = strcat(color,p_btnName);
			s3eDebugPrint(x+5, 20,  dispText, 0);
		}
		else if(keyPressedState == S3E_KEY_STATE_PRESSED)
		{
			DrawButtonRect(x, y, width, height,97,97,249);
			char color[64] = "`xe0ff00"; 
			char* dispText = strcat(color,p_btnName);
			s3eDebugPrint(x+5, 20,  dispText, 0);
		}
		else if(keyPressedState == S3E_KEY_STATE_RELEASED)
		{
			s3eSurfaceClear(0,0,0);
			char color[64] = "`xe0ff00"; 
			char* dispText = strcat(color,p_btnName);
			s3eDebugPrint(x+5, 20,  dispText, 0);
		}
		else
		{
			char color[64] = "`xD8F809";
			char* dispText = strcat(color,p_btnName);
			s3eDebugPrint(x+5, 20,  dispText, 0);
		}
    }
}

void HelloWorldInit()
{
	// Fill background blue
        s3eSurfaceClear(0, 0, 255);
		int scale;
		if (s3eSurfaceGetInt(S3E_SURFACE_WIDTH) < 320 || s3eSurfaceGetInt(S3E_SURFACE_HEIGHT) < 320)
        scale = 1;
		else if (s3eSurfaceGetInt(S3E_SURFACE_WIDTH) < 480 || s3eSurfaceGetInt(S3E_SURFACE_HEIGHT) < 480)
        scale = 2;
		else
        scale = 3;
		s3eDebugSetInt(S3E_DEBUG_FONT_SCALE, scale);
		s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, HelloWorldRotationCallBack, NULL);
}

void HelloWorldRender()
{
	s3eDebugPrint(120, 150, "`x000000Hello, World!", 0);
	createButton("Exit");
	buttonStatus = "Exit";
	s3eSurfaceShow();
}

void HelloWorldShutDown()
{
	std::hash_map<char*, char*>::iterator ihashMap; 
	if(hashMap.size() > 1) {
		ihashMap = hashMap.find("ShowAt");
		s3eSurfaceUnRegister(S3E_SURFACE_SCREENSIZE, HelloWorldRotationCallBack);
		if(ihashMap->second == "end" || ihashMap->second == "both"){
			 vservManagerFetchingAdData();
		}
		else if(ihashMap->second == "start"){
			s3eDeviceExit();
		}
	}
}

bool HelloWorldUpdate()
{
	if(s3eKeyboardGetState(s3eKeyBack) & S3E_KEY_STATE_RELEASED)
    {
		s3eDebugTraceLine("back check");
		std::hash_map<char*, char*>::iterator ihashMap; 
		if(hashMap.size() > 1) {
		ihashMap = hashMap.find("ShowAt");
		if(ihashMap->second == "both"){
			 vservManagerFetchingAdData();
		}else if(ihashMap->second == "end") {
			 vservManagerFetchingAdData();
		}
		else if(ihashMap->second == "start"){
			s3eDeviceExit();
			return false;
		}
    }
	}
	if(CheckButtonStatus("Exit") & S3E_KEY_STATE_RELEASED) /*S3E_KEY_STATE_PRESSED)*/
    {
		s3eDebugTraceLine("Exit check");
			HelloWorldShutDown();
			return false;
	}
	return true;
}

int HelloWorldMain() 
{
	HelloWorldInit();
	while (!s3eDeviceCheckQuitRequest())
	{
		s3eDeviceYield(0);
        s3eKeyboardUpdate();
        s3ePointerUpdate();
		HelloWorldRender();
		if(!HelloWorldUpdate())
			break;
	}
	HelloWorldShutDown();
    return 0;
}


void vservManagerSkipAd()
{
	HelloWorldMain();
}
//--------------------------------------------------------------------------
// Main global function
//--------------------------------------------------------------------------
int main()
{
	std::hash_map<char*, char*>::iterator ihashMap; 
	hashInit();
	vservManagerInit(hashMap);
	if(hashMap.size() > 1) {
		ihashMap = hashMap.find("ShowAt");
		if(ihashMap->second == "start" || ihashMap->second == "both"){
			vservManagerFetchingAdData();
		}
		else{
			HelloWorldMain();
		}
	}
    return 0;
}
