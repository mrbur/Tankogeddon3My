// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "IFileInput.h"
#include "CoreMinimal.h"



#if PLATFORM_IOS
#import <UIKit/UIKit.h>
// UIImagePickerControllerDelegate to respond to user interactions
// UINavigationControllerDelegate because we want to present the photo library
// modally
@interface MyViewController : UIViewController <UIImagePickerControllerDelegate,
	UINavigationControllerDelegate>
	@property(nonatomic, retain)
	UIImage* chosenImage;
+(void)runSelectPhoto;
@end


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
