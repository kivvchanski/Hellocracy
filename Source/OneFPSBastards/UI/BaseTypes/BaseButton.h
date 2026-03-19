// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

#include "BaseButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBaseButtonClicked);

class UTextBlock;

UCLASS()
class ONEFPSBASTARDS_API UBaseButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "BaseButton")
	FOnBaseButtonClicked OnBaseButtonClicked;

	UPROPERTY(EditDefaultsOnly, Category = "Components", meta = (BindWidget))
	UButton* BaseButton;

	UPROPERTY(EditDefaultsOnly, Category = "Components", meta = (BindWidget))
	UTextBlock* TextBlock_ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FText ButtonText = FText::FromString("ButtonText");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Color")
	FLinearColor NormalColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Color")
	FLinearColor HoveredColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Color")
	FLinearColor PressedColor = FLinearColor::White;

	FORCEINLINE void SetEnabled(bool NewEnabled) 
	{
		if (BaseButton) BaseButton->SetIsEnabled(NewEnabled);
	}

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void HandleOnClicked();

	UFUNCTION()
	void HandleOnHovered();
	UFUNCTION()
	void HandleOnUnhovered();
};
