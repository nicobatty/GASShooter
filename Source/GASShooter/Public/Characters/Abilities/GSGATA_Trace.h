// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WorldCollision.h"
#include "GSGATA_Trace.generated.h"

/**
 * Reusable, configurable trace TargetActor. Subclass with your own trace shapes.
 * Meant to be used with GSAT_WaitTargetDataUsingActor instead of the default WaitTargetData AbilityTask as the default
 * one will destroy the TargetActor.
 */
UCLASS()
class GASSHOOTER_API AGSGATA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
    AGSGATA_Trace();

	// Base targeting spread (degrees)
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float BaseSpread;

	// Aiming spread modifier
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float AimingSpreadMod;

	// Continuous targeting: spread increment
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float TargetingSpreadIncrement;

	// Continuous targeting: max increment
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float TargetingSpreadMax;

	// Current spread from continuous targeting
	float CurrentTargetingSpread;

	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	bool bUseAimingSpreadMod;

	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	FGameplayTag AimingTag;

	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	FGameplayTag AimingRemovalTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	float MaxRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, config, meta = (ExposeOnSpawn = true), Category = "Trace")
	FCollisionProfileName TraceProfile;

	// Does the trace affect the aiming pitch
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bTraceAffectsAimPitch;

	// Maximum hit results to return. 0 just returns the trace end point. < 0 returns infinite hit results.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	int32 MaxHitResults;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bIgnoreBlockingHits;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bTraceFromPlayerViewPoint;

	UFUNCTION(BlueprintCallable)
	virtual void ResetSpread();

	virtual float GetCurrentSpread() const;

	// Expose to Blueprint
	UFUNCTION(BlueprintCallable)
	void SetStartLocation(const FGameplayAbilityTargetingLocationInfo& InStartLocation);

	// Expose to Blueprint
	UFUNCTION(BlueprintCallable)
	virtual void SetShouldProduceTargetDataOnServer(bool bInShouldProduceTargetDataOnServer);

	// Expose to Blueprint
	UFUNCTION(BlueprintCallable)
	void SetDestroyOnConfirmation(bool bInDestroyOnConfirmation = false);

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	virtual void CancelTargeting() override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Traces as normal, but will manually filter all hit actors
	virtual void LineTraceWithFilter(TArray<FHitResult>& OutHitResults, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params);

	virtual void AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, FVector& OutTraceEnd, bool bIgnorePitch = false);

	virtual bool ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition);

	virtual void StopTargeting();

protected:
	TArray<TWeakObjectPtr<AGameplayAbilityWorldReticle>> ReticleActors;

	virtual FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<FHitResult>& HitResults) const;
	virtual TArray<FHitResult> PerformTrace(AActor* InSourceActor)  PURE_VIRTUAL(AGSGATA_Trace, return TArray<FHitResult>(););

	virtual void SpawnReticleActor(FVector Location, FRotator Rotation);
	virtual void DestroyReticleActors();
};