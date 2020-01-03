#pragma once
#include "Utils.h"
#include "Framework.h"

using namespace RTREngine;

int main(int argc, char** argv)
{
	Framework::Instance().init();
	Framework::Instance().destroy();
}