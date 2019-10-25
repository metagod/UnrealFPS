// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#ifndef __FPS_H__
#define __FPS_H__

#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetNameStringByIndex((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )

#include "EngineMinimal.h"


#endif
