// Fill out your copyright notice in the Description page of Project Settings.


#include "SMiniMapCompoundWidget.h"
#include "Widgets/SCanvas.h"
#include "Rendering/DrawElements.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMiniMapCompoundWidget::Construct(const FArguments& InArgs)
{
    ChildSlot
        [
            SNew(SCanvas)
        ]
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
