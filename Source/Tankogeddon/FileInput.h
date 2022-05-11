// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "IFileInput.h"
#include "CoreMinimal.h"

#ifdef PLATFORM_IOS
class TANKOGEDDON_API iOSFileInput : public IFileInput
{
public:
	virtual void OpenFile() override;
	iOSFileInput();
	~iOSFileInput();
};
#endif

#ifdef PLATFORM_ANDROID
class TANKOGEDDON_API AndroidFileInput : public IFileInput
{
public:
	virtual void OpenFile() override;
	AndroidFileInput();
	~AndroidFileInput();
};
#endif
