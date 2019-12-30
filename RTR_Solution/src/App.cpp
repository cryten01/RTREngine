#pragma once
#include "Utils.h"
#include "Framework.h"

Framework framework;

int main(int argc, char** argv)
{
	framework.init();
	framework.initRenderLoop();
	framework.destroy();
}